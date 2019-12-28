#pragma once
#include "Trajectory.h"

class VaryingTrajectory : public virtual Trajectory
{
public:
	inline static constexpr GLboolean DEFAULT_DIRECTION = false;

protected:
	// Forward / Backward movement
	GLboolean alternateDirection;

	// Forward / Backward - (1) or (-1)
	GLint orientation;

	// Constructors
	VaryingTrajectory(const char* type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION);
	VaryingTrajectory(Type type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION);

public:

	// Rotate when touching and end-point.
	virtual void rotate(SceneObject* object, GLfloat time) = 0;

	// Virtual destructor
	virtual ~VaryingTrajectory() = default;

	// Getters and setters
	GLboolean isAlternate() const;
	void setDirection(GLboolean direction);
};