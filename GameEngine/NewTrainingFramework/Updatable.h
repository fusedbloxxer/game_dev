#pragma once

class Updatable
{
	// Prevent code auto-generation
	Updatable(const Updatable& copy) = delete;
	Updatable(Updatable&& move) noexcept = delete;
	Updatable& operator=(const Updatable& copy) = delete;
	Updatable& operator=(Updatable&& move) noexcept = delete;

protected:
	// Default constructor
	Updatable() = default;

public:
	// Update properties
	virtual void update() = 0;

	virtual ~Updatable() = default;
};