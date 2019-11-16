#pragma once
#include "../Utilities/utilities.h"

class ModelShader
{
public:
	GLuint program, vertexShader, fragmentShader;
	
	// Attributes
	GLint positionAttribute;
	GLint binormAttribute;
	GLint normAttribute;
	GLint tgtAttribute;
	GLint uvAttribute;

	// Uniforms
	GLint unifMatrix;
	GLint textureUniform;

	int init(char* fileVertexShader, char* fileFragmentShader);
	~ModelShader();
};

