#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Draw.h"
#include <iostream>

int main()
{
	Context context;
	context.init();

	Vertex verts[7] = { {{-.5f,-.5f,0,1},{ 1, 0, 0, 1 } },
						{{.5f,-.5f,0,1},{ 0,1,0,1 } },
						{{-.5f,.5f,0,1},{ 0,0,1,1 } },
						{{.5f,.5f,0,1},{ .75,.5f,0,1 } },
						{ { .5f,-.5f,0,1 },{ .5,.75,0,1 } },
						{ { -.5f,.5f,0,1 },{ 0,.75,.5,1 } },
						{ { .5f,.5f,0,1 },{ .0,.5f,.75,1 } }};

	unsigned idxs[6] = { 0,1,2,3,2,1 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);

	const char* vsource =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"out vec4 vPos;\n"
		"out vec4 vColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"vColor = color;\n"
		"vPos = position;\n"
		"}\n";

	const char* fsource =
		"#version 450\n"
		"out vec4 outColor;\n"
		"in vec4 vPos;\n"
		"in vec4 vColor;\n"
		"void main ()\n" 
		"{\n" 
		"outColor = vColor;\n"
		"}\n";

	Framebuffer f = { 0,800,600 };

	Shader s = makeShader(vsource, fsource);

	while (context.step())
	{
		//std::cout << context.getMouseButton(0) << std::endl;
		s0_draw(f, s, g);
		
	}

	freeGeometry(g);
	freeShader(s);
	context.term();
	return 0;
}