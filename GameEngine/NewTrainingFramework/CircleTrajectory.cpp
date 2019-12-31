#include "stdafx.h"
#include "CircleTrajectory.h"

void CircleTrajectory::print(std::ostream& os) const
{
	Trajectory::print(os);
	os << " center: " << center << " radius: " << radius << " rotationPlane: " << rotationPlane;
}

CircleTrajectory::CircleTrajectory(const char* type, GLfloat speed, GLint itCount)
	:Trajectory{ type, speed, itCount }, angle{ 0.0f }, first{ true }, rotationMatrix{ Matrix().SetIdentity() } {}

CircleTrajectory::CircleTrajectory(Type type, GLfloat speed, GLint itCount)
	: Trajectory{ type, speed, itCount }, angle{ 0.0f }, first{ true }, rotationMatrix{ Matrix().SetIdentity() } {}

GLfloat adjustAngle(const GLfloat previous, const GLfloat step)
{
	return (TO_ANG(previous + TO_RAD(step)) < 360.0f) ? TO_ANG(previous + TO_RAD(step)) : 0.0f;
}

void CircleTrajectory::move(SceneObject* obj, GLfloat time)
{
	if (iterationCount != 0)
	{
		if (first)
		{
			/*obj->setRotation({
				TO_ANG(obj->getRotation().x + rotationPlane.x),
				TO_ANG(obj->getRotation().y + rotationPlane.y),
				TO_ANG(obj->getRotation().z + rotationPlane.z)
				});*/

			initial = obj->getRotation();

			first = false;
		}

		GLfloat step = speed * time;

		if (angle + step < 360.0f)
		{
			angle += step;

			updateObject(obj, step);
		}
		else
		{
			updateObject(obj, step);

			--iterationCount;
			angle = 0.0f;
		}
	}
}

void CircleTrajectory::updateObject(SceneObject* obj, const GLfloat& step)
{
	auto pos = rotationMatrix * Vector4{ radius * ::sin(TO_RAD(angle)), 0.0f, radius * ::cos(TO_RAD(angle)), 1.0f };
	obj->setPosition({ pos.x + center.x, pos.y + center.y, pos.z + center.z });

	obj->setRotation({
			TO_ANG(obj->getRotation().x),
			adjustAngle(obj->getRotation().y, step),
			TO_ANG(obj->getRotation().z)
		});
}

const Vector3& CircleTrajectory::getCenter() const
{
	return center;
}

void CircleTrajectory::setCenter(const Vector3& center)
{
	this->center = center;
}

Vector3 CircleTrajectory::getRotationPlane() const
{
	return rotationPlane;
}

void CircleTrajectory::setRotationPlane(const Vector3& rotationPlane)
{
	this->rotationPlane = { TO_RAD(rotationPlane.x), TO_RAD(rotationPlane.y), TO_RAD(rotationPlane.z) };

	auto rotation = Matrix();
	rotationMatrix = rotationMatrix * rotation.SetRotationX(this->rotationPlane.x);
	rotationMatrix = rotationMatrix * rotation.SetRotationY(this->rotationPlane.y);
	rotationMatrix = rotationMatrix * rotation.SetRotationZ(this->rotationPlane.z);
}

GLfloat CircleTrajectory::getRadius() const
{
	return radius;
}

void CircleTrajectory::setRadius(GLfloat radius)
{
	this->radius = radius;
}
