#include "stdafx.h"
#include "EventManager.h"

EventManager* EventManager::getInstance()
{
	if (!INSTANCE)
	{
		INSTANCE = new EventManager();
	}
	return INSTANCE;
}

void EventManager::init(EventAdapter* eventAdapter)
{
	// Fetch events from adapter
	this->events = eventAdapter->getEvents();

	this->holdsResources = true;

	delete eventAdapter;
}

const std::shared_ptr<Event>& EventManager::event(const Event::Type& type)
{
	if (events.find(type) != events.cend())
	{
		return events[type];
	}

	throw std::invalid_argument{ "No event was found for the specified type." };
}

EventManager* EventManager::add(const Event::Type& type, const std::shared_ptr<Event>& event)
{
	events[type] = event;

	this->holdsResources = true;

	return this;
}

EventManager* EventManager::add(const std::unordered_map<Event::Type, std::shared_ptr<Event>>& events)
{
	this->events = events;

	this->holdsResources = true;

	return this;
}

EventManager::~EventManager() = default;

void EventManager::freeResources()
{
	events.clear();
	holdsResources = false;
}
