#pragma once
#include "Resource.h"
#include <string>

struct ModelResource : public Resource {
	std::string file;
	virtual ~ModelResource() = default;
	ModelResource(GLint id, std::string file);
};