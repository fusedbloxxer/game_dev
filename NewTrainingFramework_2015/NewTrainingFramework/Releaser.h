#pragma once
#include "..\Utilities\utilities.h"

class Releaser
{
protected:
	// Flag for resources
	GLboolean holdsResources;

public:
	// Constructors
	Releaser() : holdsResources{ false } {}

	// !TODO; Prevent code auto-generation
	Releaser(const Releaser& copy) = delete;
	Releaser(Releaser&& move) noexcept = delete;
	Releaser& operator=(const Releaser& copy) = delete;
	Releaser& operator=(Releaser&& move) noexcept = delete;

	// Virtual methods
	virtual void freeResources() = 0;
	virtual ~Releaser() = default;
};

