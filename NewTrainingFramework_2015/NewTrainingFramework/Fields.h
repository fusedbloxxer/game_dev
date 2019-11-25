#pragma once
#include "..\Utilities\utilities.h"

struct Fields {
	// The maximum number of available textures
	static constexpr GLuint MAX_TEXTURES = 5;

	GLint positionAttribute;
	GLint binormAttribute;
	GLint colorAttribute;
	GLint normAttribute;
	GLint tgtAttribute;
	GLint uv2Attribute;
	GLint uvAttribute;

	// Uniforms
	GLint textureUniform[MAX_TEXTURES];
	GLint heightUniform;
	GLint unifMatrix;
};