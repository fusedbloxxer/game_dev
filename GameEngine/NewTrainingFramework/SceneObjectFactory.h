#pragma once
#include "SceneObject.h"

class SceneObjectFactory
{
	SceneObjectFactory() = default;
public:
	static SceneObject* newInstance(SceneObject::Type type, GLint objId);
	// static SceneObject* newInstanceWithParams(SceneObject::Type type, GLint objId, )
};