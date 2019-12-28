#include "stdafx.h"
#include "CircleTrajectory.h"

CircleTrajectory::CircleTrajectory(const char* type, GLfloat speed, GLint itCount)
	:Trajectory{ type, speed, itCount} {}

CircleTrajectory::CircleTrajectory(Type type, GLfloat speed, GLint itCount)
	: Trajectory{ type, speed, itCount} {}

void CircleTrajectory::move(SceneObject* object, GLfloat time)
{
	// TODO;
}
