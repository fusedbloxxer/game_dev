#pragma once
#include "..\Utilities\utilities.h"

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
	inline static constexpr GLboolean DEFAULT_DIRECTION = false;
	static Type strToType(const char* type);
protected:
	// Trajectory type
	Type type;

	// Trajectory speed
	GLfloat speed;

	// Forward / Backward movement
	GLboolean normalDirection;

	// Iteration count
	GLint iterationCount;

	// Constructors
	Trajectory(const char * type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION);
	Trajectory(Type type, GLint itCount = DEFAULT_IT_COUNT, GLfloat speed = DEFAULT_SPEED, GLboolean direction = DEFAULT_DIRECTION);

public:
	// Virtual destructor
	virtual ~Trajectory() = 0;

	// Getters and setters
	void setSpeed(GLfloat speed);
	GLfloat getSpeed() const;

	void setType(Type type);
	Type getType() const;

	void setItCount(GLint itCount);
	GLint getItCount() const;

	GLboolean isNormalDirection() const;
	void setNormalDirection(GLboolean direction);
};