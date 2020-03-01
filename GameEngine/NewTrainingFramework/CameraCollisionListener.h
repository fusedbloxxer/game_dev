#pragma once
#include "OnCollisionListener.h"
class CameraCollisionListener : public OnCollisionListener
{
public:
	// Inherited via OnCollisionListener
	virtual void onCollision() const override;

	virtual ~CameraCollisionListener() = default;
};

