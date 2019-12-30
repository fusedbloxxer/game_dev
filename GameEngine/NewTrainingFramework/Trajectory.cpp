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
		return Type::LINE_LOOP;
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

Trajectory::~Trajectory() {}

Trajectory::Trajectory(const char* type, GLfloat speed, GLint itCount)
	:Trajectory{ strToType(type), speed, itCount } {}

Trajectory::Trajectory(Type type, GLfloat speed, GLint itCount)
	: type{ type }, speed{ speed }, iterationCount{ itCount }
{
	if (itCount < 0)
		throw std::invalid_argument{ "Iteration-count cannot be negative." };
}

void Trajectory::print(std::ostream& os) const
{

	os << typeid(*this).name() << " "
		<< "speed: " << speed << " "
		<< "iteration-count: " << iterationCount;
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

void Trajectory::setItCount(const char* itCount)
{
	if (::strcmp(itCount, "infinite") == 0)
	{
		iterationCount = -1;
	}
	else
	{
		iterationCount = ::atoi(itCount);
	}
}

GLint Trajectory::getItCount() const
{
	return iterationCount;
}

std::ostream& operator<<(std::ostream& os, const Trajectory& trajectory)
{
	trajectory.print(os);
	return os;
}
