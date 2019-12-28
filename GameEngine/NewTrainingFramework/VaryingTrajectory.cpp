#include "stdafx.h"
#include "VaryingTrajectory.h"

VaryingTrajectory::VaryingTrajectory(const char* type, GLint itCount, GLfloat speed, GLboolean direction)
	:Trajectory{ type, itCount, speed }, alternateDirection{ direction }
{
}

VaryingTrajectory::VaryingTrajectory(Type type, GLint itCount, GLfloat speed, GLboolean direction)
	: Trajectory{ type, itCount, speed }, alternateDirection{ direction }
{
}

GLboolean VaryingTrajectory::isAlternate() const
{
	return alternateDirection;
}

void VaryingTrajectory::setDirection(GLboolean direction)
{
	this->alternateDirection = direction;
}
