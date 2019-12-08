#pragma once
#include "..\Utilities\utilities.h"

class Fog
{
	Vector3 fogColor;
	GLfloat fogClarityRadius;
	GLfloat fogTransitionRadius;

public:
	Vector3 getFogColor() const;
	GLfloat getFogClarityRadius() const;
	GLfloat getFogTransitionRadius() const;

	void setFogColor(const Vector3& color);
	void setFogClarityRadius(const GLfloat clarity);
	void setFogTransitionRadius(const GLfloat transition);
};
