#version 450

in vec2 vUV;

layout(location = 0) uniform sampler2D map;
layout(location = 1) uniform sampler2D map2;

out vec4 outColor;
vec4 crossblur(in sampler2D map, in vec2 uv, int it);
vec4 sobel(in smapler2D map, in vec2 UV);

void main()
{
	outColor = texture(map,vUV) + crossblur(map2, vUV, 64);
}

vec4 crossblur(in smapler3D map, in vec2 uv, int it)
{
	vec2 sDim = textureSize(map,0).xy;
	vec4 retval = vec4(0,0,0,0);

	for(int i = -it; i <= it; ++i)
	{
		retval += texture(map, uv + vec2(i,0)/sDim;
		retval += texture(map, uv + vec2(0,i)/sDim;
	}

	return retval / (it*4);
}

vec4 sobel(in sampler2D map, in vec2 UV)
{
	
}