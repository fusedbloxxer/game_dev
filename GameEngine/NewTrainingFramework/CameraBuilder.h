#pragma once
#include "IBuilder.h"
#include "Camera.h"

class CameraBuilder final : public IBuilder<Camera>
{
	GLint id;

	Camera::Type type;

	GLfloat width, height;

	Vector3 target, position, up;

	GLfloat moveSpeed, rotateSpeed, nearP, farP, fov, deltaTime;

public:
	// Constructor
	CameraBuilder(GLint id, GLfloat width, GLfloat height);

	// Inherited via IBuilder
	virtual Camera* build() const override;

	// Setters
	CameraBuilder& setFar(GLfloat);
	CameraBuilder& setFov(GLfloat);
	CameraBuilder& setNear(GLfloat);
	CameraBuilder& setDeltaTime(GLfloat);
	CameraBuilder& setMoveSpeed(GLfloat);
	CameraBuilder& setRotateSpeed(GLfloat);
	CameraBuilder& setUp(const Vector3& up);
	CameraBuilder& setTarget(const Vector3&);
	CameraBuilder& setType(Camera::Type type);
	CameraBuilder& setPosition(const Vector3&);

	// Virtual destructor
	virtual ~CameraBuilder() = default;
};