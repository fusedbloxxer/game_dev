#pragma once
#include "VaryingTrajectory.h"
#include "LineTrajectory.h"

class LinearTrajectory : public VaryingTrajectory, public LineTrajectory
{
public:
	LinearTrajectory(const char* type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION, const Vector3& start = { 0.0f, 0.0f, 0.0f }, const Vector3& end = { 0.0f, 0.0f, 0.0f });
	LinearTrajectory(Type type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION, const Vector3& start = { 0.0f, 0.0f, 0.0f }, const Vector3& end = { 0.0f, 0.0f, 0.0f });

	// Inherited via VaryingTrajectory
	virtual void rotate(SceneObject* object, GLfloat time) override;

	// Inherited via LineTrajectory
	virtual void move(SceneObject* object, GLfloat time) override;

	// Virtual destructor
	virtual ~LinearTrajectory() = default;
};