#pragma once

// Foward declare GLFW's window
// Handling pointer type

struct GLFWwindow;

class Context
{
private:
	GLFWwindow *handle;

public:
	// Pop up window in Windows OS
	bool init(size_t width = 800, size_t height = 600, const char *title = "Graphics");

	// Updating the window
		// This determines the FPS
	bool step();

	bool term();

	bool getKey(int key);
	bool getMouseButton(int button);
	void getMousePosition(double &x_out, double &y_out);
	double getTime();
};
