#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ\tiny_obj_loader.h"

#include <fstream>
#include <streambuf>
#include <random>

Texture loadTexture(const char *path)
{
	Texture retval = { 0 };

	int w, h, c;
	unsigned char *pixels;

	stbi_set_flip_vertically_on_load(true);

	pixels = stbi_load(path, &w, &h, &c, STBI_default);

	retval = makeTexture(w, h, c, pixels);

	stbi_image_free(pixels);

	return retval;
}

CubeTexture loadCubeMap(const char *path_Xpos, const char *path_Xneg,
	const char *path_Ypos, const char *path_Yneg,
	const char *path_Zpos, const char *path_Zneg)
{
	CubeTexture retval = { 0 };

	int w, h, c;
	const void **pixels = new const void *;

	pixels[0] = stbi_load(path_Xpos, &w, &h, &c, STBI_default);
	pixels[1] = stbi_load(path_Xneg, &w, &h, &c, STBI_default);
	pixels[2] = stbi_load(path_Ypos, &w, &h, &c, STBI_default);
	pixels[3] = stbi_load(path_Yneg, &w, &h, &c, STBI_default);
	pixels[4] = stbi_load(path_Zpos, &w, &h, &c, STBI_default);
	pixels[5] = stbi_load(path_Zneg, &w, &h, &c, STBI_default);

	retval = makeCubeMap(w, h, c, pixels);

	return retval;
}

std::string fileToString(const char *path)
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	return str;
}

Shader loadShader(const char *vpath, const char *fpath)
{
	Shader retval = { 0 };

	std::string vsource = fileToString(vpath);
	std::string fsource = fileToString(fpath);

	retval = makeShader(vsource.c_str(), fsource.c_str());

	return retval;
}

glm::vec4 randColor()
{
	return{ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1 };
}

Geometry loadGeometry(const char *path)
{
	Geometry retval = { 0,0,0,0 };
	tinyobj::attrib_t attrib;				// Vertex Data is stored.
	std::vector<tinyobj::shape_t> shapes;	// Triangular data, indices.
	std::vector<tinyobj::material_t> materials;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	size_t isize = shapes[0].mesh.indices.size();
	size_t *indices = new unsigned[isize];

	size_t vsize = isize;
	Vertex *verts = new Vertex[vsize];

	for (int i = 0; i < isize; ++i)
	{
		indices[i] = i;

		int pi = shapes[0].mesh.indices[i].vertex_index;
		int ni = shapes[0].mesh.indices[i].normal_index;
		int ti = shapes[0].mesh.indices[i].texcoord_index;

		const float *p = &attrib.vertices[pi * 3];  // 3x
		const float *n = &attrib.normals[ni * 3];   // 3x
		const float *t = &attrib.texcoords[ti * 2]; // 2x

		verts[i].position = { p[0],p[1],p[2],1 };
		verts[i].texCoord = { t[0],t[1] };
		verts[i].normal = { n[0],n[1],n[2],0 };
	}

	retval = makeGeometry(verts, vsize, indices, isize);

	delete[] verts;
	delete[] indices;
	return retval;
}