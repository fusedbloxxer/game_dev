#pragma once
#include "..\Utilities\utilities.h"

struct Fields {
	// The maximum number of available textures
	static constexpr GLuint MAX_LIGHT_SOURCES = 8;

	// From 0 to MAX_TEXTURES - 1
	static constexpr GLuint MAX_TEXTURES = 5; 

	static constexpr GLuint NORMAL_MAP_TEXTURE = MAX_TEXTURES;

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
		GLint associatedObjectPositionUniform;
		GLint lightDirectionUniform;
		GLint specularColorUniform;
		GLint specularPowerUniform;
		GLint diffuseColorUniform;
		GLint innerCutoffUniform;
		GLint outercutoffUniform;
		GLint lightTypeUniform;
	}lights[MAX_LIGHT_SOURCES];

	GLint ambientalRatioUniform;
	GLint ambientalLightUniform;

	GLint kdifUniform;
	GLint kspecUniform;

	// Normal Mapping
	GLint normalMapUniform;
	GLint hasNormalMapUniform;

	// MVP
	GLint modelUniform;
	GLint viewUniform;
	GLint projectionUniform;

	// For fire effect
	GLint dispMaxUniform;
	GLint timeUniform;
};