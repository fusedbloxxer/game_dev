#pragma once
#include "Trajectory.h"

class CircleTrajectory : public Trajectory
{
	Vector3 center;

public:
	// Constructors
	CircleTrajectory(const char* type, GLfloat speed = DEFAULT_SPEED, GLint itCount = DEFAULT_IT_COUNT);
	CircleTrajectory(Type type, GLfloat speed = DEFAULT_SPEED, GLint itCount = DEFAULT_IT_COUNT);

	virtual void move(SceneObject* object, GLfloat time) override;

	// Virtual destructor
	virtual ~CircleTrajectory() = default;

	// Getters and setters
};
