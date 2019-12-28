#pragma once
#include "SceneObject.h"

class Trajectory
{
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
	GLint getItCount() const;
};