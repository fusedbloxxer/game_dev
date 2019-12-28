#pragma once
#include "SceneObjectBuilder.h"

class SceneObjectBuilderFactory final
{
	SceneObjectBuilderFactory() = default;

public:
	static SceneObjectBuilder* newBuilderInstance(SceneObject::Type type, GLint id);
};