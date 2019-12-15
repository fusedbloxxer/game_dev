#pragma once
#include "SceneObjectBuilder.h"
#include "TerrainObject.h"

class TerrainObjectBuilder final : public SceneObjectBuilder
{
	// Even number of cells on a side
	GLint sideCells;

	// The size of cell line
	GLfloat cellSize;

	// Terrain center
	Vector3 center;

	// Color height
	Vector3 height;

	// Color binding
	GLuint r, g, b, blend;

public:
	// Constructor
	TerrainObjectBuilder(SceneObject::Type type, GLint id);

	// Inherited via SceneObjectBuilder
	virtual TerrainObject* build() const override;

	// Setters
	TerrainObjectBuilder& setSideCells(GLint sideCells);
	TerrainObjectBuilder& setCellSize(GLfloat cellSize);
	TerrainObjectBuilder& setHeight(const Vector3& height);
	TerrainObjectBuilder& setCenter(const Vector3& center);
	TerrainObjectBuilder& setColorBind(GLuint r, GLuint g, GLuint b, GLuint blend);

	// Virtual destructor
	virtual ~TerrainObjectBuilder() = default;
};

