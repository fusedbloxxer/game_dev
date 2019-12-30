#pragma once
#include "Trajectory.h"

class CircleTrajectory : public Trajectory
{
	Vector3 center;

	GLboolean first;

	Matrix rotationMatrix;
	
	GLfloat radius, angle;

	Vector3 rotationPlane, initial;

protected:
	virtual void print(std::ostream& os) const override;

public:
	// Constructors
	CircleTrajectory(const char* type, GLfloat speed = DEFAULT_SPEED, GLint itCount = DEFAULT_IT_COUNT);
	CircleTrajectory(Type type, GLfloat speed = DEFAULT_SPEED, GLint itCount = DEFAULT_IT_COUNT);

	virtual void move(SceneObject* obj, GLfloat time) override;

	void updateObject(SceneObject* obj, const GLfloat& step);

	// Virtual destructor
	virtual ~CircleTrajectory() = default;

	// Getters and setters
	const Vector3& getCenter() const;
	void setCenter(const Vector3& center);

	Vector3 getRotationPlane() const;
	void setRotationPlane(const Vector3& rotationPlane);

	GLfloat getRadius() const;
	void setRadius(GLfloat radius);
};
