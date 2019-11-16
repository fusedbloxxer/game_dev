#pragma once
#include "ModelResource.h"
#include "Loadable.h"
#include <memory>

class Model : public Loadable
{
	// Model Resource Config
	std::shared_ptr<ModelResource> mr;

	// Buffer id that contains the indexes
	GLuint iboId;

	// Buffer id that contains indexes for wired output
	GLuint wiredboId;

	// Buffer id that contains the vertices
	GLuint vboId;

	// The number of indexes
	GLuint noInd, noIndWired;

	// Flag for resources
	GLboolean holdsResources;

public:

	// Constructor
	Model(std::shared_ptr<ModelResource> mr = nullptr);

	// Initializer
	Model& init(std::shared_ptr<ModelResource> mr);

	// Destructor
	virtual ~Model();

	// Getters and setters
	GLuint getIboId() const;
	void setIboId(GLuint id);

	GLuint getWiredboId() const;
	void setWiredboId(GLuint id);

	GLuint getVboId() const;
	void setVboId(GLuint id);

	GLuint getNoInd() const;
	void setNoInd(GLuint id);

	GLuint getNoIndWired() const;
	void setNoIndWired(GLuint id);

	std::shared_ptr<ModelResource> getModelResource();
	void setModelResource(std::shared_ptr<ModelResource> mr);

	// Inherited via Loadable
	virtual void load() override;

private:
	// Get wired vector from indexes vector.
	std::vector<GLushort> getWired(const std::vector<GLushort>& indexes);

	// Inherited via Loadable
	virtual void freeResources() override;
};