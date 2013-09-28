#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>

unsigned char* loadPngTexture(const char *filename, int *width, int *height,
  bool *hasAlpha);

namespace
{
	const float mouseSpeed = 0.005;
	const float speed = 3; // units per second

	unsigned char* makeColorTexture(unsigned numPixels)
	{
		assert(numPixels % 4 == 0);
		unsigned char *buf= new unsigned char[numPixels];
		for(unsigned i = 0; i < numPixels; ++i)
		{
			buf[i] = 255; ++i; // red
			buf[i] = 250; ++i; // green
			buf[i] =   0; ++i; // blue
			buf[i] = 127; // alpha
		}
		return buf;
	}

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

	GLuint compileShader(GLenum type, const char *filename)
	{
		// allocate program resource
		GLuint shaderId = glCreateShader(type);

		// pull in source code
		std::string fileString = fileToString(filename);

		// compile it
		const char *source = fileString.c_str();
		glShaderSource(shaderId, 1, &source, NULL);
		glCompileShader(shaderId);

		// check results
		GLint result = GL_FALSE;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
		if (result != GL_TRUE)
		{
			int infoLogLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

			std::string infoLog(infoLogLength+1, 0);
			glGetShaderInfoLog(shaderId, infoLogLength, NULL, &infoLog[0]);
			std::cerr << "Shader " << std::string(filename)
			  << " compilation failed." << std::endl
			  << infoLog.c_str() << std::endl;
			return 0; // invalid id
		}
		return shaderId;
	}

	GLuint makeShaderProgram()
	{
		// allocate program resource
		// vertex shader
		GLuint vertexShaderId = compileShader(GL_VERTEX_SHADER,
		  "vertBasic.glsl");
		if (vertexShaderId == 0)
			return 0;

		// fragment shader
		GLuint fragmentShaderId = compileShader(GL_FRAGMENT_SHADER,
		  "fragCube.glsl");
		if (fragmentShaderId == 0)
			return 0;

		// link the program
		GLuint programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);

		// check results
		GLint result = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			int infoLogLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::cerr << "Shader Program link failed." << std::endl;
			return 0; // invalid id
		}

		glDeleteShader(fragmentShaderId);
		return programId;
	}
}

// wrapper for matrix operations related to the view
class Camera
{
public:
	Camera()
	: x_(-1)
	, y_(-1)
	, z_(2)
	, tX_(0)
	, tY_(0)
	, tZ_(0)
	, headVec_(0, 1, 0)
	, fov_(90)
	, aspectRatio_(16./9)
	, nearClipPlane_(.1)
	, farClipPlane_(100)
	{
	}

	glm::mat4 projection() const
	{
		return glm::perspective(fov_, aspectRatio_,
		  nearClipPlane_, farClipPlane_);
	}

	glm::mat4 view() const
	{
		return glm::lookAt(
		  position(),
		  glm::vec3(tX_, tY_, tZ_), // target xyz
		  headVec_
		);
	}

	glm::vec3 position() const
	{
		return glm::vec3(x_, y_, z_);
	}

	void setPosition(const glm::vec3 &pos)
	{
		x_ = pos[0];
		y_ = pos[1];
		z_ = pos[2];
	}

	void setTargetPosition(const glm::vec3 &v)
	{
		tX_ = v[0];
		tY_ = v[1];
		tZ_ = v[2];
	}

	void setHeadVec(const glm::vec3 &v)
	{
		headVec_ = v;
	}

protected:
	// pointing angle
	float x_, y_, z_; // position
	float tX_, tY_, tZ_; // target position

	glm::vec3 headVec_; // head tilt

	// projection state
	float fov_; // field of view

	float aspectRatio_;
	float nearClipPlane_, farClipPlane_;
};

class Controls
{
public:
	Controls()
	: lastTime_(glfwGetTime())
	, windowWidth_(1092)
	, windowHeight_(614)
	{
		glfwDisable(GLFW_MOUSE_CURSOR);
	}

	void beginFrame(Camera *cp)
	{
		const double curTime = glfwGetTime();
		const double deltaT = curTime - lastTime_;

		// find amount of mouse motion since last frame
		int mouseX, mouseY;
		glfwGetMousePos(&mouseX, &mouseY);

		// compute new orientation
		const float deltaX = float(windowWidth_/2 - mouseX);
		const float deltaY = float(windowHeight_/2 - mouseY);

		const float horizontalAngle = 3.14f + mouseSpeed * deltaX;
		const float verticalAngle   = mouseSpeed * deltaY;

		// direction : spherical to cartesian coordinate conversion
		const glm::vec3 direction(
		  cos(verticalAngle) * sin(horizontalAngle),
		  sin(verticalAngle),
		  cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		const glm::vec3 right = glm::vec3(
		  sin(horizontalAngle - 3.14f/2.0f),
		  0,
		  cos(horizontalAngle - 3.14f/2.0f)
		);

		// Up vector
		const glm::vec3 up = glm::cross(right, direction);

		glm::vec3 position = cp->position();
		// update position
		if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
		{
			position[1] += deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			position[1] -= deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			position[0] += deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			position[0] -= deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_PAGEUP) == GLFW_PRESS)
		{
			position[2] += deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_PAGEDOWN) == GLFW_PRESS)
		{
			position[2] -= deltaT * speed;
		}

		cp->setPosition(position);
		cp->setTargetPosition(position + direction);
		cp->setHeadVec(up);

		lastTime_ = curTime;
	}

protected:
	double lastTime_;
	int windowWidth_, windowHeight_;
};

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

void drawSceneQuad()
{
	glBegin(GL_QUADS);

	// bottom
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glVertex3d(1, 1, 0);
	glVertex3d(0, 1, 0);

	// top
	glVertex3d(0, 0, 1);
	glVertex3d(1, 0, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(0, 1, 1);

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

