#define GLM_FORCE_SWIZZLE

#include "glinc.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"

#ifdef _DEBUG
#include <iostream>
#endif

glm::vec4 calcTangent(const Vertex &v0, const Vertex &v1, const Vertex &v2)
{
	// Calculate the left and right edge of the triangle
	glm::vec4 p1 = v1.position - v0.position;
	glm::vec4 p2 = v2.position - v0.position;

	// Calculate the left and right edge of the triangle's UVs
	glm::vec2 t1 = v1.texCoord - v0.texCoord;
	glm::vec2 t2 = v2.texCoord - v0.texCoord;

	// Rotate the position edge to line up with the texture edge and normalize
	return glm::normalize((p1*t2.y - p2*t1.y) / (t1.x*t2.y - t1.y*t2.x));
}

Geometry makeGeometry(const Vertex *verts, size_t vsize,
	const unsigned *idxs, size_t isize)
{
	Geometry retval = { 0,0,0,isize };

	// Declare openGL objects and acquire handles
	glGenBuffers(1, &retval.vbo);
	glGenBuffers(1, &retval.ibo);
	glGenVertexArrays(1, &retval.handle);

	// Scope the variables
	glBindVertexArray(retval.handle);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);

	// Initialize the buffers
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(unsigned), idxs, GL_STATIC_DRAW);

	// Describe memory layout
	glEnableVertexAttribArray(0); // Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); // Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);
	glEnableVertexAttribArray(2); // texCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	glEnableVertexAttribArray(3); // Normals
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

	glEnableVertexAttribArray(4); // Tangent
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)56);

	glEnableVertexAttribArray(5); // Bitangent
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)72);


	// Unbind data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return retval;
}

void freeGeometry(Geometry &g)
{
	glDeleteBuffers(1, &g.vbo);
	glDeleteBuffers(1, &g.ibo);
	glDeleteBuffers(1, &g.handle);
	g = { 0, 0, 0, 0 };
}

Shader makeShader(const char *vert_source, const char *frag_source)
{
	Shader retval = { 0 };

	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vert_source, 0);
	glShaderSource(fs, 1, &frag_source, 0);

	glCompileShader(vs);
#ifdef _DEBUG
	GLint success = GL_FALSE;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(vs, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}



#endif _DEBUG

	glCompileShader(fs);
#ifdef _DEBUG
	success = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(fs, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}



#endif _DEBUG

	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);

	glLinkProgram(retval.handle);
#ifdef _DEBUG
	success = GL_FALSE;
	glGetProgramiv(retval.handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(retval.handle, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetProgramInfoLog(retval.handle, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}
#endif _DEBUG

	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

void freeShader(Shader &s)
{
	glDeleteProgram(s.handle);
	s = { 0 };
}

Texture makeTexture(unsigned w, unsigned h, unsigned c, const void *pixels, bool isFloat)
{
	Texture retval = { 0 };

	GLenum f = 0, i = 0; // External and internal format for color storage
	switch (c)
	{
	case 0: f = GL_DEPTH_COMPONENT; i = GL_DEPTH24_STENCIL8; break;
	case 1: f = GL_RED; i = GL_R32F; break;
	case 2: f = GL_RG; i = GL_RG32F;  break;
	case 3: f = GL_RGB; i = GL_RGB32F;  break;
	case 4: f = GL_RGBA; i = GL_RGBA32F; break;
	}

	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels);


	glBindTexture(GL_TEXTURE_2D, 0);
	return retval;
}

void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}

// Requires #define GLM_FORCE_SWIZZLE
void solveTangents(Vertex *v, size_t vsize, const unsigned *idxs, size_t isize)
{
	for (int i = 0; i < isize; i += 3)
	{
		glm::vec4 T = calcTangent(v[idxs[i]], v[idxs[i + 1]], v[idxs[i] + 2]);

		for (int j = 0; j < 3; ++j)
		{
			v[idxs[i + j]].tangent = glm::normalize(T + v[idxs[i + j]].tangent);

		}
	}

	for (int i = 0; i < vsize; ++i)
		v[i].bitangent = glm::vec4(glm::cross(v[i].normal.xyz(), v[i].tangent.xyz()), 0);

}




CubeTexture makeCubeMap(unsigned w, unsigned h, unsigned c,
	const void **pixels, bool isFloat)
{
	CubeTexture retval = { 0 };

	GLenum f = 0, i = 0; // External and internal format for color storage
	switch (c)
	{
		case 0: f = GL_DEPTH_COMPONENT; i = GL_DEPTH24_STENCIL8; break;
		case 1: f = GL_RED; i = GL_R32F; break;
		case 2: f = GL_RG; i = GL_RG32F;  break;
		case 3: f = GL_RGB; i = GL_RGB32F;  break;
		case 4: f = GL_RGBA; i = GL_RGBA32F; break;
	}

	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, retval.handle);

	// Right, Left
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f,
		(isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f,
		(isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[1]);

	// Top, Bottom
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f,
		(isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, w, h,
		0, GL_RGB, GL_UNSIGNED_BYTE, pixels[3]);

	// Back, Front
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f,
		(isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f,
		(isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[5]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return retval;
}

void freeCubeTexture(CubeTexture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}

Framebuffer makeFramebuffer(unsigned w, unsigned h, unsigned c, bool hasDepth,
	unsigned nTargets, unsigned nFloatTargets)
{
	Framebuffer retval = { 0,w,h, nTargets + nFloatTargets ,0,{ 0 } };

	// Code goes here!
	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);

	if (hasDepth) {
		retval.depthTarget = makeTexture(w, h, 0, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			retval.depthTarget.handle, 0);
	}

	const GLenum attachments[8] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	for (int i = 0; i < retval.nTargets && i < 8; ++i)
	{
		retval.targets[i] = makeTexture(w, h, c, 0, i >= nTargets);
		glFramebufferTexture(GL_FRAMEBUFFER, attachments[i],
			retval.targets[i].handle, 0);
	}

	glDrawBuffers(retval.nTargets, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return retval;
}

void freeFramebuffer(Framebuffer &fb);