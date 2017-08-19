#include "graphics\Draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "graphics\GenShape.h"

int main()
{
	Context context;
	context.init(800, 800);

	Vertex verts[4] = { { { -.05,-.05,0,1 },{ 1, 0, 0, 1 }, {0,0} },
		{ {.05,-.05,0,1},{ 0,1,0,1 }, {1,0} },
		{ {-.05,.05,0,1},{ 0,0,1,1 }, {1,1} },
		{ {.05,.05,0,1},{ .75,.05,0,1 }, {0,1}} };

	unsigned idxs[6] = { 0,1,2,3,2,1 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);
	Geometry gt = makeNGon(3750000, 1.5f);


	const char* vsource =
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 2) in vec2 uv;\n"
		"layout(location = 0) uniform float time;\n"
		"layout(location = 2) uniform float xpos;\n"
		"layout(location = 3) uniform float ypos; \n"
		"out vec4 vPos;\n"
		"out vec4 vColor;\n"
		"out vec2 vUV;\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;\n"
		"gl_Position.x += xpos;\n"
		"gl_Position.y += ypos;\n"
		//"gl_Position.x += sin(time);\n"
		//"gl_Position.y += tan(time)*tan(time)  / 2;\n"
		"vColor = color;\n"
		"vUV = position.xy;\n"
		"vPos = position;\n"
		"}\n";

	const char* fsource =
		"#version 450\n"
		"out vec4 outColor;\n"
		"layout(location = 0) uniform float time;\n"
		"layout(location = 1) uniform int tog;\n"
		"layout(location = 4) uniform sampler2D map;\n"
		"in vec4 vPos;\n"
		"in vec4 vColor;\n"
		"in vec2 vUV;\n"
		"void main ()\n"
		"{\n"
		"vec2 uv = vUV;\n"
		//"uv.x += sin(time*1.5 + uv.y)/3;\n"
		"uv += sin(time);\n"
		"uv+= sin(.5*time+gl_FragCoord.x*0.01)/2 + sin(.05*time+gl_FragCoord.y*0.01)/2;\n"
		"outColor = texture(map, uv);\n"
		"outColor.x += sin(time) * 2;\n"
		"outColor.y -= cos(time);\n"
		"outColor.z += sin(time) / 2;\n"
	/*	"if(tog ==1)\n"
		"outColor = 1.0 - vColor;\n"
		"else outColor = vColor;\n"
		"outColor.r = 0.5+sin(time+gl_FragCoord.x/10)/2.0;\n"*/
		"}\n";

	//const char* vsource =
	//	"#version 450\n"
	//	"layout(location = 0) in vec4 position;\n"
	//	"layout(location = 1) in vec4 color;\n"
	//	"layout(location = 0) uniform float time;\n"
	//	"out vec4 vPos;\n"
	//	"out vec4 vColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"gl_Position = position;\n"
	//	"gl_Position.x += sin(time);\n"
	//	"gl_Position.y += tan(time)*tan(time)  / 2;\n"
	//	"vColor = color;\n"
	//	"vColor.x += sin(time);\n"
	//	"vColor.y += sin(time)/2;\n"
	//	"vColor.z += sin(time)/3;\n"
	//	"vPos = position;\n"
	//	"}\n";

	//const char* fsource =
	//	"#version 450\n"
	//	"out vec4 outColor;\n"
	//	"in vec4 vPos;\n"
	//	"in vec4 vColor;\n"
	//	"void main ()\n"
	//	"{\n"
	//	"outColor = vColor;\n"
	//	"}\n";

	//const char* vsource2 =
	//	"#version 450\n"
	//	"layout(location = 0) in vec4 position;\n"
	//	"layout(location = 1) in vec4 color;\n"
	//	"layout(location = 0) uniform float time;\n"
	//	"out vec4 vPos;\n"
	//	"out vec4 vColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"gl_Position = position;\n"
	//	"gl_Position.x += -sin(time);\n"
	//	"gl_Position.y += -tan(time)*tan(time)  / 2;\n"
	//	"vColor = color;\n"
	//	"vColor.x += sin(time);\n"
	//	"vColor.y += sin(time)/2;\n"
	//	"vColor.z += sin(time)/3;\n"
	//	"vPos = position;\n"
	//	"}\n";

	//const char* fsource2 =
	//	"#version 450\n"
	//	"out vec4 outColor;\n"
	//	"in vec4 vPos;\n"
	//	"in vec4 vColor;\n"
	//	"void main ()\n"
	//	"{\n"
	//	"outColor = vColor;\n"
	//	"}\n";

	//const char* vsource3 =
	//	"#version 450\n"
	//	"layout(location = 0) in vec4 position;\n"
	//	"layout(location = 1) in vec4 color;\n"
	//	"layout(location = 0) uniform float time;\n"
	//	"out vec4 vPos;\n"
	//	"out vec4 vColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"gl_Position = position;\n"
	//	"gl_Position.x += -sin(time)*sin(time);\n"
	//	"gl_Position.y += -tan(time)  / 2;\n"
	//	"vColor = color;\n"
	//	"vColor.x += sin(time);\n"
	//	"vColor.y += sin(time)/2;\n"
	//	"vColor.z += sin(time)/3;\n"
	//	"vPos = position;\n"
	//	"}\n";

	//const char* fsource3 =
	//	"#version 450\n"
	//	"out vec4 outColor;\n"
	//	"in vec4 vPos;\n"
	//	"in vec4 vColor;\n"
	//	"void main ()\n"
	//	"{\n"
	//	"outColor = vColor;\n"
	//	"}\n";

	//const char* vsource4 =
	//	"#version 450\n"
	//	"layout(location = 0) in vec4 position;\n"
	//	"layout(location = 1) in vec4 color;\n"
	//	"layout(location = 0) uniform float time;\n"
	//	"out vec4 vPos;\n"
	//	"out vec4 vColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"gl_Position = position;\n"
	//	"gl_Position.x += sin(time)*sin(time);\n"
	//	"gl_Position.y += tan(time) / 2;\n"
	//	"vColor = color;\n"
	//	"vColor.x += sin(time);\n"
	//	"vColor.y += sin(time)/2;\n"
	//	"vColor.z += sin(time)/3;\n"
	//	"vPos = position;\n"
	//	"}\n";

	//const char* fsource4 =
	//	"#version 450\n"
	//	"out vec4 outColor;\n"
	//	"in vec4 vPos;\n"
	//	"in vec4 vColor;\n"
	//	"void main ()\n"
	//	"{\n"
	//	"outColor = vColor;\n"
	//	"}\n";

	Framebuffer f = { 0,800,800 };

	Shader s = makeShader(vsource, fsource);
	//Shader s2 = makeShader(vsource2, fsource2);
	//Shader s3 = makeShader(vsource3, fsource3);
	//Shader s4 = makeShader(vsource4, fsource4);
	
	glm::vec2 pos = { 0,0 };
	float xpos = 0, ypos = 0;
	float prevTime = 0;
	float speed = 5.0f;

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = {255,0,255,255,255,0};

	Texture t_magyel = makeTexture(150, 1, 3, pixels);

	while (context.step())
	{
		float ct = context.getTime();
		float dt = ct - prevTime;
		prevTime = ct;
		glm::vec2 vel = { 0,0 };

		vel.y += context.getKey('W');
		vel.x -= context.getKey('A');
		vel.y -= context.getKey('S');
		vel.x += context.getKey('D');

		if (glm::length(vel) > 0)
		{
			pos += glm::normalize(vel) * dt * speed;
		}
	


		clearFramebuffer(f);
		setUniform(s, 0, (float)context.getTime());
		setUniform(s, 1, context.getKey(' '));
		setUniform(s, 2, pos.x);
		setUniform(s, 3, pos.y);

		setUniform(s, 4, t_magyel, 0);

		//setUniform(s, 0, (float)context.getTime());
		//setUniform(s2, 0, (float)context.getTime());
		//setUniform(s3, 0, (float)context.getTime());
		//setUniform(s4, 0, (float)context.getTime());
		s0_draw(f, s, gt);
		//s0_draw(f, s2, gt);
		//s0_draw(f, s3, gt);
		//s0_draw(f, s4, gt);
	}

	freeGeometry(g);
	freeShader(s);
	context.term();
	return 0;
}