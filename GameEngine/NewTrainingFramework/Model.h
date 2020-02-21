#pragma once
#include "ModelResource.h"
#include "VertexAxis.h"
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

	// Buffer id that contains the normal lines
	GLuint normalVboId;

	// Buffer id that contains the collision box data
	GLuint collisionVboId;

	// Buffer id that contains the collision box indices
	GLuint collisionIboId;

	// The number of indexes
	GLuint noInd, noIndWired, noNormalInd, noCollisionInd, noCollisionIndices;

	// Model Resource Config
	std::shared_ptr<ModelResource> mr;

	// Get wired vector from indexes vector.
	std::vector<GLushort> getWired(const std::vector<GLushort>& indexes); 

	// Represents the collision box vertices
	std::vector<VertexAxis> AABBs;

	// Collision Box Color
	Vector3 collisionBoxColor;
	
	// Contains the maximum x, y, z values on the upper row and the min values on the lower row
	GLfloat m[2][3];

public:
	// Constructor
	Model(std::shared_ptr<ModelResource> mr = nullptr, const Vector3& collisionBoxColor = { 0.0f, 0.0f, 0.0f });

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

	GLuint getNoNormalInd() const;
	void setNoNormalInd(GLuint count);

	GLuint getNormalVboId() const;
	void setNormalVboId(GLuint id);

	GLuint getCollisionVboId() const;
	void setCollisionVboId(GLuint id);

	GLuint getNoCollisionInd() const;
	void setNoCollisionInd(GLuint count);

	GLuint getCollisionIboId() const;
	void setCollisionIboId(GLuint id);

	GLuint getNoCollisionIndices() const;
	void setNoCollisionIndices(GLuint count);

	const Vector3& getCollisionBoxColor() const;
	void setCollisionBoxColor(const Vector3& collisionBoxColor);

	// Use trailing return type to send a reference to the matrix
	auto getCollisionCoordinates() const -> const GLfloat(&)[2][3];

	AxisModel& getAxisModel();

	std::shared_ptr<ModelResource> getModelResource();
	void setModelResource(std::shared_ptr<ModelResource> mr);

public:
	template<typename VertexType>
	void loadCollisionBox(const std::vector<VertexType>& vertices);

	void updateCollisionBox(const Matrix& worldMatrix);

private:
	template<typename VertexType>
	void loadAxisModel(const std::vector<VertexType>& vertices);

	template<typename VertexType>
	void loadNormals(const std::vector<VertexType>& vertices);
};

template<typename VertexType>
void Model::load(const std::vector<VertexType>& vertices, const std::vector<GLushort>& indexes)
{
	// Number of indexes
	noInd = indexes.size();

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
		// Load AABBs 
		loadCollisionBox<VertexType>(vertices);

		// Load axis 
		loadAxisModel<VertexType>(vertices);

		// Load normals
		loadNormals<VertexType>(vertices);

		// Load vertices into buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		// Load indexes into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * noInd, indexes.data(), GL_STATIC_DRAW);

		// Create wireframe indexes
		auto wireframe = getWired(indexes);
		noIndWired = wireframe.size();

		// Load wireframe indexes into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * noIndWired, wireframe.data(), GL_STATIC_DRAW);

		holdsResources = true;
	}
}