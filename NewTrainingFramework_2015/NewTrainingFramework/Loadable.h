#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

class Loadable
{
protected:
	// Flag for resources
	GLboolean holdsResources;

public:
	// Constructors
	Loadable();

	// !TODO; Prevent code auto-generation
	Loadable(const Loadable& copy) = delete;
	Loadable(Loadable&& move) noexcept = delete;
	Loadable& operator=(const Loadable& copy) = delete;
	Loadable& operator=(Loadable&& move) noexcept = delete;

	// Virtual methods
	virtual void load() = 0;
	virtual void freeResources() = 0;
	virtual ~Loadable();
};