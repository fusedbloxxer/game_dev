#include "stdafx.h"
#include "CircleTrajectory.h"

void CircleTrajectory::print(std::ostream& os) const
{
	Trajectory::print(os);
	os << " center: " << center << " radius: " << radius << " rotationPlane: " << rotationPlane;
}

CircleTrajectory::CircleTrajectory(const char* type, GLfloat speed, GLint itCount)
	:Trajectory{ type, speed, itCount} {}

CircleTrajectory::CircleTrajectory(Type type, GLfloat speed, GLint itCount)
	: Trajectory{ type, speed, itCount} {}

void CircleTrajectory::move(SceneObject* obj, GLfloat time)
{
	// TODO;
}

const Vector3& CircleTrajectory::getCenter() const
{
	return center;
}

void CircleTrajectory::setCenter(const Vector3& center)
{
	this->center = center;
}

const Vector3& CircleTrajectory::getRotationPlane() const
{
	return rotationPlane;
}

void CircleTrajectory::setRotationPlane(const Vector3& rotationPlane)
{
	this->rotationPlane = rotationPlane;
}

GLfloat CircleTrajectory::getRadius() const
{
	return radius;
}

void CircleTrajectory::setRadius(GLfloat radius)
{
	this->radius = radius;
}
