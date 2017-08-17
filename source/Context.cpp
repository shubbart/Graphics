#include "../include/graphics/Context.h"
#include "glinc.h"

bool Context::init(size_t w, size_t h, const char *title)
{
	glfwInit();

	handle = glfwCreateWindow(w, h, title, 0, 0);
	glfwMakeContextCurrent(handle);

	glewExperimental = true;
	glewInit();

	return true;
}

bool Context::step()
{
	glfwPollEvents();
	glfwSwapBuffers(handle);

	return !glfwWindowShouldClose(handle);
}

bool Context::term()
{
	glfwTerminate();
	handle = nullptr;

	return true;
}

bool Context::getKey(int key)
{
	return glfwGetKey(handle, key);
}

bool Context::getMouseButton(int button)
{
	return glfwGetMouseButton(handle, button);
}

void Context::getMousePosition(double &x_out, double &y_out)
{
	return glfwGetCursorPos(handle, &x_out, &y_out);
}

double Context::getTime()
{
	return glfwGetTime();
}