#include "chunk.h"
#include "controls.h"
#include "camera.h"
#include "hud.h"
#include "resources.h"
#include "utils.h"
#include <GL/glfw.h>
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

	for(unsigned x = 0; x < 16; ++x)
	for(unsigned z = 0; z < 16; ++z)
		blockIds[Chunk::index(x, 1, z)] = 2;
}

int main(int argc, char **argv)
{
	if (initGraphics() != 0)
		return 1;

	const int BOT = -1;
	const int TOP = 3;
	unsigned char *chunkData[63][63];
	Chunk *chunks[63][63];
	for(int x = BOT; x < TOP; ++x)
		for(int z = BOT; z < TOP; ++z)
		{
			unsigned char *chunk0 = new unsigned char[4096];
			chunkData[x+31][z+31] = chunk0;
			fillChunk(chunk0);
			chunks[x+31][z+31] = new Chunk(chunk0, x, 0, z);
		}

	Resources res;
	Camera camera;

	Hud hud;
	const size_t varFPS = hud.addVarLine("FPS: ", "0");
	const size_t varPos = hud.addVarLine("Position: ", "-");
	const size_t varTgt = hud.addVarLine("Target: ", "-");
	//const size_t varMous = hud.addVarLine("Mouse: ", "-");

	Controls ctl;
	bool running = true;
	while (running)
	{
		ctl.beginFrame(&camera);

		for(int x = BOT; x < TOP; ++x)
			for(int z = BOT; z < TOP; ++z)
				chunks[x+31][z+31]->render(res);

		//hud.updateVarLine(varMous, ctl.lastMouseDXY());
		hud.updateVarLine(varFPS, ctl.fps());
		hud.updateVarLine(varPos, camera.position());
		hud.updateVarLine(varTgt, camera.targetPosition());

		hud.render(res.font());

		// swap double buffers
		glfwSwapBuffers();

		// keep running until the user hits ESC or closes the main window
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// cleanup
	glfwTerminate();
	for(int x = BOT; x < TOP; ++x)
		for(int z = BOT; z < TOP; ++z)
		{
			delete[] chunkData[x+31][z+31];
			delete chunks[x+31][z+31];
		}

	return 0;
}

