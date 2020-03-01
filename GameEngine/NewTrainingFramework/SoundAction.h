#pragma once
#include "Action.h"
#include "Sound.h"
#include <vector>

class SoundAction : public Action
{
	std::vector<std::shared_ptr<Sound>> sounds;

	decltype(sounds.size()) previous;

public:
	SoundAction(const std::vector<std::shared_ptr<Sound>>& sounds);

	// Inherited via Action
	virtual bool isExecuting() const override;
	virtual void execute() override;

	virtual ~SoundAction() = default;

};