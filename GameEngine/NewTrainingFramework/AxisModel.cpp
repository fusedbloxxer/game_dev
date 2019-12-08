#include "stdafx.h"
#include "AxisModel.h"
#include "AxisResource.h"

AxisModel::AxisModel(const Vector3& max)
	:max{ max }, vboId{}
{
	// Create buffer
	glGenBuffers(1, &vboId); 
}

AxisModel& AxisModel::init(const Vector3& max)
{
	freeResources();
	this->max = max;
	return *this;
}

void AxisModel::freeResources()
{
	if (holdsResources)
	{
		holdsResources = false;
	}
}

void AxisModel::load()
{
	// Bind buffer to id
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// Send data to current active buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAxis) * AxisResource::VERTICES, AxisResource(max).getAxis(), GL_STATIC_DRAW);

	holdsResources = true;
}

AxisModel::~AxisModel()
{
	freeResources();
	glDeleteBuffers(1, &vboId);
}

GLuint AxisModel::getId() const
{
	return vboId;
}