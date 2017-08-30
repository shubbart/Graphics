#include "graphics\Context.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\Draw.h"
#include "graphics\Load.h"
#include "glm\ext.hpp"

int main()
{
	Context context;
	context.init(800, 800);

	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1,1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};
	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Texture floor_normal = loadTexture("../../resources/textures/face.jpg");
	Shader standard = loadShader("../../resources/shaders/standard.vert",
								"../../resources/shaders/standard.frag");

//	Geometry spear = loadGeometry("../../resources/models/soulspear.obj");
//
//	Texture tex = loadTexture("../../resources/textures/soulspear_diffuse.tga");
//	Texture floorTex = loadTexture("../../resources/textures/floor.jpg");
//
//	Shader lambert = loadShader("../../resources/shaders/lambert.vert",
//		"../../resources/shaders/lambert.frag");
//
//	Shader point = loadShader("../../resources/shaders/point.vert",
//		"../../resources/shaders/point.frag");
//
	Framebuffer screen = { 0,800,800 };

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4), 
						glm::vec3(0, 1, 0), 
						glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 100.f);
	glm::mat4 go_model;

	glm::vec3 dl_dir = glm::normalize(glm::vec3(1, -1, 1));

	while (context.step())
	{
		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, tslot = 0;
		float time = context.getTime();

		setUniforms(standard, loc, tslot, cam_proj, cam_view, go_model, floor_normal, dl_dir);

		s0_draw(screen, standard, quad);
	}

	context.term();
	return 0;
}