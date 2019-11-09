#include "stdafx.h"
#include "ModelShader.h"

int ModelShader::Init(char* fileVertexShader, char* fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	
	if (vertexShader == 0)
	{
		return -1;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

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

	// Bind C++ with OpenGL
	// Attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	binormAttribute = glGetAttribLocation(program, "a_binormL");
	normAttribute = glGetAttribLocation(program, "a_normL");
	tgtAttribute = glGetAttribLocation(program, "a_tgtL");
	uvAttribute = glGetAttribLocation(program, "a_uvL");

	// Uniforms
	unifMatrix = glGetUniformLocation(program, "u_matrix");
	textureUniform = glGetUniformLocation(program, "u_texture");

	return 0;
}

ModelShader::~ModelShader()
{
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(program);
}
