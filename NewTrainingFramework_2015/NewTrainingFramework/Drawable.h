#pragma once

class Drawable
{
public:
	// Draw on screen
	virtual void draw() = 0;

	// Update properties
	virtual void update() = 0;
	
	// Virtual Destructor
	virtual ~Drawable();
};

