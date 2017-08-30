#version 450

in vec2 vUV;

layout(location = 3) uniform sampler2D normalMap;

out vec4 outColor;
in mat4 vTBN;

void main()
{
	outColor = texture(normalMap,vUV);

	//outColor = vTBN[0] * texture(normalMap,vUV);
}