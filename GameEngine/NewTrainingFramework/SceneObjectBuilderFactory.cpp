#include "stdafx.h"
#include "SceneObjectBuilderFactory.h"
#include "TerrainObjectBuilder.h"
#include "SkyboxObjectBuilder.h"
#include "FireObjectBuilder.h"

SceneObjectBuilder* SceneObjectBuilderFactory::newBuilderInstance(SceneObject::Type type, GLint id)
{
	switch (type)
	{
	case SceneObject::Type::NORMAL:
	{
		return new SceneObjectBuilder(type, id);
	}
	case SceneObject::Type::SKYBOX:
	{
		return new SkyboxObjectBuilder(type, id);
	}
	case SceneObject::Type::TERRAIN:
	{
		return new TerrainObjectBuilder(type, id);
	}
	case SceneObject::Type::FIRE:
	{
		return new FireObjectBuilder(type, id);
	}
	default:
		throw std::runtime_error{ "SceneObject::Type::..... not listed here." };
	}
}