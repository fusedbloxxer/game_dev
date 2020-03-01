#pragma once
#include "OnCollisionListener.h"
class SceneObjectCollisionListener : public OnCollisionListener
{
public:
	// Inherited via OnCollisionListener
	virtual void onCollision() const override;

	virtual ~SceneObjectCollisionListener() = default;
};

