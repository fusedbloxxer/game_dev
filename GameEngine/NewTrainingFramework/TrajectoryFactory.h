#pragma once
#include "Trajectory.h"

class TrajectoryFactory final
{
	TrajectoryFactory() = default;
public:
	static Trajectory* newInstance(Trajectory::Type type);
};

