#include "controls.h"
#include "camera.h"
#include "shader.h"
#include "utils.h"
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>

void drawScene()
{
	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(0, 1, 0); // 4
	glVertex3f(1, 1, 0); // 3
	glVertex3f(0, 0, 0); // 7
	glVertex3f(1, 0, 0); // 8
	glVertex3f(1, 0, 1); // 5
	glVertex3f(1, 1, 0); // 3
	glVertex3f(1, 1, 1); // 1
	glVertex3f(0, 1, 0); // 4
	glVertex3f(0, 1, 1); // 2
	glVertex3f(0, 0, 0); // 7
	glVertex3f(0, 0, 1); // 6
	glVertex3f(1, 0, 1); // 5
	glVertex3f(0, 1, 1); // 2
	glVertex3f(1, 1, 1); // 1

	glEnd();
}

int main(int argc, char **argv)
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

	GLuint shaderProgram = makeShaderProgram();
	GLuint texUnitId = glGetUniformLocation(shaderProgram, "tex");

	// create texture
	// 1k pix, 4 B per pix
	//std::auto_ptr<unsigned char> textureBuf(makeColorTexture(32*32*4));
	int width = 0, height = 0;
	bool hasAlpha = false;
	std::auto_ptr<unsigned char> textureBuf(
	  loadPngTexture("textures/blocks/stone.png",
	    &width, &height, &hasAlpha)
	);
	const GLenum format = hasAlpha ? GL_RGBA : GL_RGB;

	// send it to OpenGL
	GLuint textureId = 0; // invalid
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// our texture is 32 by 32 unnormalized integer RGBA data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
	  GL_UNSIGNED_BYTE, textureBuf.get());
	glGenerateMipmap(GL_TEXTURE_2D);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "Error creating texture." << std::endl;
		return 1;
	}

	Camera camera;
	// track key down events until we get around to them
	glfwEnable(GLFW_STICKY_KEYS);

	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(texUnitId, 0);

	glEnable(GL_CULL_FACE);

	Controls ctl;
	bool running = true;
	while (running)
	{
		ctl.beginFrame(&camera);

		// build MVP matrix
		glLoadMatrixf(glm::value_ptr(camera.projection()));
		glMultMatrixf(glm::value_ptr(camera.view()));

		// clear screen
		glClear(GL_COLOR_BUFFER_BIT);

		drawScene();

		// swap double buffers
		glfwSwapBuffers();

		// keep running until the user hits ESC or closes the main window
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// cleanup
	glfwTerminate();
	return 0;
}

