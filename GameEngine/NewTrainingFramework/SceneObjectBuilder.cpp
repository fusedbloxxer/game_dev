#include "stdafx.h"
#include "SceneObjectBuilder.h"

SceneObjectBuilder::SceneObjectBuilder(SceneObject::Type type, GLint id)
	:type{ type }, id{ id } {}

void SceneObjectBuilder::sendCommonProperties(SceneObject* base) const
{
	base->setCollisionBoxColor(collisionBoxColor);
	base->setFollowingCamera(followingCamera);
	base->setWiredFormat(wiredFormat);
	base->setTrajectory(trajectory);
	base->setReflection(reflection);
	base->setNormalMap(normalMap);
	base->setRotation(rotation);
	base->setPosition(position);
	base->setTextures(textures);
	base->setShader(shader);
	base->setModel(model);
	base->setScale(scale);
	base->setColor(color);
	base->setKSpec(kspec);
	base->setKDif(kdif);
	base->setName(name);
}

SceneObject* SceneObjectBuilder::build() const
{
	// Create instance
	SceneObject* object = new SceneObject(id, type);
	
	// Set properties
	sendCommonProperties(object);
	
	return object;
}

SceneObjectBuilder& SceneObjectBuilder::setName(const std::string& name)
{
	this->name = name;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setRotation(const Vector3& rotation)
{
	this->rotation = rotation;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setPosition(const Vector3& position)
{
	this->position = position;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setReflection(GLboolean reflection)
{
	this->reflection = reflection;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setWiredFormat(GLboolean wiredFormat)
{
	this->wiredFormat = wiredFormat;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setModel(std::shared_ptr<Model> model)
{
	this->model = model;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setShader(std::shared_ptr<Shader> shader)
{
	this->shader = shader;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setFollowingCamera(const Vector3& followingCamera)
{
	this->followingCamera = followingCamera;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setCollisionBoxColor(const Vector3& collisionBoxColor)
{
	this->collisionBoxColor = collisionBoxColor;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setNormalMap(const std::shared_ptr<Texture>& normalMap)
{
	this->normalMap = normalMap;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setTrajectory(const std::shared_ptr<Trajectory> trajectory)
{
	this->trajectory = trajectory;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setTextures(const std::vector<std::shared_ptr<Texture>>& textures)
{
	this->textures = textures;
	return *this;
}

const Vector3& SceneObjectBuilder::getCollisionBoxColor() const
{
	return collisionBoxColor;
}

SceneObjectBuilder& SceneObjectBuilder::setKDif(GLfloat kdif)
{
	if (kdif < 0.0f || kdif > 1.0f)
	{
		throw std::runtime_error{ "Value cannot be lower than zero or higher than one." };
	}
	this->kdif = kdif;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setKSpec(GLfloat kspec)
{
	if (kspec < 0.0f || kspec > 1.0f)
	{
		throw std::runtime_error{ "Value cannot be lower than zero or higher than one." };
	}
	this->kspec = kspec;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setScale(const Vector3& scale)
{
	this->scale = scale;
	return *this;
}

SceneObjectBuilder& SceneObjectBuilder::setColor(const Vector3& color)
{
	this->color = color;
	return *this;
}