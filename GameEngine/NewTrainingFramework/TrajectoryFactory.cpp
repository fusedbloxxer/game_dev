#include "stdafx.h"
#include "TrajectoryFactory.h"
#include "CircleTrajectory.h"
#include "LineTrajectory.h"
#include "LinearTrajectory.h"

Trajectory* TrajectoryFactory::newInstance(Trajectory::Type type)
{
	switch (type)
	{
	case Trajectory::Type::LINEAR:
	case Trajectory::Type::LINE_LOOP:
	case Trajectory::Type::LINE_STRIP:
		return new LinearTrajectory(type);
		break;
	case Trajectory::Type::CIRCLE:
		return new CircleTrajectory(type);
		break;
	default:
		throw std::domain_error{ "Invalid trajectory type !" };
		break;
	}
}