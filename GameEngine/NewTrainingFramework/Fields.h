#pragma once
#include "..\Utilities\utilities.h"

struct Fields {
	// The maximum number of available textures
	static constexpr GLuint MAX_LIGHT_SOURCES = 8;
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
	GLint isReflectedUniform;
	GLint heightUniform;
	GLint cameraUniform;

	// For fog effect
	GLint fogTransitionUniform;
	GLint fogClarityUniform;
	GLint fogColorUniform;

	// For reflection effect
	GLint skyboxUniform;

	// Light effects
	struct Light
	{
		GLint lightDirectionUniform;
		GLint specularColorUniform;
		GLint specularPowerUniform;
		GLint diffuseColorUniform;
	}lights[MAX_LIGHT_SOURCES];

	GLint ambientalRatioUniform;
	GLint ambientalLightUniform;

	// MVP
	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;

	// For fire effect
	GLint dispMaxUniform;
	GLint timeUniform;
};