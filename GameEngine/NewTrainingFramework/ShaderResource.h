#pragma once
#include "Resource.h"
#include <string>

struct ShaderResource : public Resource {
	std::string vsShader, fsShader;
	virtual ~ShaderResource() = default;
	ShaderResource(GLint id, std::string vs, std::string fs);
};

