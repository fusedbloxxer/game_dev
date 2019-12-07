#include "stdafx.h"
#include "ShaderResource.h"

ShaderResource::ShaderResource(GLint id, std::string vs, std::string fs)
	: Resource{ id }, vsShader{ vs }, fsShader{ fs }
{
}