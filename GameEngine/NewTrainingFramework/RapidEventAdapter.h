#pragma once
#include "Rapid.h"
#include "EventAdapter.h"

class RapidEventAdapter final : public EventAdapter, public Rapid
{
public:
	RapidEventAdapter(const char* filePath);

	// Inherited via EventAdapter
	virtual std::unordered_map<Event::Type, std::shared_ptr<Event>> getEvents() const override;

	virtual ~RapidEventAdapter() = default;

private:
	std::shared_ptr<Action> getSingleAction(const rapidxml::xml_node<>* actionsTag) const;

	std::unordered_set<Trigger::Type> getTriggers(const rapidxml::xml_node<>* eventTag) const;
	
	std::vector<std::shared_ptr<Action>> getActions(const rapidxml::xml_node<>* eventTag) const;
};

