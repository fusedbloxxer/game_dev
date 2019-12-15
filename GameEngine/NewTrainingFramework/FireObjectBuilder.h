#pragma once
#include "SceneObjectBuilder.h"
#include "FireObject.h"

class FireObjectBuilder final : public SceneObjectBuilder
{
	// Max displacement
	GLfloat dispMax;

public:
	// Constructor
	FireObjectBuilder(SceneObject::Type type, GLint id);

	// Inherited via SceneObjectBuilder
	virtual FireObject* build() const override;

	FireObjectBuilder& setDispMax(GLfloat dispMax);

	// Virtual destructor
	virtual ~FireObjectBuilder() = default;
};