#include "chunk.h"
#include "controls.h"
#include "camera.h"
#include "hud.h"
#include "shader.h"
#include "utils.h"
#include <FTGL/ftgl.h>
#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

std::ostream& operator<<(std::ostream &os, const glm::vec3 &v)
{
	return os << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')';
}

/// populate a chunk - should probably take a random seed
void fillChunk(unsigned char *blockIds)
{
	for(unsigned i = 0; i < 4096; ++i)
		blockIds[i] = 0;

	blockIds[Chunk::index(0, 0, 0)] = 1;
	blockIds[Chunk::index(0, 0, 1)] = 1;
}

int main(int argc, char **argv)
{
	if (initGraphics() != 0)
		return 1;

	FTGLPixmapFont font("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
	if (font.Error())
	{
		std::cerr << "Error opening font" << std::endl;
		return 2;
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// cull back faces
	glEnable(GL_CULL_FACE);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// fixed perspective
	glLoadMatrixf(glm::value_ptr(glm::perspective(
	  90.f, // fov
	  1.f, // aspect ratio
	  .1f, // near clip
	  100.f // far clip
	)));

	// fixed view
	glMultMatrixf(glm::value_ptr(glm::lookAt(
	  glm::vec3(.5, .5, .5), // position
	  glm::vec3(.5, .5, -1), // target xyz
	  glm::vec3(0, 1, 0) // headVec
	)));

	unsigned char *chunk0 = new unsigned char[4096];
	fillChunk(chunk0);

	const GLint uniformCameraDirection = glGetUniformLocation(shaderProgram, "cameraDirection");

	Camera camera;
	Controls ctl;
	Hud hud;
	const size_t varFPS = hud.addVarLine("FPS: ", "0");
	const size_t varPos = hud.addVarLine("Position: ", "-");
	const size_t varTgt = hud.addVarLine("Target: ", "-");

	bool running = true;
	while (running)
	{
		// feed camera info to shader
		ctl.beginFrame(&camera);

		const glm::vec3 cd = camera.direction() - camera.position();

		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform3f(uniformCameraDirection, cd.x, cd.y, cd.z);

		glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(0, 1, 0);
		glEnd();

		glUseProgram(0);

		// update HUD variables
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

