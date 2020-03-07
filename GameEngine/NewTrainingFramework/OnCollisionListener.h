#pragma once
#define NDEBUG
#undef NDEBUG

class OnCollisionListener
{
public:
	virtual void onCollision() const = 0;
	virtual ~OnCollisionListener() = default;
};