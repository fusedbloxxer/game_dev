#pragma once
#include "OnCollisionListener.h"
#include <memory>

class Collidable {
	bool isCollisionEnabled;
	std::unique_ptr<OnCollisionListener> collisionListener;

protected:
	Collidable(bool isCollisionEnabled = true, OnCollisionListener* collisionListener = nullptr)
		: isCollisionEnabled{ isCollisionEnabled }, collisionListener{ collisionListener } {}

public:
	// Implement rule when two objects collide.
	virtual bool collides(Collidable* object) const = 0;

	// Uses template pattern to separate implementation details.
	virtual bool collideWith(Collidable* object) const {
		if (isCollisionEnabled && object->isCollisionEnabled && collides(object))
		{
			// We have a listener attached.
			if (collisionListener != nullptr)
			{
				collisionListener->onCollision();
			}
			return true;
		}
		return false;
	}

	// Attach a listener to execute it when a collision appears.
	void addOnCollisionListener(OnCollisionListener* onCollisionListener) {
		this->collisionListener = std::unique_ptr<OnCollisionListener>(onCollisionListener);
	}

	void enableCollision()
	{
		this->isCollisionEnabled = true;
	}

	void disableCollision()
	{
		this->isCollisionEnabled = false;
	}

	virtual ~Collidable() = default;
};