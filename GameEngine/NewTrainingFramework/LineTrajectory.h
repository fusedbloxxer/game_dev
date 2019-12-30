#pragma once
#include "Trajectory.h"
#include <vector>

class LineTrajectory : public virtual Trajectory
{
protected:
	// Intermediary positions
	std::vector<Vector3> points;

	// Normalized direction vector
	Vector3 vecDirection;

	// Distante to be travelled up to point
	GLfloat distance, rotationSpeed = 20, rotationValue;

	// Current portion index
	GLint pos;

	// Forward / Backward movement
	GLboolean alternateDirection, reset, rotating, first;

	// Forward / Backward - (1) or (-1)
	GLint orientation;

	// Compute step
	Vector3 calculateStep(GLfloat time);

	void calculateTravelProps(SceneObject* obj, GLfloat time);

	void setRotation();

	virtual void print(std::ostream& os) const override;

	inline static constexpr GLboolean DEFAULT_DIRECTION = false;

public:
	// Constructors
	LineTrajectory(const char* type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION, const std::vector<Vector3>& points = {});
	LineTrajectory(Type type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION, const std::vector<Vector3>& points = {});

	// Rotate when touching and end-point.
	virtual void rotate(SceneObject *obj, GLfloat time); // TOOD;

	virtual void move(SceneObject* obj, GLfloat time) override;

	// Getters and setters
	virtual void setPoints(const std::vector<Vector3>& points);
	std::vector<Vector3>& getPoints();

	// Virtual destructor
	virtual ~LineTrajectory() = default;

	// Getters and setters
	GLboolean isAlternate() const;
	void setDirection(GLboolean direction);
	void setDirection(const char* direction);

	GLfloat getRotationSpeed() const;
	void setRotationSpeed(GLfloat speed);
};