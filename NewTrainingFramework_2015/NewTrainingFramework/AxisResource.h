#pragma once
#include "VertexAxis.h"

class AxisResource
{
public:
	inline static constexpr GLint VERTICES = 6;

	AxisResource(const Vector3& max = {});

	VertexAxis* getAxis();

private:
	VertexAxis vertices[VERTICES];
};