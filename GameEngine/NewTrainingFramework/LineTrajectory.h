#pragma once
#include "Trajectory.h"
#include <vector>

class LineTrajectory : public Trajectory
{
	std::vector<Vector3> points;
public:
	// Constructors
	LineTrajectory(const char* type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION);
	LineTrajectory(Type type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION);

	// Virtual destructor
	virtual ~LineTrajectory() = default;

	// Getters and setters
	void setPoints(const std::vector<Vector3>& points);
	std::vector<Vector3>& getPoints();
};

