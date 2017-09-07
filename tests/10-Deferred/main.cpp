#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "glm\ext.hpp"
#include "graphics\GameObjects.h"


int main()
{
	Context context;
	context.init(1280, 720);

	Camera cam;
	SpecGloss objects[8];
	StandardLight l;

	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Shader gpass = loadShader("../../resources/shaders/gpass.vert",
		"../../resources/shaders/gpass.frag");

	Shader cpass = loadShader("../../resources/shaders/cpass.vert",
		"../../resources/shaders/cpass.frag");

	////////////////////////
	/// Model Data

	// Floor
	objects[0].geo = quad;
	objects[0].model = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(5, 5, 1));
	objects[0].normal = loadTexture("../../resources/textures/four_normal.tga");
	objects[0].diffuse = loadTexture("../../resources/textures/four_diffuse.tga");
	objects[0].specular = loadTexture("../../resources/textures/sfour_specular.tga");
	objects[0].gloss = 4.0f;

	// Soulspear
	objects[1].geo = loadGeometry("../../resources/models/soulspear.obj");
	objects[1].normal = loadTexture("../../resources/textures/soulspear_normal.tga");
	objects[1].diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	objects[1].specular = loadTexture("../../resources/textures/soulspear_specular.tga");
	objects[1].gloss = 4.0f;

	// Earth
	objects[2].geo = loadGeometry("../../resources/models/sphere.obj");
	objects[2].normal = loadTexture("../../resources/textures/earth_normal.tga");
	objects[2].diffuse = loadTexture("../../resources/textures/earth_diffuse.tga");
	objects[2].specular = loadTexture("../../resources/textures/earth_specular.tga");
	objects[2].gloss = 4.0f;
	objects[2].model = glm::scale(glm::vec3(2, 2, 2)) * glm::translate(glm::vec3(2, 1, -1));
	

	//////////////////////////
	// Camera Data
	cam.view = glm::lookAt(glm::vec3(0, 2, 5),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0));
	cam.proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 10.f);

	//////////////////////////
	// Light
	l.direction = glm::normalize(glm::vec3(.2, -1, -1));
	l.color = glm::vec4(.7, .5, .9, 1);
	l.intensity = 5.0;
	l.ambient = glm::vec4(.2, .2, .01, 1);
	l.type = 0;


	Framebuffer screen = { 0, 1280, 720 };
	Framebuffer gBuffer = makeFramebuffer(1280, 720, 4, true, 2, 2);

	while (context.step())
	{
		float time = context.getTime();
		//objects[0].model = glm::rotate(time, glm::vec3(0, 1, 0));
		objects[1].model = glm::rotate(time * 50, glm::vec3(0, -1, 0));

		l.color = glm::vec4(.1, .1, .1, 1) * time;

		// GPass
		clearFramebuffer(gBuffer);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, slot = 0;

		for(int i = 0; i < 3; ++i)
		{
			loc = 0, slot = 0;
			setUniforms(gpass, loc, slot,
				cam,	// Camera data
				objects[i] // Model data
				//,l.direction, l.color, l.intensity, l.ambient, l.type		  // Light data
																	);
			s0_draw(gBuffer, gpass, objects[i].geo);
		}

		// CPass

		loc = 0, slot = 0;
		setUniforms(cpass, loc, slot,
					gBuffer.targets[0]);
		s0_draw(screen, cpass, quad);
	}
	context.term();
	return 0;
}