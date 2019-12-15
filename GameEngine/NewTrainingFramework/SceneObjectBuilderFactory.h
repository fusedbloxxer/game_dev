#pragma once
#include "SceneObjectBuilder.h"

class SceneObjectBuilderFactory
{
	SceneObjectBuilderFactory() = default;
public:
	static SceneObjectBuilder* newBuilderInstance(SceneObject::Type type, GLint id);
	virtual ~SceneObjectBuilderFactory() = default;
};