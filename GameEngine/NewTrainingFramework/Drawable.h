#pragma once

class Drawable
{
public:
	// Default constructor
	Drawable() = default;

	// Prevent code auto-generation
	Drawable(const Drawable& copy) = delete;
	Drawable(Drawable&& move) noexcept = delete;
	Drawable& operator=(const Drawable& copy) = delete;
	Drawable& operator=(Drawable&& move) noexcept = delete;

	// Draw on screen
	virtual void draw() = 0;

	// Update properties
	virtual void update() = 0;
	
	// Virtual Destructor
	virtual ~Drawable() = default;
};