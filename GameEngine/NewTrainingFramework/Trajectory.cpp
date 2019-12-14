#include "stdafx.h"
#include "Trajectory.h"

Trajectory::Type Trajectory::strToType(const char* type)
{
	if (std::strcmp(type, "linear") == 0)
	{
		return Type::LINEAR;
	}
	else if (std::strcmp(type, "line_strip") == 0)
	{
		return Type::LINE_STRIP;
	}
	else if (std::strcmp(type, "line_loop") == 0)
	{
		return Type::LINE_STRIP;
	}
	else if (std::strcmp(type, "circle") == 0)
	{
		return Type::CIRCLE;
	}
	else
	{
		throw std::invalid_argument{ "Type does not exist !" };
	}
}

Trajectory::~Trajectory()
{
}

Trajectory::Trajectory(const char* type, GLint itCount, GLfloat speed, GLboolean direction)
	:Trajectory{ strToType(type), itCount, speed, direction }
{
}

Trajectory::Trajectory(Type type, GLint itCount, GLfloat speed, GLboolean direction)
	: type{ type }, iterationCount{ itCount }, speed{ speed }, normalDirection{ direction }
{
}

void Trajectory::setSpeed(GLfloat speed)
{
	this->speed = speed;
}

GLfloat Trajectory::getSpeed() const
{
	return speed;
}

void Trajectory::setType(Type type)
{
	this->type = type;
}

Trajectory::Type Trajectory::getType() const
{
	return type;
}

void Trajectory::setItCount(GLint itCount)
{
	this->iterationCount = itCount;
}

GLint Trajectory::getItCount() const
{
	return iterationCount;
}

GLboolean Trajectory::isNormalDirection() const
{
	return normalDirection;
}

void Trajectory::setNormalDirection(GLboolean direction)
{
	this->normalDirection = direction;
}
