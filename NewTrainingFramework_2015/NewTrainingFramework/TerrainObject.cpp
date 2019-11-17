#include "stdafx.h"
#include "TerrainObject.h"

TerrainObject::TerrainObject(GLint id)
	:TerrainObject{ id, Type::TERRAIN }
{
}

TerrainObject::TerrainObject(GLint id, Type type)
	: SceneObject{ id, type }
{
}

TerrainObject::~TerrainObject()
{
	// TODO;
}
