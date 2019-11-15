#pragma once
#include "ModelResource.h"
#include "Loadable.h"
#include <memory>

class Model : public Loadable
{
	// Model Resource Config
	std::shared_ptr<ModelResource> mr;

	// Buffer id that contains the indexes
	GLint iboId;

	// Buffer id that contains indexes for wired output
	GLint wiredboId;

	// Buffer id that contains the vertices
	GLint vboId;

	// The number of indexes
	GLint noInd, noIndWired;

public:

	// Constructor
	Model(std::shared_ptr<ModelResource> mr = nullptr);

	// Destructor
	virtual ~Model();

	// Getters and setters
	GLint getIboId() const;
	void setIboId(GLint id);

	GLint getWiredboId() const;
	void setWiredboId(GLint id);

	GLint getVboId() const;
	void setVboId(GLint id);

	GLint getNoInd() const;
	void setNoInd(GLint id);

	GLint getNoIndWired() const;
	void setNoIndWired(GLint id);

	std::shared_ptr<ModelResource> getModelResource();
	void setModelResource(std::shared_ptr<ModelResource> mr);

	// Inherited via Loadable
	virtual void load() override;
};