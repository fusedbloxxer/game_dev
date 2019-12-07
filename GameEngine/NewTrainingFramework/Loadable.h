#pragma once
#include "Releaser.h"

class Loadable : public Releaser
{
public:
	Loadable() = default;
	virtual void load() = 0;
	virtual ~Loadable() = default;
};