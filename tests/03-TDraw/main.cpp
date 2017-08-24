#include "graphics\Draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "graphics\GenShape.h"
#include "glm\ext.hpp"

#include <iostream>

int main()
{
	Context context;
	context.init(800, 800);

	Vertex verts[4] = { { { -.05,-.05,0,1 },{ 1, 0, 0, 1 },{ 0,0 } },
	{ { .05,-.05,0,1 },{ 0,1,0,1 },{ 1,0 } },
	{ { -.05,.05,0,1 },{ 0,0,1,1 },{ 1,1 } },
	{ { .05,.05,0,1 },{ .75,.05,0,1 },{ 0,1 } } };

	unsigned idxs[6] = { 0,1,2,3,2,1 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);
	Geometry gt = makeNGon(3750000, .5f);


	const char* vsource =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 6) uniform mat4 model;\n"
		"void main()\n"
		"{\n"
		"gl_Position = model * position;\n"
		"}\n";

	const char* fsource =
		"#version 450\n"
		"out vec4 outColor;\n"

		"layout(location = 0) uniform int key;\n"
		"layout(location = 1) uniform int key2;\n"
		"layout(location = 2) uniform int key3;\n"

		"layout(location = 3) uniform vec4 c0 = vec4(1,0,0,1);\n"
		"layout(location = 4) uniform vec4 c1 = vec4(0,1,0,1);\n"
		"layout(location = 5) uniform vec4 c2 = vec4(0,0,1,1);\n"

		"void main ()\n"
		"{\n"
		//"outColor = texture(map,vUV);\n;"
		"outColor = c0*(1-key) + c1*(1-key2) + c2*(1-key3);\n"
		"}\n";


	Framebuffer f = { 0,800,800 };

	Shader s = makeShader(vsource, fsource);

	glm::vec2 pos = { 0,0 };
	float xpos = 0, ypos = 0;
	float prevTime = 0;
	float speed = 5.0f;

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255,0,255,255,255,0 };

	Texture t_magyel = makeTexture(4, 1, 3, pixels);

	while (context.step())
	{
		clearFramebuffer(f);
		glm::mat4 rot = glm::translate(glm::vec3(.5, 0, 0)) * glm::rotate((float)context.getTime(), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(glm::vec3(.5, .5, 1));
		int loc = 0, tex = 0;
		setUniforms(s, loc, tex, (int)context.getKey('A'), (int)context.getKey('S'), (int)context.getKey('D'), glm::vec4(1,1,1,1), glm::vec4(1, 0, 1, 1), glm::vec4(.5, .5, .5, 1), rot);

		s0_draw(f, s, gt);
	}

	freeGeometry(g);
	freeGeometry(gt);
	freeShader(s);
	context.term();
	return 0;
}