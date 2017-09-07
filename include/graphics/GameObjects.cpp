#include "..\include\graphics\GameObjects.h"
#include "graphics\Draw.h"

void _internal::t_setUniform(const Shader & s, int & loc_io, int & tex_io, const Camera & val)
{
	t_setUniform(s, loc_io, tex_io, val.proj);
	t_setUniform(s, loc_io, tex_io, val.view);
}

void _internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, const SpecGloss &val)
{
	t_setUniform(s, loc_io, tex_io, val.model);

	t_setUniform(s, loc_io, tex_io, val.diffuse);
	t_setUniform(s, loc_io, tex_io, val.specular);
	t_setUniform(s, loc_io, tex_io, val.normal);
	t_setUniform(s, loc_io, tex_io, val.gloss);
}

void _internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, const DirectionalLight &val)
{
	t_setUniform(s, loc_io, tex_io, val.getProjection);
	t_setUniform(s, loc_io, tex_io, val.getView);
	t_setUniform(s, loc_io, tex_io, val.color);
	t_setUniform(s, loc_io, tex_io, val.intensity);
}
