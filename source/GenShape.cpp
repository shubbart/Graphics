#include <cmath>
#include "..\include\graphics\GenShape.h"
#include "..\include\graphics\Vertex.h"
#include "..\include\graphics\RenderObjects.h"

Geometry makeNGon(size_t N, float r)
{
	unsigned vsize = N + 1;
	unsigned isize = N * 3;

	Vertex *verts = new Vertex[vsize];
	unsigned *idxs = new unsigned[isize];

	float a = (2 * 3.14159265359) / N;

	verts[N] = { { 0,0,0,1 },{ 1,1,1,1 } };

	for (int i = 0; i < N; ++i)
	{
		float x = cos(i*a)*r;
		float y = sin(i*a)*r;

		verts[i] = { {x,y,0,1},{x,y,0,1} };

		idxs[i * 3] = N;
		idxs[i * 3 +1] = i;
		idxs[i * 3 +2] = i == N-1? 0 : i + 1;
	}

	Geometry ret = makeGeometry(verts, vsize, idxs, isize);

	delete[] verts;
	delete[] idxs;
	return ret;
}

Geometry makeCheckerboard(int dim, float size)
{
	unsigned vdim = dim + 1;
	unsigned vsize = vdim*vdim;
	unsigned isize = 3*2*dim*dim;

	Vertex *verts = new Vertex[vsize];
	unsigned *idxs = new unsigned[isize];

	float step = size / dim;
	float offset = size / 2;
	int l = 0;

	for (int i = 0; i < vsize; ++i)
	{
		float x = (i % vdim)*step - offset;
		float y = (i / vdim)*step - offset;

		verts[i].position = { x, y, 0, 1 };
		verts[i].color = { rand()/(float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1 };

		if (i == 1 && dim > 1 || i % vdim != vdim - 1 && i / vdim != vdim - 1 && l < isize)
		{
			idxs[l++] = i;
			idxs[l++] = i + 1;
			idxs[l++] = i + vdim;

			idxs[l++] = i + 1;
			idxs[l++] = i + vdim;
			idxs[l++] = i + vdim + 1;
		}
	}

	for (int i = 0; i < vdim; ++i)
	{
		float y = i * step;
		for (int j = 0; j < vdim; ++j)
		{
			float x = j * step;
		}
	}

	Geometry ret = makeGeometry(verts, vsize, idxs, isize);

	delete[] verts;
	delete[] idxs;
	return ret;
}
