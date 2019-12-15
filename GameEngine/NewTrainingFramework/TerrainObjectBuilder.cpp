#include "stdafx.h"
#include "TerrainObjectBuilder.h"

TerrainObjectBuilder::TerrainObjectBuilder(SceneObject::Type type, GLint id)
	:SceneObjectBuilder{ type, id } {}

TerrainObject* TerrainObjectBuilder::build() const
{
	TerrainObject* object = new TerrainObject(id, type);

	SceneObjectBuilder::sendCommonProperties(object);

	// Send specific properties
	object->setColorBind(r, g, b, blend);
	object->setSideCells(sideCells);
	object->setCellSize(cellSize);
	object->setHeight(height);
	object->setCenter(center);
	object->generateModel();

	return object;
}

TerrainObjectBuilder& TerrainObjectBuilder::setSideCells(GLint sideCells)
{
	this->sideCells = sideCells;
	return *this;
}

TerrainObjectBuilder& TerrainObjectBuilder::setCellSize(GLfloat cellSize)
{
	this->cellSize = cellSize;
	return *this;
}

TerrainObjectBuilder& TerrainObjectBuilder::setHeight(const Vector3& height)
{
	this->height = height;
	return *this;
}

TerrainObjectBuilder& TerrainObjectBuilder::setCenter(const Vector3& center)
{
	this->center = center;
	return *this;
}

TerrainObjectBuilder& TerrainObjectBuilder::setColorBind(GLuint r, GLuint g, GLuint b, GLuint blend)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->blend = blend;
	return *this;
}