#include "stdafx.h"
#include "SoundAction.h"
#include <random>
#include <ctime>
#include <thread>
#include <chrono>

SoundAction::SoundAction(const std::vector<std::shared_ptr<Sound>>& sounds)
	:sounds{ sounds }, previous{ 0 }
{
	srand(time(NULL));
}

void SoundAction::execute()
{
	if (sounds.size() > 1) 
	{
		auto rnd = rand() % sounds.size();
		for (; rnd == previous; rnd = rand() % sounds.size()); 
		previous = rnd;
	}
	sounds[previous]->playSound();
}

bool SoundAction::isExecuting() const
{
	return sounds[previous]->isPlaying();
}
