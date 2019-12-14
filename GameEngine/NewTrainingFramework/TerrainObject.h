#pragma once
#include "GeneratedModel.h"
#include "SceneObject.h"
#include "VertexNfg.h"

class TerrainObject : public SceneObject, public GeneratedModel
{
	// Terrain is quadratic
	// Even number of cells on a side
	GLint sideCells;

	// The size of cell line
	GLfloat cellSize;

	// Terrain center
	Vector3 center;

	// Color height
	Vector3 height;

	// Moving diffs
	GLfloat dx, dz;

	// Color binding
	GLuint colorBind[4];

	// Vertices
	std::vector<VertexNfg> vertices;

	// Indexes
	std::vector<GLushort> indexes;

public:
	// Constructors
	TerrainObject(GLint id);
	TerrainObject(GLint id, Type type);

	// Virtual destructor
	virtual ~TerrainObject();

	// Iherited via SceneObject
	virtual void draw() override;

	// Iherited via SceneObject
	virtual void update() override;

	virtual Matrix& getModelMatrix() override;

	// Inherited via GeneratedModel
	virtual void generateModel() override;

	// Getters and setters
	void setSideCells(GLint sideCells);
	GLint getSideCells() const;

	void setCellSize(GLfloat cellSize);
	GLfloat getCellSize() const;

	void setCenter(const Vector3& center);
	Vector3& getCenter();

	void setHeight(Vector3& height);
	Vector3& getHeight();

	void setColorBind(GLuint r, GLuint g, GLuint b, GLuint blend);
	const GLuint* getColorBind() const;

private:
	template<typename Fun>
	void move(GLfloat& diff, const GLfloat& cpos, GLfloat& pos, Fun fun);
};