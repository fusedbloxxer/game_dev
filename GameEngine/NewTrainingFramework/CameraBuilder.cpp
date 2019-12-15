#include "stdafx.h"
#include "CameraBuilder.h"

CameraBuilder::CameraBuilder(GLint id, GLfloat width, GLfloat height)
	:id{ id }, width{ width }, height{ height } {}

Camera* CameraBuilder::build() const
{
	return new Camera(width, height, id, position, target, up, moveSpeed, rotateSpeed, nearP, farP, fov, deltaTime, type);
}

CameraBuilder& CameraBuilder::setUp(const Vector3& up)
{
	this->up = up;
	return *this;
}

CameraBuilder& CameraBuilder::setFar(GLfloat farP)
{
	this->farP = farP;
	return *this;
}

CameraBuilder& CameraBuilder::setFov(GLfloat fov)
{
	this->fov = fov;
	return *this;
}

CameraBuilder& CameraBuilder::setNear(GLfloat nearP)
{
	this->nearP = nearP;
	return *this;
}

CameraBuilder& CameraBuilder::setTarget(const Vector3& target)
{
	this->target = target;
	return *this;
}

CameraBuilder& CameraBuilder::setDeltaTime(GLfloat deltaTime)
{
	this->deltaTime = deltaTime;
	return *this;
}

CameraBuilder& CameraBuilder::setMoveSpeed(GLfloat moveSpeed)
{
	this->moveSpeed = moveSpeed;
	return *this;
}

CameraBuilder& CameraBuilder::setPosition(const Vector3& position)
{
	this->position = position;
	return *this;
}

CameraBuilder& CameraBuilder::setRotateSpeed(GLfloat rotateSpeed)
{
	this->rotateSpeed = rotateSpeed;
	return *this;
}

CameraBuilder& CameraBuilder::setType(Camera::Type type)
{
	this->type = type;
	return *this;
}