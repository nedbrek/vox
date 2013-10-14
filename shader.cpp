#include <GL/glew.h>
#include "shader.h"
#include "utils.h"
#include <iostream>

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
	  "vertOffset.glsl");
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

