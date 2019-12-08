#include "stdafx.h"
#include "Fog.h"


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
