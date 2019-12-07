#pragma once
#include "..\Utilities\utilities.h"

struct Resource
{
	// Unique id
	GLuint id;

	// Constructor
	Resource(GLuint id);

	// Virtual Destructor
	virtual ~Resource();
};