#include "stdafx.h"
#include "AxisModel.h"
#include "AxisResource.h"

AxisModel::AxisModel(const Vector3& max)
{
	glGenBuffers(1, &vboId);
	init(max);
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
		// TODO; FREE THEM
		holdsResources = false;
	}
}

void AxisModel::load()
{
	holdsResources = true;
	AxisResource axis{ max };

	// Bind buffer to id
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	// Send data to current active buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAxis) * AxisResource::VERTICES, axis.getAxis(), GL_STATIC_DRAW);
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
