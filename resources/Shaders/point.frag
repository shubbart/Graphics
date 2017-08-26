#version 450

in vec2 vUV;
in vec3 vPos;
in vec3 vNormal;

layout (location = 3) uniform sampler2D diffuse;
layout (location = 4) uniform vec3 lightPosition;
layout (location = 5) uniform vec4 lightColor;

out vec4 outColor;

void main()
{
    vec4 surfaceColor = texture(diffuse, vUV);
    vec3 DISP = vPos - lightPosition;

	float dist = length(DISP);
	float attenuation = 10.f/(dist*dist);
	vec3 L = normalize(DISP);
	vec3 N = vNormal;

    float lambertianFactor = max(0,dot(N, -L)) * attenuation;

    vec4 lambertianTerm = surfaceColor * lambertianFactor * lightColor;

    outColor = lambertianTerm;
}