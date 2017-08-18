#include "graphics\Draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "graphics\GenShape.h"

int main()
{
	Context context;
	context.init(800, 800);

	Vertex verts[7] = { { { -.05,-.05,0,1 },{ 1, 0, 0, 1 } },
		{ {.05,-.05,0,1},{ 0,1,0,1 } },
		{ {-.05,.05,0,1},{ 0,0,1,1 } },
		{ {.05,.05,0,1},{ .75,.05,0,1 } },
		{ { .05,-.05,0,1 },{ .5,.75,0,1 } },
		{ { -.05,.05,0,1 },{ 0,.75,.5,1 } },
		{ { .05,.05,0,1 },{ .0,.05,.75,1 } } };

	unsigned idxs[6] = { 0,1,2,3,2,1 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);
	Geometry gt = makeNGon(3750000, .15f);

	const char* vsource =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 0) uniform float time;\n"
		"out vec4 vPos;\n"
		"out vec4 vColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"gl_Position.x += sin(time);\n"
		"gl_Position.y += tan(time)*tan(time)  / 2;\n"
		"vColor = color;\n"
		"vColor.x += sin(time);\n"
		"vColor.y += sin(time)/2;\n"
		"vColor.z += sin(time)/3;\n"
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

	const char* vsource2 =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 0) uniform float time;\n"
		"out vec4 vPos;\n"
		"out vec4 vColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"gl_Position.x += -sin(time);\n"
		"gl_Position.y += -tan(time)*tan(time)  / 2;\n"
		"vColor = color;\n"
		"vColor.x += sin(time);\n"
		"vColor.y += sin(time)/2;\n"
		"vColor.z += sin(time)/3;\n"
		"vPos = position;\n"
		"}\n";

	const char* fsource2 =
		"#version 450\n"
		"out vec4 outColor;\n"
		"in vec4 vPos;\n"
		"in vec4 vColor;\n"
		"void main ()\n"
		"{\n"
		"outColor = vColor;\n"
		"}\n";

	const char* vsource3 =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 0) uniform float time;\n"
		"out vec4 vPos;\n"
		"out vec4 vColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"gl_Position.x += -sin(time)*sin(time);\n"
		"gl_Position.y += -tan(time)  / 2;\n"
		"vColor = color;\n"
		"vColor.x += sin(time);\n"
		"vColor.y += sin(time)/2;\n"
		"vColor.z += sin(time)/3;\n"
		"vPos = position;\n"
		"}\n";

	const char* fsource3 =
		"#version 450\n"
		"out vec4 outColor;\n"
		"in vec4 vPos;\n"
		"in vec4 vColor;\n"
		"void main ()\n"
		"{\n"
		"outColor = vColor;\n"
		"}\n";

	const char* vsource4 =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 0) uniform float time;\n"
		"out vec4 vPos;\n"
		"out vec4 vColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"gl_Position.x += sin(time)*sin(time);\n"
		"gl_Position.y += tan(time) / 2;\n"
		"vColor = color;\n"
		"vColor.x += sin(time);\n"
		"vColor.y += sin(time)/2;\n"
		"vColor.z += sin(time)/3;\n"
		"vPos = position;\n"
		"}\n";

	const char* fsource4 =
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
	Shader s2 = makeShader(vsource2, fsource2);
	Shader s3 = makeShader(vsource3, fsource3);
	Shader s4 = makeShader(vsource4, fsource4);

	while (context.step())
	{
		clearFramebuffer(f);
		//setUniform(s, 0, (float)context.getTime());
		//setUniform(s, 1, context.getKey(' '));
		setUniform(s, 0, (float)context.getTime());
		setUniform(s2, 0, (float)context.getTime());
		setUniform(s3, 0, (float)context.getTime());
		setUniform(s4, 0, (float)context.getTime());
		s0_draw(f, s, gt);
		s0_draw(f, s2, gt);
		s0_draw(f, s3, gt);
		s0_draw(f, s4, gt);
	}

	freeGeometry(g);
	freeShader(s);
	context.term();
	return 0;
}