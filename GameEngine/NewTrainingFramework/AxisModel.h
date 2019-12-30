#pragma once
#include "Loadable.h"

class AxisModel : public Loadable
{
	// Buffer id that contains the vertices
	GLuint vboId;

	// Axis margins
	Vector3 max;

public:
	// Initializers
	AxisModel(const Vector3& max = {});
	AxisModel& init(const Vector3& max = {});

	// Virtual Destructor
	virtual ~AxisModel();

	// Inherited via Loadable
	virtual void freeResources() override;
	virtual void load() override;

	// Getters and setters
	GLuint getId() const;

	Vector3 getAxis() const;
};