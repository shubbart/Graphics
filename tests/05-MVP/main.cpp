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

	Geometry spear = loadGeometry("../../resources/models/soulspear.obj");

	Texture tex = loadTexture("../../resources/textures/soulspear_diffuse.tga");

	Shader mvplite = loadShader("../../resources/shaders/mvplite.vert",
							"../../resources/shaders/mvplite.frag");

	Framebuffer screen = { 0,800,800 };

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 100.f);
	glm::mat4 go_model;

	glm::vec3 targetPosition = glm::vec3(0, 1, 0);

	while (context.step())
	{
		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, tslot = 0;
		float time = context.getTime();

		if (context.getKey('A'))
			targetPosition.x += .1f;
		if (context.getKey('D'))
			targetPosition.x -= .1f;
		if (context.getKey('W'))
			targetPosition.y += .1f;
		if (context.getKey('S'))
			targetPosition.y -= .1f;
		//if (context.getKey('E'))
		//	targetPosition.z += 1.f;
		//if (context.getKey('Q'))
		//	targetPosition.z -= 1.f;

		//targetPosition.x += -context.getKey('A') | context.getKey('D');
		//targetPosition.y += -context.getKey('S') | context.getKey('W');
		//targetPosition.z += -context.getKey('Q') | context.getKey('E');


		cam_view = glm::lookAt(glm::vec3(0, 3, -4), targetPosition, glm::vec3(0, 1, 0));
		//cam_view = glm::lookAt(targetPosition, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));


		glm::mat4 mod_spear = glm::rotate(time * 6, glm::vec3(0, 1, 0))
		*glm::scale(glm::vec3(.5, .5, .5)); //Rotate
		setUniforms(mvplite, loc, tslot, cam_view, cam_proj, mod_spear, tex); // Rotating Uniform
		//setUniforms(mvplite, loc, tslot, cam_view, cam_proj, mod_spear, tex); // Static
		s0_draw(screen, mvplite, spear);
	}

	context.term();
	return 0;
}