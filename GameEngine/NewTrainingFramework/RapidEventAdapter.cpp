#include "stdafx.h"
#include "RapidEventAdapter.h"
#include "ResourceManager.h"
#include "SoundAction.h"

RapidEventAdapter::RapidEventAdapter(const char* filePath)
	:Rapid{ filePath } {}

std::unordered_map<Event::Type, std::shared_ptr<Event>> RapidEventAdapter::getEvents() const
{
	Sound::setUpSystem();

	const auto& eventsTag = root->first_node("events"); if (!eventsTag) { throw std::runtime_error{ "No events were found" }; }
	std::unordered_map<Event::Type, std::shared_ptr<Event>> events;

	for (auto eventTag = eventsTag->first_node("event"); eventTag; eventTag = eventTag->next_sibling())
	{
		if (!eventTag->first_attribute("type")) { throw std::runtime_error{ "No type was found for an event." }; }

		const auto& type = eventTag->first_attribute("type")->value();
		const auto& triggers = getTriggers(eventTag);
		const auto& actions = getActions(eventTag);

		(events[static_cast<Event::Type>(Event::outputToInput(type))] = std::shared_ptr<Event>(new Event()))->setUpEvent(triggers, actions);
	}

	return events;
}

std::unordered_set<Trigger::Type> RapidEventAdapter::getTriggers(const rapidxml::xml_node<>* eventTag) const
{
	const auto& triggersTag = eventTag->first_node("triggers"); if (!triggersTag) { throw std::runtime_error{ "No triggers were found." }; }
	std::unordered_set<Trigger::Type> triggers;

	for (auto triggerTag = triggersTag->first_node("trigger"); triggerTag; triggerTag = triggerTag->next_sibling())
	{
		triggers.insert(static_cast<Trigger::Type>(Trigger::outputToInput(triggerTag->value())));
	}

	return triggers;
}

std::vector<std::shared_ptr<Action>> RapidEventAdapter::getActions(const rapidxml::xml_node<>* eventTag) const
{
	const auto& actionsTag = eventTag->first_node("actions"); if (!actionsTag) { throw std::runtime_error{ "No actions were found." }; }
	std::vector<std::shared_ptr<Action>> actions;

	for (auto actionTag = actionsTag->first_node("action"); actionTag; actionTag = actionTag->next_sibling())
	{
		actions.push_back(getSingleAction(actionTag));
	}

	return actions;
}

std::shared_ptr<Action> RapidEventAdapter::getSingleAction(const rapidxml::xml_node<>* actionTag) const
{
	const auto& typeAttrib = actionTag->first_attribute("type"); if (!typeAttrib) { throw std::runtime_error{ "No action type was found." }; }

	if (std::strcmp("sound", typeAttrib->value()) == 0)
	{
		FMOD_MODE loop; bool overlap;

		{
			// Get isSoundLooping attribute
			const auto& loopAttrib = actionTag->first_attribute("loop"); if (!loopAttrib) { throw std::runtime_error{ "Loop attribute was not found." }; }
			loop = std::strcmp(loopAttrib->value(), "on") == 0 ? FMOD_LOOP_NORMAL
				: std::strcmp(loopAttrib->value(), "off") == 0 ? FMOD_LOOP_OFF
				: throw std::runtime_error{ "Invalid loop mode." };
		}

		{
			// Get overlapping attribute
			const auto& overlapAttrib = actionTag->first_attribute("overlap"); if (!overlapAttrib) { throw std::runtime_error{ "Overlap attribute was not found." }; }
			overlap = std::strcmp(overlapAttrib->value(), "true") == 0 ? true
				: std::strcmp(overlapAttrib->value(), "false") == 0 ? false
				: throw std::runtime_error{ "Invalid overlap mode." };
		}
		
		std::vector<std::shared_ptr<Sound>> sounds;

		for (auto soundTag = actionTag->first_node("sound"); soundTag; soundTag = soundTag->next_sibling())
		{
			GLint id;

			{
				// Get sound id
				const auto& soundId = soundTag->value(); if (!soundId) { throw std::runtime_error{ "Sound id was not found." }; }
				id = static_cast<GLint>(::atoi(soundId));
			}

			sounds.push_back(ResourceManager::getInstance()->load(id, loop, overlap));
		}

		return std::make_shared<SoundAction>(sounds);
	}
	else
	{
		throw std::runtime_error{ "This action type is not yet implemented." };
	}
}
