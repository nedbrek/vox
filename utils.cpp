#include <GL/glew.h>
#include "utils.h"
#include <GL/glfw.h>
#include <fstream>
#include <iostream>

std::string fileToString(const char *filename)
{
	std::ifstream fs(filename);
	if (!fs.is_open())
	{
		std::cerr << "Unable to open shader " << filename << '.'
		  << std::endl;
		return 0; // invalid id
	}

	std::string fileString;
	std::string line;
	while (std::getline(fs, line))
	{
		fileString += line;
		fileString += '\n';
	}

	return fileString;
}

int initGraphics()
{
	// initialize GLFW
	int rc = glfwInit();
	if (rc != GL_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

	// open main window
	rc = glfwOpenWindow(1092, 614, 8, 8, 8, 8, 32, 0, GLFW_WINDOW);
	if (rc != GL_TRUE)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return 1;
	}

	if (glewInit() != GLEW_OK) // must be after OpenGL context
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return 1;
	}

	glfwSwapInterval(1);

	return 0; // success
}

