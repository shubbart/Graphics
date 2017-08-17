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