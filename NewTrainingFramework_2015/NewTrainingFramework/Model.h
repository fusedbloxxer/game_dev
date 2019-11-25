#pragma once
#include "ModelResource.h"
#include "Loadable.h"
#include <vector>
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

	// Get wired vector from indexes vector.
	std::vector<GLushort> getWired(const std::vector<GLushort>& indexes);

public:
	// Constructor
	Model(std::shared_ptr<ModelResource> mr = nullptr);

	// Initializer
	Model& init(std::shared_ptr<ModelResource> mr);

	// Destructor
	virtual ~Model();

	// For generated models
	template<typename VertexType>
	void load(const std::vector<VertexType>& vertices, const std::vector<GLushort>& indexes);

	// Inherited via Loadable
	virtual void load() override;

	// Inherited via Loadable
	virtual void freeResources() override;

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
};

template<typename VertexType>
void Model::load(const std::vector<VertexType>& vertices, const std::vector<GLushort>& indexes)
{
	noInd = indexes.size();

	// Load vertices into buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Load indexes into buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * noInd, indexes.data(), GL_STATIC_DRAW);

	// Create wireframe indexes
	auto wireframe = getWired(indexes);
	noIndWired = wireframe.size();

	// Load wirefram indexes into buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * noIndWired, wireframe.data(), GL_STATIC_DRAW);

	if (mr)
	{
		std::cout << "Model was loaded succesfully.";
		std::cout << " Id: " << mr->id << ".\n";
		std::cout << std::endl;
	}
	holdsResources = true;
}