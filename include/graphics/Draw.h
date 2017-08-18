#pragma once

struct Geometry;
struct Shader;
struct Framebuffer;

void s0_draw(const Framebuffer &f, const Shader &s, const Geometry &g);

void setUniform(const Shader &shader, int location, float value);

void clearFramebuffer(const Framebuffer &f);

void setUniform(const Shader &s, int location, int value);
