#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Draw.h"
#include <iostream>

int main()
{
	Context context;
	context.init();

	Vertex verts[3] = { {{-.5f,-.5f,0,1}}, {{.5f,-.5f,0,1}}, {{0,.5f,0,1}} };

	unsigned idxs[3] = { 0,1,2 };

	Geometry g = makeGeometry(verts, 3, idxs, 3);

	const char* vsource =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"out vec4 vPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"gl_Position.x -= 0.5;\n"
		"gl_Position.y += 0.5;\n"
		"vPos = position;\n"
		"}\n";

	const char* fsource =
		"#version 450\n"
		"out vec4 outColor;\n"
		"in vec4 vPos;\n"
		"void main ()\n"
		"{\n"
		"outColor = 1.0 - vPos;\n"
		"}\n";
		//"void main () { outColor = vec4(0.0, 1.0, 1.0, 1.0); }\n";

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