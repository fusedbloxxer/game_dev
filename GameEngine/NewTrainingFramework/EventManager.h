#pragma once
#include <unordered_map>
#include "EventAdapter.h"
#include "Releaser.h"
#include "Event.h"

class EventManager final : public Releaser
{
	inline static EventManager* INSTANCE = nullptr;

	EventManager() = default;

	EventManager(const EventManager& copy) = delete;
	
	EventManager(EventManager&& move) noexcept = delete;
	
	EventManager& operator=(const EventManager& copy) = delete;
	
	EventManager& operator=(EventManager&& move) noexcept = delete;

	std::unordered_map<Event::Type, std::shared_ptr<Event>> events;

public:
	// Get the singleton
	static EventManager* getInstance();

	void init(EventAdapter* eventAdapter);

	// Inherited via Releaser
	virtual void freeResources() override;

	// Retrieve event with specified type
	const std::shared_ptr<Event>& event(const Event::Type& type);

	// Populate EventManager with events
	EventManager* add(const Event::Type& type, const std::shared_ptr<Event>& event);
	EventManager* add(const std::unordered_map<Event::Type, std::shared_ptr<Event>>& events);

	virtual ~EventManager();
};