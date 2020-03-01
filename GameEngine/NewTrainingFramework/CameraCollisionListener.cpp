#include "stdafx.h"
#include "CameraCollisionListener.h"
#include "EventManager.h"

void CameraCollisionListener::onCollision() const
{
	EventManager::getInstance()->event(Event::Type::EVENT_CAMERA_COLLISION)->trigger(Trigger::Type::OBJECT_COLLISION);
}
