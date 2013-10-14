#include "chunk.h"
#include "controls.h"
#include "camera.h"
#include "hud.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"
#include <FTGL/ftgl.h>
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <sstream>

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v)
{
	return os << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')';
}

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

	Texture stone("textures/blocks/stone.png");

	Camera camera;
	// track key down events until we get around to them
	glfwEnable(GLFW_STICKY_KEYS);

	glUseProgram(shaderProgram);
	const GLint uniformBlockLoc = glGetUniformLocation(shaderProgram, "blockOffset");

	glActiveTexture(GL_TEXTURE0);
	stone.makeActive();

	// cull back faces
	glEnable(GL_CULL_FACE);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	unsigned char *chunk0 = new unsigned char[4096];
	fillChunk(chunk0);
	Chunk centerChunk(chunk0, 0, 0, 0);

	Hud hud;
	const size_t varFPS = hud.addVarLine("FPS: ", "0");
	const size_t varPos = hud.addVarLine("Position: ", "-");
	const size_t varTgt = hud.addVarLine("Target: ", "-");
	const size_t varMous = hud.addVarLine("Mouse: ", "-");
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

		hud.updateVarLine(varMous, ctl.lastMouseDXY());
		hud.updateVarLine(varFPS, ctl.fps());
		hud.updateVarLine(varPos, camera.position());
		hud.updateVarLine(varTgt, camera.targetPosition());

		hud.render(font);

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

