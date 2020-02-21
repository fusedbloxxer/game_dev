#include "stdafx.h"
#include "SceneManager.h"
#include "SkyboxObject.h"

SkyboxObject::SkyboxObject(GLint id)
	:SkyboxObject{ id, Type::SKYBOX }
{
}

SkyboxObject::SkyboxObject(GLint id, Type type)
	:SceneObject{ id, type }
{
	disableCollision();
}

SkyboxObject::~SkyboxObject()
{
	// TODO;
}
