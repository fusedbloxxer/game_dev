#pragma once
#include "ShaderResource.h"
#include "Loadable.h"
#include "Fields.h"
#include <memory>

class Shader : public Loadable
{
	// Shader Resource Config
	std::shared_ptr<ShaderResource> sr;

	// File fields
	Fields fields;

	// Program Id
	GLuint programId;

public:
	// Constructor
	Shader(std::shared_ptr<ShaderResource> sr = nullptr);

	// Initializer
	Shader& init(std::shared_ptr<ShaderResource> sr);

	// Destructor
	virtual ~Shader();

	// Getters and setters
	GLuint getProgramId() const;
	void setProgramId(GLuint programId);

	Fields getFields() const;

	std::shared_ptr<ShaderResource> getShaderResource();
	void setShaderResource(std::shared_ptr<ShaderResource> shaderResource);

	// Inherited via Loadable
	virtual void load() override;

	// Inherited via Loadable
	virtual void freeResources() override;
};