#pragma once
#include "Loadable.h"

class AxisModel : public Loadable
{
	// Buffer id that contains the vertices
	GLuint vboId;

	// Axis margins
	Vector3 max;

public:
	AxisModel(const Vector3& max = {});

	AxisModel& init(const Vector3& max = {});

	// Inherited via Loadable
	virtual void freeResources() override;
	virtual void load() override;

	// Virtual Destructor
	virtual ~AxisModel();

	GLuint getId() const;
};

