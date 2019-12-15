#pragma once
#include "SceneObjectBuilder.h"
#include "SkyboxObject.h"

class SkyboxObjectBuilder final : public SceneObjectBuilder
{
public:
	// Constructor
	SkyboxObjectBuilder(SceneObject::Type type, GLint id);

	// Inherited via SceneObjectBuilder
	virtual SkyboxObject* build() const override;

	// Virtual destructor
	virtual ~SkyboxObjectBuilder() = default;
};

