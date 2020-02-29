#pragma once
#include "Resource.h"
#include <string>

struct ModelResource : public Resource
{
	std::string file;

	ModelResource(GLint id, std::string file);

	virtual ~ModelResource() = default;
};