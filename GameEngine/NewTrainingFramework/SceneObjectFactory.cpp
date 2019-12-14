#include "stdafx.h"
#include "SceneObjectFactory.h"
#include "TerrainObject.h"
#include "SkyboxObject.h"
#include "FireObject.h"

class SceneObject;

SceneObject* SceneObjectFactory::newInstance(SceneObject::Type type, GLint objId)
{
	switch (type)
	{
	case SceneObject::Type::NORMAL:
		return new SceneObject(objId);
	case SceneObject::Type::TERRAIN:
		return new TerrainObject(objId);
	case SceneObject::Type::SKYBOX:
		return new SkyboxObject(objId);
	case SceneObject::Type::FIRE:
		return new FireObject(objId);
	default:
		throw std::runtime_error{ "SceneObject::Type::..... not listed here." };
	}
}
