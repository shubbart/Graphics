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

	//Texture tex = loadTexture("../../resources/textures/bear.jpg");
	//Texture tex = loadTexture("../../resources/textures/bear_stand.png");
	Texture tex = loadTexture("../../resources/textures/sprite.png");
	//Texture tex = loadTexture("../../resources/textures/red_run.png");

	Vertex vquad[] = {
						{ { -1,-1,0,1 },{},{ 0,0 } },
						{ { 1,-1,0,1 },{},{ 1,0 } },
						{ { 1, 1,0,1 },{},{ 1,1 } },
						{ { -1,1,0,1 },{},{ 0,1 } }
					};

	unsigned quadidx[] = { 0,1,3,1,2,3 };
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Geometry cube = loadGeometry("../../resources/models/cube.obj");
	Geometry spear = loadGeometry("../../resources/models/soulspear.obj");

	Shader sq = loadShader("../../resources/shaders/test.vert", "../../resources/shaders/test.frag");

	Shader scube = loadShader("../../resources/shaders/cube.vert", "../../resources/shaders/cube.frag");

	Framebuffer screen = { 0,800,800 };

	while (context.step())
	{
		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, tslot = 0;
		float time = context.getTime();

		glm::mat4 mod_cube = glm::rotate(time, glm::vec3(.75, 1, .25))*glm::scale(glm::vec3(.5, .5, .5));		
		setUniforms(scube, loc, tslot, mod_cube, tex);
		s0_draw(screen, scube, cube);

		//setUniforms(sq, loc, tslot, tex, (int)(time * 3) % 4, 4, 4);
		//s0_draw(screen, sq, quad);
		//s0_draw(screen, sq, cube);

/*		glm::mat4 mod_spear = glm::rotate(time, glm::vec3(1, 1, 1))
			*glm::scale(glm::vec3(.12, .12, .12));
		setUniforms(scube, loc, tslot, mod_spear, tex);
		s0_draw(screen, scube, spear);*/
	}

	context.term();
	return 0;
}