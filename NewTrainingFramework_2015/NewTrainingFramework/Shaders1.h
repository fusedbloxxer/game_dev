#pragma once
#include "../Utilities/utilities.h"

class Shaders1
{
public:
	GLuint program, vertexShader, fragmentShader;
	GLint positionAttribute;

	GLint Init(char * vertexShaderPath, char * fragmentShaderPath);
	~Shaders1();
};

