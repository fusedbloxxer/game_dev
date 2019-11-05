#include "stdafx.h"
#include "Shaders1.h"

GLint Shaders1::Init(char * vertexShaderPath, char * fragmentShaderPath)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, vertexShaderPath);

	if (vertexShader == 0)
	{
		return -1;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	if (program == 0)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return -3;
	}

	// Binding C++ code with OpenGL ES 2.0
	positionAttribute = glGetAttribLocation(program, "a_posL");

	return 0;
}

Shaders1::~Shaders1()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
