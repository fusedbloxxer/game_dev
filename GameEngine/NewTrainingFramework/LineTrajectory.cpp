#include "stdafx.h"
#include "LineTrajectory.h"

LineTrajectory::LineTrajectory(const char* type, GLint itCount, GLfloat speed, GLboolean direction, const std::vector<Vector3>& points)
	:Trajectory{ type, itCount, speed }, distance{ 0.0f }, pos{ 0 }, vecDirection{ 0.0f, 0.0f, 0.0f }, alternateDirection{ direction }, orientation{ 1 }, reset{ true }, rotating{ false }, first{ true }
{
	setPoints(points);
}

LineTrajectory::LineTrajectory(Type type, GLint itCount, GLfloat speed, GLboolean direction, const std::vector<Vector3>& points)
	: Trajectory{ type, itCount, speed }, distance{ 0.0f }, pos{ 0 }, vecDirection{ 0.0f, 0.0f, 0.0f }, alternateDirection{ direction }, orientation{ 1 }, reset{ true }, rotating{ false }, first{ true }
{
	setPoints(points);
}

template<typename T>
int sign(T value)
{
	if (360 - abs(value) < abs(value) - 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

Matrix skewSym(const Vector3& x)
// Skey-Sym Matrix = Sym and Sign Sym
{
	Matrix A{ 0.0f };

	A.m[0][1] = x.z;
	A.m[0][2] = -x.y;

	A.m[1][0] = -x.z;
	A.m[1][2] = x.x;

	A.m[2][0] = x.y;
	A.m[2][1] = -x.x;

	return A;
}

Matrix rotation(Vector3& a, Vector3& b)
// Rotation matrix from vector a to vector b
{
	auto x = a.Cross(b) / (a.Cross(b).Length());
	auto u = ::acos(a.Dot(b) / (a.Length() * b.Length()));
	auto A = skewSym(x);
	return Matrix().SetIdentity() + A * ::sin(u) + A * A * (1 - ::cos(u));
}

void LineTrajectory::rotate(SceneObject* obj, GLfloat time)
{
	if (rotationValue > 0)
	{
		GLfloat step = (rotationValue <= rotationSpeed * time) ? rotationValue : rotationSpeed * time;
		rotationValue = (rotationValue <= step) ? 0.0f : rotationValue - step;

		auto objectRotation = obj->getRotation();

		obj->setRotation({ TO_ANG(objectRotation.x), TO_ANG(objectRotation.y) + step, TO_ANG(objectRotation.z) });
	}
	else
	{
		rotating = false;
	}
}

void LineTrajectory::move(SceneObject* obj, GLfloat time)
{
	if (iterationCount != 0)
	{
		if (rotating)
		{
			rotate(obj, time);
		}
		else
		{
			if (reset && points.size() > 0)
			{
				obj->setPosition(points[0]);
				reset = false;

				if (first == false && type == Trajectory::Type::LINE_LOOP)
				{
					setRotation();
					return;
				}

				first = false;
			}

			Vector3 travel = calculateStep(time);

			obj->setPosition(obj->getPosition() + ((distance <= travel.Length()) ? vecDirection * distance : travel));
			(distance <= travel.Length()) ? calculateTravelProps(obj, time) : distance -= travel.Length();
		}
	}
}

void LineTrajectory::calculateTravelProps(SceneObject* obj, GLfloat time)
{
	if (pos < points.size() && pos >= 0)
	{
		if (Trajectory::Type::LINE_LOOP == type && pos == points.size() - 2)
		{
			setRotation();
		}
		else if (pos == points.size() - 1)
		{
			if (alternateDirection)
			{
				orientation = -orientation;

				setRotation();
			}
			else
			{
				pos = 0;
				
				reset = true;

				--iterationCount;
			}
		}
		else if (pos == 0 && orientation == -1)
		{
			--iterationCount;
			orientation = -orientation;

			setRotation();
		}

		// Calculate the vector
		vecDirection = points[pos + orientation] - points[pos];

		// Calculate the distance to be travelled.
		distance = vecDirection.Length();

		// Normalize the directon vector.
		vecDirection.Normalize();

		// Move further
		pos += orientation;
	}
}

void LineTrajectory::setRotation()
{
	// Rotate the object !
	rotationValue = 180.0f;
	rotating = true;
}

Vector3 LineTrajectory::calculateStep(GLfloat time)
{
	return vecDirection * time * speed;
}

void LineTrajectory::print(std::ostream& os) const
{
	Trajectory::print(os);
	os << " rotationSpeed: " << rotationSpeed << " points: ";
	for (decltype(points.size()) it = 0; it < points.size(); ++it)
	{
		os << points[it];
		if (it + 1 != points.size())
		{
			os << " ";
		}
	}
}

void LineTrajectory::setPoints(const std::vector<Vector3>& points)
{
	if (type == Trajectory::Type::LINEAR && points.size() > 2)
	{
		throw std::invalid_argument{ "Linear trajectory cannot have more than 2 points." };
	}

	this->points = points;

	if (type == Trajectory::Type::LINE_LOOP && points.size() > 0)
	{
		this->points.push_back(points[0]);
	}
}

std::vector<Vector3>& LineTrajectory::getPoints()
{
	return points;
}

GLboolean LineTrajectory::isAlternate() const
{
	return alternateDirection;
}

void LineTrajectory::setDirection(GLboolean direction)
{
	this->alternateDirection = direction;
}

void LineTrajectory::setDirection(const char* direction)
{
	if (::strcmp(direction, "normal") == 0)
	{
		this->alternateDirection = false;
	}
	else if (::strcmp(direction, "alternate") == 0)
	{
		this->alternateDirection = true;
	}
	else
	{
		throw std::invalid_argument{ "Invalid direction type." };
	}
}

GLfloat LineTrajectory::getRotationSpeed() const
{
	return rotationSpeed;
}

void LineTrajectory::setRotationSpeed(GLfloat speed)
{
	this->rotationSpeed = speed;
}
