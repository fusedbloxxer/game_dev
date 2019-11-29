#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Math.h"
#include <iostream>

class Vertex_NFG
{
	friend std::istream& operator>>(std::istream& is, Vertex_NFG& obj);
	friend std::ostream& operator<<(std::ostream& os, const Vertex_NFG& obj);
public:
	Vertex_NFG();
	Vertex_NFG(const Vertex_NFG& copy);
	Vector3 pos, binorm, norm, tgt, color;
	Vector2 uv, uv2;
};

void find_num(const GLchar* const buff, GLint& i);
GLfloat extr_num(const GLchar* const buff, GLint& i);
