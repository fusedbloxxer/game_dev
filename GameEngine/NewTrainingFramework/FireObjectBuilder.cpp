#include "stdafx.h"
#include "FireObjectBuilder.h"

FireObjectBuilder::FireObjectBuilder(SceneObject::Type type, GLint id)
	:SceneObjectBuilder{ type, id } {}

FireObject* FireObjectBuilder::build() const
{
	FireObject* object = new FireObject(id, type);

	SceneObjectBuilder::sendCommonProperties(object);

	// Send specific data
	object->setDispMax(dispMax);

	return object;
}

FireObjectBuilder& FireObjectBuilder::setDispMax(GLfloat dispMax)
{
	this->dispMax = dispMax;
	return *this;
}
