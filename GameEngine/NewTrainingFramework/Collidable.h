#pragma once
#include "OnCollisionListener.h"
#include "EventManager.h"
#include <unordered_map>
#include "Logger.h"
#include <memory>

class Collidable {
	std::unordered_map<Collidable*, bool> collisionMap;

	std::unique_ptr<OnCollisionListener> collisionListener;

protected:
	bool isCollisionEnabled;

	Collidable(bool isCollisionEnabled = true, OnCollisionListener* collisionListener = nullptr)
		: isCollisionEnabled{ isCollisionEnabled }, collisionListener{ collisionListener } {}

public:
	// Implement rule when two objects collide.
	virtual bool collides(Collidable* object) const = 0;

	// Uses template pattern to separate implementation details.
	virtual bool collideWith(Collidable* object)
	{
		if (isCollisionEnabled && object->isCollisionEnabled)
		{
			const auto& collision = this->collides(object);

			if (!collisionMap[object] && collision)
			{
				collisionMap[object] = true;

				if (collisionListener != nullptr)
				{
					collisionListener->onCollision();
				}

				return true;
			}
			else if (!collision)
			{
				collisionMap[object] = false;
			}
		}

		return false;
	}

	// Attach a listener to execute it when a collision appears.
	void addOnCollisionListener(OnCollisionListener* onCollisionListener) {
		this->collisionListener = std::unique_ptr<OnCollisionListener>(onCollisionListener);
	}

	void setCollisionProperty(bool isEnabled)
	{
		this->isCollisionEnabled = isEnabled;
	}

	const bool isCollisionActivated() const
	{
		return isCollisionEnabled;
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