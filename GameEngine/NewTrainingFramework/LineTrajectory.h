#pragma once
#include "VaryingTrajectory.h"
#include <vector>

class LineTrajectory : public virtual Trajectory
{
	// Intermediary positions
	std::vector<Vector3> points;

public:
	// Constructors
	LineTrajectory(const char* type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, const std::vector<Vector3>& points = {});
	LineTrajectory(Type type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, const std::vector<Vector3>& points = {});

	virtual void move(SceneObject* object, GLfloat time) = 0;

	// Virtual destructor
	virtual ~LineTrajectory() = default;

	// Getters and setters
	void setPoints(const std::vector<Vector3>& points);
	std::vector<Vector3>& getPoints();
};