#pragma once
#include "ModelResource.h"
#include "AxisModel.h"
#include <vector>
#include <memory>

class Model : public Loadable
{
	// The model for its axis
	AxisModel axisModel;

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

	AxisModel& getAxisModel();
	void setAxisModel(const AxisModel& axisModel);

	std::shared_ptr<ModelResource> getModelResource();
	void setModelResource(std::shared_ptr<ModelResource> mr);
};

template<typename VertexType>
void Model::load(const std::vector<VertexType>& vertices, const std::vector<GLushort>& indexes)
{
	noInd = indexes.size();

	if (vertices.size() > 0)
	{
		GLfloat x = vertices[0].pos.x, y = vertices[0].pos.y, z = vertices[0].pos.z;
		for (decltype(vertices.size()) i = 1; i < vertices.size(); ++i)
		{
			if (x < vertices[i].pos.x)
			{
				x = vertices[i].pos.x;
			}

			if (y < vertices[i].pos.y)
			{
				y = vertices[i].pos.y;
			}

			if (z < vertices[i].pos.z)
			{
				z = vertices[i].pos.z;
			}
		}

		axisModel.init({ x, y, z });
		axisModel.load();
	}

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