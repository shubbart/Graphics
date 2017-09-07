#pragma once

#include "graphics\RenderObjects.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat3 rotation;

	Transform *parent = nullptr;

	glm::mat4 getLocal() const; // Local matrix
	glm::mat4 getGlobal() const; // Global: parent.global * local

	void axisAngle(float angle, const glm::vec3 &axis = { 0,1,0 });
	void lookAt(const glm::vec3 &target, const glm::vec3 &up = { 0,1,0 });
};

struct Camera
{
	glm::mat4 proj;
	glm::mat4 view;

	Transform transform;

	float fov = 45.f, aspect = 8.f / 6.f, near = 0.01f, far = 100.0f;

	glm::mat4 getProjection() const;
	glm::mat4 getView() const;
};

struct Mesh
{
	Transform transform;

	Geometry geometry;
};

struct FlyCam
{
	Camera camera;

	float speed;
	// add any members needed to keep track of input


	// Check input and modify camera.transform accordingly
	void update(float dt);
};

struct OrbitCam
{
	Camera camera;

	float speed;
	// add any members needed to keep track of input
	glm::vec3 target;

	// Check input and modify camera.transform accordingly
	void update(float dt);
};

struct SpecGloss
{
	Geometry geo;

	glm::mat4 model;
	Texture diffuse;
	Texture specular;
	Texture normal;
	float gloss;
};

struct StandardLight
{
	glm::vec3 direction;
	glm::vec4 color;
	float intensity;
	glm::vec4 ambient;
	int type;
};

namespace _internal
{
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Camera &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const SpecGloss &val);
}