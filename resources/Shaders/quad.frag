#version 450

in vec2 vUV;

layout(location = 0) uniform sampler2D map;

out vec4 outColor;

void main()
{
	outColor = texture(map,vUV);
}