#pragma once
#include "VaryingTrajectory.h"
#include <vector>

class LineTrajectory : public virtual Trajectory
{
protected:
	// Intermediary positions
	std::vector<Vector3> points;

	// Normalized direction vector
	Vector4 vecDirection;

	// Distante to be travelled up to point
	GLfloat distance;

	// Current portion index
	GLint pos;

	void calculateTravelProps(const GLint orientation);

	Vector4 calculateStep(GLfloat time);

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