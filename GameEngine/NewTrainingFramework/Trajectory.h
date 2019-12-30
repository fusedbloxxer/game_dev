#pragma once
#include "..\Utilities\utilities.h"
#include "SceneObject.h"
#include <typeinfo>
#include <iostream>

class SceneObject;

class Trajectory
{
	friend std::ostream& operator<<(std::ostream& os, const Trajectory& trajectory);

public:
	enum class Type
	{
		LINEAR,
		LINE_STRIP,
		LINE_LOOP,
		CIRCLE
	};

	inline static constexpr GLint DEFAULT_IT_COUNT = 1;
	inline static constexpr GLfloat DEFAULT_SPEED = 100.0f;
	static Type strToType(const char* type);

protected:
	// Trajectory type
	Type type;

	// Trajectory speed
	GLfloat speed;

	// Iteration count
	GLint iterationCount;

	// Constructors
	Trajectory(const char * type, GLfloat speed = DEFAULT_SPEED, GLint itCount = DEFAULT_IT_COUNT);
	Trajectory(Type type, GLfloat speed = DEFAULT_SPEED, GLint itCount = DEFAULT_IT_COUNT);

	// Virtual print
	virtual void print(std::ostream& os) const = 0;

public:
	// Move the object into the world
	virtual void move(SceneObject* object, GLfloat time) = 0;

	// Virtual destructor
	virtual ~Trajectory() = 0;

	// Getters and setters
	void setSpeed(GLfloat speed);
	GLfloat getSpeed() const;

	void setType(Type type);
	Type getType() const;

	void setItCount(GLint itCount);
	void setItCount(const char* itCount);
	GLint getItCount() const;
};