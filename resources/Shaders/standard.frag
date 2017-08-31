#version 450
// final output
out vec4 outColor;
out vec4 outNormal;
out vec4 outFinal;

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
float calc_phong(in vec3 N, in vec3 L, in vec3 V, in float specPower);



void main()
{
	// Read surface data
	vec3 normal  = (vTBN*(2*texture(normalMap, vUV)-1)).xyz;	
	vec4 diffuse = texture(diffuseMap, vUV);
	vec4 specular = texture(specularMap, vUV);
	float attenuation = 1;

	// SPLIT VIEW COMPARISON, REMOVE LATER
	//if(vPos.x < 0) normal = vTBN[2].xyz;

	// Calculate light direction
	vec3 lDir = l_data;
	if(light_type == 1)
	{
		lDir = normalize(vPos.xyz - l_data);
		attenuation = 1.0 /distance(vPos.xyz, l_data);
	}
		
	// Calculate our lighting factors
	float lamb = calc_lambert(normal, lDir);
	float ambi = 1;

	float spec = calc_phong(normal,lDir, normalize(view[3].xyz - vPos), gloss);

	// Calculate color terms
	vec4 outAmbient = diffuse * ambi * l_ambient;
	vec4 outDiffuse = diffuse * lamb * l_color * l_intensity * attenuation;
	vec4 outSpecular = specular * spec * l_color * l_intensity * attenuation;


	//vec4 outAmbient = diffuse * ambi * vec4(0,0,1,1);
	//vec4 outDiffuse = diffuse * lamb * vec4(0,1,0,1) * l_intensity * attenuation;
	//vec4 outSpecular = specular * spec * vec4(1,0,0,1) * l_intensity* attenuation;

	outNormal = vec4(normal,0);
	outFinal = outAmbient + outDiffuse + outSpecular;
	
	//outColor = outSpecular;

	//outColor = vec4(normal,1); // Test if they work!
}


float calc_lambert(in vec3 N, in vec3 L)
{
	return max(0,dot(N,-L));
}

float calc_phong(in vec3 N, in vec3 L, in vec3 V, in float specPower)
{
	if(dot(N,-L) <= 0) return 0;
	vec3 R = reflect(L,N);
	return pow(max(0,dot(V, -R)), specPower);
}