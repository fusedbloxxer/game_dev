#pragma once
#include "SceneObject.h"

class TerrainObject : public SceneObject
{
public:
	// Constructors
	TerrainObject(GLint id);

	TerrainObject(GLint id, Type type);

	virtual ~TerrainObject();
};

