#include "glinc.h"
#include "graphics\RenderObjects.h"
#include "graphics\Draw.h"

void s0_draw(const Framebuffer &f, const Shader &s, const Geometry &g)
{
	// Bind
	glBindFramebuffer(GL_FRAMEBUFFER, f.handle);
	glUseProgram(s.handle);
	glBindVertexArray(g.handle);

	// Area
	glViewport(0, 0, f.width, f.height);

	// Draw
	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void setUniform(const Shader &shader, int location, float value)
{
	glProgramUniform1f(shader.handle, location, value);
}

void clearFramebuffer(const Framebuffer &f)
{
	glBindFramebuffer(GL_FRAMEBUFFER, f.handle);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setUniform(const Shader &s, int location, int value)
{
	glProgramUniform1i(s.handle, location, value);
}
void setUniform(const Shader &s, int location, const Texture &value, unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, value.handle);

	glProgramUniform1i(s.handle, location, slot);
}
