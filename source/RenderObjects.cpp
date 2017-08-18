#include "glinc.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"

#ifdef _DEBUG
#include <iostream>
#endif

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
	glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); // Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);


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