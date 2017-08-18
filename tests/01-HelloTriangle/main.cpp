#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Draw.h"
#include "graphics\GenShape.h"
#include <iostream>

int main()
{
	Context context;
	context.init(800,800);

	Vertex verts[7] = { {{0,0,0,1},{ 1, 0, 0, 1 } },
						{{.5f,0,0,1},{ 0,1,0,1 } },
						{ { .25f,.55f,0,1 },{ .75,.5f,0,1 } },
						{ { -.25f,.55f,0,1 },{ 0,.75,.5,1 } },
						{{-.5f,0,0,1},{ 0,0,1,1 } },						
						{ { -.25f,-.55f,0,1 },{ .5,.75,0,1 } },						
						{ { .25f,-.55f,0,1 },{ .0,.5f,.75,1 } }};

	unsigned idxs[18] =	{ 0,1,2,
						0,2,3,
						0,3,4,
						0,4,5, 
						0,5,6,
						0,6,1};

	//Geometry g = makeGeometry(verts, 7, idxs, 18);
	//Geometry gt = makeNGon(3750000, .5f);

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

	Framebuffer f = { 0,800,800 };

	Shader s = makeShader(vsource, fsource);

	while (context.step())
	{
		//std::cout << context.getMouseButton(0) << std::endl;
		//s0_draw(f, s, g);
		//s0_draw(f, s, gt);
		
	}

	freeGeometry(g);
	freeShader(s);
	context.term();
	return 0;
}