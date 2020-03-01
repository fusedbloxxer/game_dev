#pragma once
#include "Event.h"
#include <unordered_map>

class EventAdapter
{
public:
	virtual std::unordered_map<Event::Type, std::shared_ptr<Event>> getEvents() const = 0;

	virtual ~EventAdapter() = 0;
};