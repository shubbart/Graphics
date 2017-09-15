#include "graphics\GameObjects.h"
#include "graphics\Vertex.h"
#include "graphics\Load.h"
#include "glm\ext.hpp"
#include "graphics\Context.h"
#include "graphics\draw.h"


void main()
{
	Context context;
	float width = 1280.f;
	float height = 720.f;
	context.init(width, height);

	Vertex vquad[] = { { { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } } };
	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	// Game objects/geometry
	SpecGloss objects[8];

	// Floor
	objects[0].geo = quad;

	objects[0].model = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0))
		* glm::scale(glm::vec3(5, 5, 1));

	objects[0].diffuse = loadTexture("../../resources/textures/four_diffuse.tga");
	objects[0].specular = loadTexture("../../resources/textures/four_specular.tga");
	objects[0].normal = loadTexture("../../resources/textures/four_normal.tga");
	objects[0].gloss = 4;


	// Soulspear
	objects[1].geo = loadGeometry("../../resources/models/soulspear.obj");
	objects[1].diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	objects[1].specular = loadTexture("../../resources/textures/soulspear_specular.tga");
	objects[1].normal = loadTexture("../../resources/textures/soulspear_normal.tga");
	objects[1].gloss = 4;

	// Earth
	objects[2].geo = loadGeometry("../../resources/models/sphere.obj");
	objects[2].diffuse = loadTexture("../../resources/textures/earth_diffuse.jpg");
	objects[2].specular = loadTexture("../../resources/textures/earth_specular.jpg");
	objects[2].normal = loadTexture("../../resources/textures/earth_normal.jpg");
	objects[2].gloss = 4;
	objects[2].model = glm::scale(glm::vec3(2, 2, 2)) * glm::translate(glm::vec3(2, 1, -1));

	// Camera
	Camera cam;
	cam.view = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	cam.proj = glm::perspective(45.f, width / height, 1.f, 10.f);

	// Skybox
	CubeTexture skybox = loadCubeMap("../../resources/textures/toh_rt.tga",
		"../../resources/textures/toh_lf.tga",
		"../../resources/textures/toh_up.tga",
		"../../resources/textures/toh_dn.tga",
		"../../resources/textures/toh_bk.tga",
		"../../resources/textures/toh_ft.tga");

	DirectionalLight dlights[2];

	dlights[0].range = 10;
	dlights[0].intensity = 1;
	dlights[0].color = glm::vec4(1, 1, 0, 1);
	dlights[0].direction = glm::normalize(glm::vec3(.8, -.8, -.4));

	//dlights[1].range = 10;
	//dlights[1].intensity = 1;
	//dlights[1].color = glm::vec4(0, 0, 1, 1);
	//dlights[1].direction = glm::normalize(glm::vec3(-1, 1, 0));

	Shader gpass = loadShader("../../resources/shaders/gpass.vert", "../../resources/shaders/gpass.frag");
	Shader cpass = loadShader("../../resources/shaders/cpass.vert", "../../resources/shaders/cpass.frag");
	Shader lpassD = loadShader("../../resources/shaders/lpassD.vert", "../../resources/shaders/lpassD.frag");
	Shader spassD = loadShader("../../resources/shaders/shadow.vert", "../../resources/shaders/shadow.frag");

	Framebuffer screen = { 0,width, height };
	Framebuffer gbuffer = makeFramebuffer(width, height, 4, true, 2, 2);
	Framebuffer lbuffer = makeFramebuffer(width, height, 4, false, 2, 0);
	Framebuffer sbuffer = makeFramebuffer(1024, 1024, 0, true, 0, 0);

	int loc = 0, slot = 0;
	while (context.step())
	{
		float time = context.getTime();
		objects[1].model = glm::rotate(time, glm::vec3(0, 1, 0));

		// GPass
		clearFramebuffer(gbuffer);
		setFlags(RenderFlag::DEPTH);
		for (int i = 0; i < 3; ++i)
		{
			loc = slot = 0;
			setUniforms(gpass, loc, slot, cam, objects[i]);
			s0_draw(gbuffer, gpass, objects[i].geo);
		}

		// LPass
		clearFramebuffer(lbuffer);
		for (int i = 0; i < 1; ++i)
		{
			/// SPass Pre-Pass
			clearFramebuffer(sbuffer);
			setFlags(RenderFlag::DEPTH);
			for (int j = 0; j < 3; ++j)
			{
				loc = slot = 0;
				setUniforms(spassD, loc, slot, dlights[i].getProjection(), dlights[i].getView(), objects[j].model);
				s0_draw(sbuffer, spassD, objects[j].geo);
			}

			setFlags(RenderFlag::ADDITIVE);
			loc = slot = 0;
			setUniforms(lpassD, loc, slot, cam, dlights[i], gbuffer.targets[3], gbuffer.targets[2], sbuffer.depthTarget);
			s0_draw(lbuffer, lpassD, quad);
		}

		// CPass
		loc = slot = 0;
		clearFramebuffer(screen);
		setFlags(RenderFlag::NONE);
		setUniforms(cpass, loc, slot, gbuffer.targets[0],
			lbuffer.targets[0]);
		s0_draw(screen, cpass, quad);
	}
	context.term();
}