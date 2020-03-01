#pragma once
#include "Updatable.h"

class Drawable : public Updatable
{
	// Prevent code auto-generation
	Drawable(const Drawable& copy) = delete;
	Drawable(Drawable&& move) noexcept = delete;
	Drawable& operator=(const Drawable& copy) = delete;
	Drawable& operator=(Drawable&& move) noexcept = delete;

protected:
	// Default constructor
	Drawable() = default;

public:
	// Draw on screen
	virtual void draw() = 0;
	
	// Virtual Destructor
	virtual ~Drawable() = default;
};