#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <iostream>
#include "Math.h"

class VertexNfg
{
	friend std::istream& operator>>(std::istream& is, VertexNfg& obj);
	friend std::ostream& operator<<(std::ostream& os, const VertexNfg& obj);
public:
	VertexNfg();
	VertexNfg(const VertexNfg& copy);
	Vector3 pos, binorm, norm, tgt, color;
	Vector2 uv, uv2;

	static void find_num(const GLchar* const buff, GLint& i);
	static GLfloat extr_num(const GLchar* const buff, GLint& i);
	static void read(const GLchar* const buff, GLint& i, Vector2& vec2);
	static void read(const GLchar* const buff, GLint& i, Vector3& vec3);
};