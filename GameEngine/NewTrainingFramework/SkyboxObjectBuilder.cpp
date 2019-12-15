#include "stdafx.h"
#include "SkyboxObjectBuilder.h"

SkyboxObjectBuilder::SkyboxObjectBuilder(SceneObject::Type type, GLint id)
	:SceneObjectBuilder{ type, id } {}

SkyboxObject* SkyboxObjectBuilder::build() const
{
	SkyboxObject* object = new SkyboxObject(id, type);

	SceneObjectBuilder::sendCommonProperties(object);

	return object;
}
