#include "stdafx.h"
#include "SkyboxObject.h"

SkyboxObject::SkyboxObject(GLint id)
	:SkyboxObject{ id, Type::SKYBOX }
{
}

SkyboxObject::SkyboxObject(GLint id, Type type)
	:SceneObject{ id, type }
{
}

SkyboxObject::~SkyboxObject()
{
	// TODO;
}
