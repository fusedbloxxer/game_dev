#include "stdafx.h"
#include "AxisResource.h"

AxisResource::AxisResource(const Vector3& mx)
{
	// Axis x, Axis y, Axis z
	vertices[0].pos = { mx.x, 0.0f, 0.0f };
	vertices[1].pos = { 0.0f, 0.0f, 0.0f };

	vertices[2].pos = { 0.0f, mx.y, 0.0f };
	vertices[3].pos = { 0.0f, 0.0f, 0.0f };

	vertices[4].pos = { 0.0f, 0.0f, mx.z };
	vertices[5].pos = { 0.0f, 0.0f, 0.0f };

	// RGB - XYZ
	vertices[0].color = { 1.0f, 0.0f, 0.0f };
	vertices[1].color = { 1.0f, 0.0f, 0.0f };

	vertices[2].color = { 0.0f, 1.0f, 0.0f };
	vertices[3].color = { 0.0f, 1.0f, 0.0f };

	vertices[4].color = { 0.0f, 0.0f, 1.0f };
	vertices[5].color = { 0.0f, 0.0f, 1.0f };
}

VertexAxis* AxisResource::getAxis()
{
	return vertices;
}