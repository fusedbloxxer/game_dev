#include "stdafx.h"
#include "LineTrajectory.h"

LineTrajectory::LineTrajectory(const char* type, GLint itCount, GLfloat speed, GLboolean direction)
	:Trajectory{ type, itCount, speed, direction }
{
}

LineTrajectory::LineTrajectory(Type type, GLint itCount, GLfloat speed, GLboolean direction)
	: Trajectory{ type, itCount, speed, direction }
{
}

void LineTrajectory::setPoints(const std::vector<Vector3>& points)
{
	this->points = points;
}

std::vector<Vector3>& LineTrajectory::getPoints()
{
	return points;
}
