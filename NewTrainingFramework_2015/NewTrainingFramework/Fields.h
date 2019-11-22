#pragma once
#include "..\Utilities\utilities.h"

struct Fields {
	// Attributes
	GLint isTextureAttribute;
	GLint positionAttribute;
	GLint binormAttribute;
	GLint colorAttribute;
	GLint normAttribute;
	GLint tgtAttribute;
	GLint uvAttribute;

	// Uniforms
	GLint textureUniform;
	GLint unifMatrix;
};