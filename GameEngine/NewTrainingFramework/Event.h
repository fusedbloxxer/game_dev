#pragma once
#include "..\Utilities\utilities.h"
#include "ObjectConverter.h"
#include <unordered_set>
#include "Trigger.h"
#include "Action.h"
#include <vector>
#include <string>

class Event
{
public:
	enum Type
	{
		EVENT_MOVE,
		EVENT_PASSIVE,
		EVENT_COLLISION,
		EVENT_CAMERA_COLLISION
	};

private:
	// Trigger::Type - On/Off
	std::unordered_set<Trigger::Type> triggers;

	// Vector of actions that will be executed when any trigger is On
	std::vector<std::shared_ptr<Action>> actions;

public:
	// Fill the event with data
	void add(const Trigger::Type& trigger);
	void add(const std::shared_ptr<Action>& action);
	void setTriggers(const std::unordered_set<Trigger::Type>& triggers);
	void setActions(const std::vector<std::shared_ptr<Action>>& actions);
	void setUpEvent(const std::unordered_set<Trigger::Type>& triggers, const std::vector<std::shared_ptr<Action>>& actions);

	// Launch execution if trigger is available
	void trigger(Trigger::Type trigger) const;

	// Converter methods
	static std::string inputToOutput(const GLint& input);
	static GLint outputToInput(const std::string& output);

	// Virtual destructor
	virtual ~Event() = default;
};