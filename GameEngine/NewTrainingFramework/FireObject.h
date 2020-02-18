#pragma once
#include "SceneObject.h"

class FireObject : public SceneObject
{
	inline static constexpr GLint TIME_FACTOR = 10'000;

	// Current time
	GLfloat time;

	// Max displacement
	GLfloat dispMax;

public:
	// Initializers
	FireObject(GLint id);
	FireObject(GLint id, Type type);

	// Inherited via SceneObject
	virtual void update() override;

	// Virtual destructor
	virtual ~FireObject();

	// Getters and setters
	GLfloat getTime() const;
	void setTime(GLfloat time);

	GLfloat getDispMax() const;
	void setDispMax(GLfloat dispMax);

protected:
	virtual void sendSpecificData(const Fields& fields) override;
};

