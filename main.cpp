#include "chunk.h"
#include "controls.h"
#include "camera.h"
#include "shader.h"
#include "utils.h"
#include <FTGL/ftgl.h>
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

/// populate a chunk - should probably take a random seed
void fillChunk(unsigned char *blockIds)
{
	for(unsigned i = 0; i < 4096; ++i)
		blockIds[i] = 0;

	for(unsigned x = 0; x < 16; ++x)
	for(unsigned z = 0; z < 16; ++z)
		blockIds[Chunk::index(x, 0, z)] = 1;
}

int main(int argc, char **argv)
{
	if (initGraphics() != 0)
		return 1;

	FTGLPixmapFont font("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
	if (font.Error())
	{
		std::cerr << "Error opening font" << std::endl;
		return 1;
	}
	font.FaceSize(12);

	GLuint shaderProgram = makeShaderProgram();

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
	const GLint uniformBlockLoc = glGetUniformLocation(shaderProgram, "blockOffset");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// cull back faces
	glEnable(GL_CULL_FACE);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	unsigned char *chunk0 = new unsigned char[4096];
	fillChunk(chunk0);
	Chunk centerChunk(chunk0, 0, 0, 0);

	Controls ctl;
	bool running = true;
	while (running)
	{
		ctl.beginFrame(&camera);

		// build MVP matrix
		glLoadMatrixf(glm::value_ptr(camera.projection()));
		glMultMatrixf(glm::value_ptr(camera.view()));

		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		centerChunk.render(uniformBlockLoc);
		font.Render("Hello world", -1, FTPoint(500, 300));

		// swap double buffers
		glfwSwapBuffers();

		// keep running until the user hits ESC or closes the main window
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// cleanup
	glfwTerminate();
	delete[] chunk0;
	return 0;
}

