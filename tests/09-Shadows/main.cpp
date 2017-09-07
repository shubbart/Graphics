#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "glm\ext.hpp"


int main()
{
	Context context;
	context.init();

	Framebuffer screen = { 0, 800, 600 };
	Framebuffer fb_shadow = makeFramebuffer(1280, 720, 4, true, 3, 1);

	Vertex vquad[] = {
		// Position		Col	Tex		Normals
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);

	Geometry floor_geo = makeGeometry(vquad, 4, quadidx, 6);
	glm::mat4 floor_model = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0))
		* glm::scale(glm::vec3(5, 5, 1));


	Shader shdr_shadow = loadShader("../../resources/shaders/shadow.vert",
		"../../resources/shaders/shadow.frag");

	Shader shdr_direct = loadShader("../../resources/shaders/direct.vert",
		"../../resources/shaders/direct.frag");

	////////////////////////
	/// Model Data
	Geometry  ss_geo = loadGeometry("../../resources/models/soulspear.obj");
	glm::mat4 ss_model;

	Texture   ss_normal = loadTexture("../../resources/textures/soulspear_normal.tga");
	Texture   ss_diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	Texture   ss_specular = loadTexture("../../resources/textures/soulspear_specular.tga");
	float     ss_gloss = 4.0f;

	Geometry cube_geo = loadGeometry("../../resources/models/cube.obj");
	glm::mat4 cube_model = glm::translate(glm::vec3(2, 1, -1));

	//////////////////////////
	// Camera Data
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 2, 5),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, (float)screen.width/screen.height, 1.f, 10.f);

	//////////////////////////
	// Light
	glm::vec3 l_dir = glm::normalize(glm::vec3(.8, -1.f, -1.f));
	glm::mat4 l_proj = glm::ortho<float>(-10,10,-10,10,-10,10);
	glm::mat4 l_view = glm::lookAt(-l_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 l_color = glm::vec4(.7, .5, .9, 1);
	float     l_intensity = 3.0;
	glm::vec4 l_ambient = glm::vec4(.2, .2, .01, 1);
	int		  l_type = 0;

	glm::vec3 l2_dir = glm::normalize(glm::vec3(-.8, -1.f, 1.f));
	glm::mat4 l2_proj = glm::ortho<float>(10, 10, -10, 10, -10, 10);
	glm::mat4 l2_view = glm::lookAt(-l2_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 l2_color = glm::vec4(0, 1, 0, 1);
	float     l2_intensity = 3.0;
	glm::vec4 l2_ambient = glm::vec4(.2, .2, .01, 1);
	int		  l2_type = 0;

	while (context.step())
	{
		float time = context.getTime();
		
		// Shadow Pass
		setFlags(RenderFlag::DEPTH);
		clearFramebuffer(fb_shadow, false, true);

		ss_model = glm::rotate(time, glm::vec3(0, 1, 0));

		int loc = 0, slot = 0;

		setUniforms(shdr_shadow, loc, slot,
			l_proj, l_view, floor_model);
		s0_draw(fb_shadow, shdr_shadow, floor_geo);

		loc = slot = 0;
		setUniforms(shdr_shadow, loc, slot,
			l_proj, l_view, ss_model);
		s0_draw(fb_shadow, shdr_shadow, ss_geo);

		loc = slot = 0;
		setUniforms(shdr_shadow, loc, slot,
			l_proj, l_view, cube_model);
		s0_draw(fb_shadow, shdr_shadow, cube_geo);

		// Light Pass
		setFlags(RenderFlag::DEPTH);
		clearFramebuffer(screen);

		loc = slot = 0;
		setUniforms(shdr_direct, loc, slot, cam_proj, cam_view, 
			floor_model, l_proj, l_view, 
			fb_shadow.depthTarget);
		s0_draw(screen, shdr_direct, floor_geo);

		loc = slot = 0;
		setUniforms(shdr_direct, loc, slot, cam_proj, cam_view, 
			ss_model, l_proj, l_view, fb_shadow.depthTarget);
		s0_draw(screen, shdr_direct, ss_geo);

		loc = slot = 0;
		setUniforms(shdr_direct, loc, slot, cam_proj, cam_view, 
			cube_model, l_proj, l_view, fb_shadow.depthTarget);
		s0_draw(screen, shdr_direct, cube_geo);

		////////////// Second Light

		//// Shadow Pass
		//setFlags(RenderFlag::DEPTH);
		//clearFramebuffer(fb_shadow, false, true);

		//loc = 0, slot = 0;

		//setUniforms(shdr_shadow, loc, slot,
		//	l2_proj, l2_view, floor_model);
		//s0_draw(fb_shadow, shdr_shadow, floor_geo);

		//loc = slot = 0;
		//setUniforms(shdr_shadow, loc, slot,
		//	l2_proj, l2_view, ss_model);
		//s0_draw(fb_shadow, shdr_shadow, ss_geo);

		//loc = slot = 0;
		//setUniforms(shdr_shadow, loc, slot,
		//	l2_proj, l2_view, cube_model);
		//s0_draw(fb_shadow, shdr_shadow, cube_geo);

		//// Light Pass
		//setFlags(RenderFlag::DEPTH);
		//clearFramebuffer(screen);

		//loc = slot = 0;
		//setUniforms(shdr_direct, loc, slot, cam_proj, cam_view,
		//	floor_model, l2_proj, l2_view, l2_color,
		//	fb_shadow.depthTarget);
		//s0_draw(screen, shdr_direct, floor_geo);

		//loc = slot = 0;
		//setUniforms(shdr_direct, loc, slot, cam_proj, cam_view,
		//	ss_model, l2_proj, l2_view, l2_color,
		//	fb_shadow.depthTarget);
		//s0_draw(screen, shdr_direct, ss_geo);

		//loc = slot = 0;
		//setUniforms(shdr_direct, loc, slot, cam_proj, cam_view,
		//	cube_model, l2_proj, l2_view, l2_color,
		//	fb_shadow.depthTarget);
		//s0_draw(screen, shdr_direct, cube_geo);

	}
	context.term();
	return 0;
}