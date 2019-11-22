#pragma once
#include "..\Utilities\utilities.h"

struct Resource
{
	// Unique id
	GLint id;

	// Constructor
	Resource(GLint id);

	// Virtual Destructor
	virtual ~Resource();
};