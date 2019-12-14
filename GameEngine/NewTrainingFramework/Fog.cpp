#include "stdafx.h"
#include "Fog.h"


Fog::Fog(const Vector3& fogColor, GLfloat fogClarityRadius, GLfloat fogTransitionRadius)
	:fogColor{ fogColor }, fogClarityRadius{ fogClarityRadius }, fogTransitionRadius{ fogTransitionRadius }
{
}

Vector3 Fog::getFogColor() const
{
	return fogColor;
}

GLfloat Fog::getFogClarityRadius() const
{
	return fogClarityRadius;
}

GLfloat Fog::getFogTransitionRadius() const
{
	return fogTransitionRadius;
}

void Fog::setFogColor(const Vector3& color)
{
	this->fogColor = color;
}

void Fog::setFogClarityRadius(const GLfloat clarity)
{
	this->fogClarityRadius = clarity;
}

void Fog::setFogTransitionRadius(const GLfloat transition)
{
	this->fogTransitionRadius = transition;
}
