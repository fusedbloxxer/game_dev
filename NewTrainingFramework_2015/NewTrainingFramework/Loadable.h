#pragma once
#include "Releaser.h"

class Loadable : public Releaser
{
public:
	// Constructors
	Loadable();

	// Virtual methods
	virtual void load() = 0;
	virtual ~Loadable();
};