#pragma once
#include "ShaderResource.h"
#include "Loadable.h"
#include <memory>

class Shader : public Loadable
{
	// Shader Resource Config
	std::shared_ptr<ShaderResource> sr;

	// Program Id
	GLint programId;

public:
	// Constructor
	Shader(std::shared_ptr<ShaderResource> sr = nullptr);

	// Destructor
	virtual ~Shader();

	// Getters and setters
	GLint getProgramId() const;
	void setProgramId(GLint programId);

	std::shared_ptr<ShaderResource> getShaderResource();
	void setShaderResource(std::shared_ptr<ShaderResource> shaderResource);

	// Inherited via Loadable
	virtual void load() override;
};