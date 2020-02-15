#pragma once
#include "SceneObject.h"

class SkyboxObject : public SceneObject
{
public:
	// Constructors
	SkyboxObject(GLint id);

	SkyboxObject(GLint id, Type type);

	virtual ~SkyboxObject();
};