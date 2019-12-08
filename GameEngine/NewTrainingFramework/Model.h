#pragma once
#include "ModelResource.h"
#include "VertexNfg.h"
#include "AxisModel.h"
#include <vector>
#include <memory>

class Model : public Loadable
{
	// The model for its axis
	AxisModel axisModel;

	// Buffer id that contains the indexes
	GLuint iboId;

	// Buffer id that contains indexes for wired output
	GLuint wiredboId;

	// Buffer id that contains the vertices
	GLuint vboId;

	// The number of indexes
	GLuint noInd, noIndWired;

	// Model Resource Config
	std::shared_ptr<ModelResource> mr;

	// Get wired vector from indexes vector.
	std::vector<GLushort> getWired(const std::vector<GLushort>& indexes);

public:
	// Constructor
	Model(std::shared_ptr<ModelResource> mr = nullptr);

	// Initializer
	Model& init(std::shared_ptr<ModelResource> mr);

	// For generated models
	template<typename VertexType>
	void load(const std::vector<VertexType>& vertices, const std::vector<GLushort>& indexes);

	// Static method which parses .nfg files.
	static std::pair<std::vector<VertexNfg>, std::vector<GLushort>> parseFile(const char* const filePath);

	// Destructor
	virtual ~Model();

	// Inherited via Loadable
	virtual void load() override;
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

	AxisModel& getAxisModel();

	std::shared_ptr<ModelResource> getModelResource();
	void setModelResource(std::shared_ptr<ModelResource> mr);

private:
	template<typename VertexType>
	void loadAxisModel(const std::vector<VertexType>& vertices);
};

template<typename VertexType>
void Model::load(const std::vector<VertexType>& vertices, const std::vector<GLushort>& indexes)
{
	// Number of indexes
	noInd = indexes.size();

	// Load axis 
	loadAxisModel(vertices);

	if (holdsResources)
	{
		// Load vertices into buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexType) * vertices.size(), vertices.data());

		// Load indexes into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * noInd, indexes.data());

		// Create wireframe indexes
		auto wireframe = getWired(indexes);
		noIndWired = wireframe.size();

		// Load wirefram indexes into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredboId);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * noIndWired, wireframe.data());
	}
	else
	{
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

		holdsResources = true;
	}
}