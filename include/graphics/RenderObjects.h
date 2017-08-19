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

struct Framebuffer
{
	unsigned handle, width, height;
};

struct Texture
{
	unsigned handle;
};

Texture makeTexture(unsigned w, unsigned h, unsigned c, const unsigned char *pixels);
void freeTexture(Texture &t);