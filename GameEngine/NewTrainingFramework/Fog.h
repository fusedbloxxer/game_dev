#pragma once
#include "..\Utilities\utilities.h"

class Fog
{
	Vector3 fogColor;
	GLfloat fogClarityRadius;
	GLfloat fogTransitionRadius;

public:
	Fog(const Vector3& fogColor = { 0.0f, 0.0f, 0.0f }, GLfloat fogClarityRadius = 0.0f, GLfloat fogTransitionRadius = 0.0f);

	Vector3 getFogColor() const;
	GLfloat getFogClarityRadius() const;
	GLfloat getFogTransitionRadius() const;

	void setFogColor(const Vector3& color);
	void setFogClarityRadius(const GLfloat clarity);
	void setFogTransitionRadius(const GLfloat transition);
};
