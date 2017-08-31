#version 450
// final output
out vec4 outColor;

// input from frag shader
in vec2 vUV;
in vec3 vPos;
in mat4 vTBN;

// Camera
layout(location = 1) uniform mat4 view;

// Surface Material Data
layout(location = 3) uniform sampler2D diffuseMap;
layout(location = 4) uniform sampler2D specularMap;
layout(location = 5) uniform sampler2D normalMap;
layout(location = 6) uniform float     gloss;

// Light Data
layout(location =  7) uniform vec3  l_data;
layout(location =  8) uniform vec4  l_color;
layout(location =  9) uniform float l_intensity;
layout(location = 10) uniform vec4  l_ambient;
layout(location = 11) uniform int   light_type; // 0=dir, 1=pnt

// illumination model factors
float calc_lambert(in vec3 N, in vec3 L);
// PHONG
// AMBIENT
// ATTENUATION

void main()
{
	// Read surface data
	vec3 normal  = (vTBN*(2*texture(normalMap, vUV)-1)).xyz;	
	vec4 diffuse = texture(diffuseMap, vUV);

	// SPLIT VIEW COMPARISON, REMOVE LATER
	if(vPos.x < 0) normal = vTBN[2].xyz;

	// calculate light direction
	vec3 lDir = l_data;
	if(light_type == 1)
		lDir = normalize(vPos.xyz - l_data);

	// calculate our lighting factors
	float lamb = calc_lambert(normal, lDir);
	float ambi = 1;

	// calculate color terms
	vec4 outAmbient = diffuse * ambi * l_ambient;
	vec4 outDiffuse = diffuse * lamb * l_color * l_intensity;

	outColor = outAmbient + outDiffuse;

	//outColor = vec4(normal,1); // Test if they work!
}


float calc_lambert(in vec3 N, in vec3 L)
{
	return max(0,dot(N,-L));
}