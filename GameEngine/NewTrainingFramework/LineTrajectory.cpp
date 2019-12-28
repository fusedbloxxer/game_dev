#include "stdafx.h"
#include "LineTrajectory.h"

LineTrajectory::LineTrajectory(const char* type, GLint itCount, GLfloat speed, const std::vector<Vector3>& points)
	:Trajectory{ type, itCount, speed }, distance{ 0.0f }, pos{ 0 }, vecDirection{ 0.0f, 0.0f, 0.0f, 0.0f }
{
	setPoints(points);
}

LineTrajectory::LineTrajectory(Type type, GLint itCount, GLfloat speed, const std::vector<Vector3>& points)
	: Trajectory{ type, itCount, speed }, distance{ 0.0f }, pos{ 0 }, vecDirection{ 0.0f, 0.0f, 0.0f, 0.0f }
{
	setPoints(points);
}

void LineTrajectory::calculateTravelProps(const GLint orientation)
{
	// Calculate vector between two points.
	if (orientation == 1) 
	{
		vecDirection = Vector4(points[pos] - points[pos + orientation], 1.0f);
	}
	else if (orientation == -1) 
	{
		vecDirection = Vector4(points[pos + orientation] - points[pos], 1.0f);
	}
	else 
	{
		throw std::runtime_error{ "Bad orientation." };
	}

	// Calculate the distance to be travelled.
	distance = vecDirection.Length();

	// Normalize the directon vector.
	vecDirection.Normalize();
}

Vector4 LineTrajectory::calculateStep(GLfloat time)
{
	return vecDirection * time * speed;
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