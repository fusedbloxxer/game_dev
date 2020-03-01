#include "stdafx.h"
#include "Event.h"
#include <future>
#include <thread>
#include <chrono>

void Event::add(const Trigger::Type& trigger)
{
	triggers.insert(trigger);
}

void Event::add(const std::shared_ptr<Action>& action)
{
	actions.push_back(action);
}

void Event::setTriggers(const std::unordered_set<Trigger::Type>& triggers)
{
	this->triggers = triggers;
}

void Event::setActions(const std::vector<std::shared_ptr<Action>>& actions)
{
	this->actions = actions;
}

void Event::setUpEvent(const std::unordered_set<Trigger::Type>& triggers, const std::vector<std::shared_ptr<Action>>& actions)
{
	setActions(actions);
	setTriggers(triggers);
}

void Event::trigger(Trigger::Type trigger) const
{
	// Launch thread which will execute some actions
	if (triggers.find(trigger) != triggers.cend())
	{
		std::async(
			[&]()
			{
				for (const auto& action : actions)
				{
					action->execute();
				}
			}
		);
	}
	else
	{
		throw std::runtime_error{ "Trigger does not belong to this event." };
	}
}

std::string Event::inputToOutput(const GLint& input)
{
	if (input == Event::Type::EVENT_MOVE)
	{
		return "EVENT_MOVE";
	}
	else if (input == Event::Type::EVENT_PASSIVE)
	{
		return "EVENT_PASSIVE";
	}
	else if (input == Event::Type::EVENT_COLLISION)
	{
		return "EVENT_COLLISION";
	}
	else if (input == Event::Type::EVENT_CAMERA_COLLISION)
	{
		return "EVENT_CAMERA_COLLISION";
	}
	else
	{
		throw std::runtime_error{ "Event::Type - std::string conversion failed." };
	}
}

GLint Event::outputToInput(const std::string& output)
{
	if (output == "EVENT_PASSIVE")
	{
		return Event::Type::EVENT_PASSIVE;
	}
	else if (output == "EVENT_MOVE")
	{
		return Event::Type::EVENT_MOVE;
	}
	else if (output == "EVENT_COLLISION")
	{
		return Event::Type::EVENT_COLLISION;
	}
	else if (output == "EVENT_CAMERA_COLLISION")
	{
		return Event::Type::EVENT_CAMERA_COLLISION;
	}
	else
	{
		throw std::runtime_error{ "Invalid std::string - Event::Type conversion." };
	}
}
