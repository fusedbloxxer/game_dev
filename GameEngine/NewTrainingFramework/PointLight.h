#pragma once
#include "Light.h"

class PointLight : public Light
{
	GLint id, associatedObject;

	Vector3 position, diffuseColor, specularColor;
public:

	virtual ~PointLight() = default;
};