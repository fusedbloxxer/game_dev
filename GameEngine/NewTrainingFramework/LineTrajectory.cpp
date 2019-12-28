#include "stdafx.h"
#include "LineTrajectory.h"

LineTrajectory::LineTrajectory(const char* type, GLint itCount, GLfloat speed, const std::vector<Vector3>& points)
	:Trajectory{ type, itCount, speed }
{
	setPoints(points);
}

LineTrajectory::LineTrajectory(Type type, GLint itCount, GLfloat speed, const std::vector<Vector3>& points)
	: Trajectory{ type, itCount, speed }
{
	setPoints(points);
}

void LineTrajectory::setPoints(const std::vector<Vector3>& points)
{
	this->points = points;

	if (type == Trajectory::Type::LINE_LOOP && this->points.size() > 0)
	{
		this->points.push_back(this->points.at(0));
	}
}

std::vector<Vector3>& LineTrajectory::getPoints()
{
	return points;
}