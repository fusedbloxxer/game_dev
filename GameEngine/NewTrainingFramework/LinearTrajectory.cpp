#include "stdafx.h"
#include "LinearTrajectory.h"

LinearTrajectory::LinearTrajectory(const char* type, GLint itCount, GLfloat speed, GLboolean direction, const Vector3& start, const Vector3& end)
	:VaryingTrajectory{ type, itCount, speed, direction }, LineTrajectory{ type, itCount, speed, {start, end} }, Trajectory{ type, itCount, speed }
{
}

LinearTrajectory::LinearTrajectory(Type type, GLint itCount, GLfloat speed, GLboolean direction, const Vector3& start, const Vector3& end)
	: VaryingTrajectory{ type, itCount, speed, direction }, LineTrajectory{ type, itCount, speed, {start, end} }, Trajectory{ type, itCount, speed }
{
}

void LinearTrajectory::rotate(SceneObject* object, GLfloat time)
{
	// TODO;
}

void LinearTrajectory::move(SceneObject* object, GLfloat time)
{

}
