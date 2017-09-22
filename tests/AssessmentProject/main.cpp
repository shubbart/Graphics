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

	// Skybox
	Geometry skyCube = loadGeometry("../../resources/models/cube.obj");
	glm::mat4 model;

	CubeTexture skybox = loadCubeMap("../../resources/textures/toh_ft.tga",
									"../../resources/textures/toh_bk.tga",
									"../../resources/textures/toh_up.tga",
									"../../resources/textures/toh_dn.tga",
									"../../resources/textures/toh_rt.tga",
									"../../resources/textures/toh_lf.tga");
	// Camera
	Camera cam;
	cam.proj = glm::perspective(45.f, width / height, 1.f, 30.f);

	Shader sbpass = loadShader("../../resources/shaders/cubeMap.vert", "../../resources/shaders/cubeMap.frag");

	Framebuffer screen = { 0,width, height };

	int loc = 0, slot = 0;
	while (context.step())
	{
		float time = context.getTime();
		// Skybox
		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);
		model = glm::rotate(time * .5f, glm::vec3(0,1,0)) * glm::scale(glm::vec3(30, 30, 30));
		loc = 0, slot = 0;
		setUniforms(sbpass, loc, slot, cam, model, skybox);
		s0_draw(screen, sbpass, skyCube);
	}
	context.term();
}