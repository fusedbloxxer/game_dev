#include "stdafx.h"
#include "SceneObjectCollisionListener.h"
#include "EventManager.h"

void SceneObjectCollisionListener::onCollision() const
{
	EventManager::getInstance()->event(Event::Type::EVENT_COLLISION)->trigger(Trigger::Type::OBJECT_COLLISION);
}
