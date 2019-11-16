#include "stdafx.h"
#include "ShaderResource.h"

ShaderResource::ShaderResource(int id, std::string vs, std::string fs)
	: id{ id }, vsShader{ vs }, fsShader{ fs }
{
}