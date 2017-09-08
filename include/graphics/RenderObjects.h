#pragma once
#include "glm\glm.hpp"

struct Vertex;

struct Geometry
{
	// Vertex array object, Vertex Buffer Object, Index Buffer Object
	unsigned handle, vbo, ibo, size;
};

Geometry makeGeometry(const Vertex *verts, size_t vsize, const unsigned *idxs, size_t isize);
void freeGeometry(Geometry &g);

struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vert_source, const char *frag_source);
void freeShader(Shader &s);

struct Texture
{
	unsigned handle;
};

struct Framebuffer
{
	unsigned handle, width, height, nTargets;
	Texture depthTarget;
	Texture targets[8];
};



Texture makeTexture(unsigned w, unsigned h, unsigned c, const void *pixels, bool isFloat = false);
void freeTexture(Texture &t);

void solveTangents(Vertex *v, size_t vsize, const unsigned *idx, size_t isize);

Framebuffer makeFramebuffer(unsigned w, unsigned h, unsigned c, bool hasDepth, unsigned nTargets, unsigned nFlatTargets);