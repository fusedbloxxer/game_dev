#pragma once
#include <string>

struct ShaderResource {
	int id;
	std::string vsShader, fsShader;
	ShaderResource(int id, std::string vs, std::string fs);
};

