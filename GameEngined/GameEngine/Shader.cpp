#include "Shader.h"

Shader::Shader(std::shared_ptr<ShaderResource> sr)
	:programId{}, sr{ nullptr }
{
	// TODO;
}

Shader::~Shader()
{
	// TODO;
}

void Shader::load()
{
	// TODO;
}

GLint Shader::getProgramId() const
{
	return programId;
}

void Shader::setProgramId(GLint programId)
{
	this->programId = programId;
}

std::shared_ptr<ShaderResource> Shader::getShaderResource()
{
	return sr;
}

void Shader::setShaderResource(std::shared_ptr<ShaderResource> shaderResource)
{
	sr = shaderResource;
}