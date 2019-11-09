#include "stdafx.h"
#include "Vertex_NFG.h"

std::istream& operator>>(std::istream& is, Vertex_NFG& obj)
{
	is >> obj.pos.x >> obj.pos.y >> obj.pos.z;
	is >> obj.binorm.x >> obj.binorm.y >> obj.binorm.z;
	is >> obj.norm.x >> obj.norm.y >> obj.norm.z;
	is >> obj.tgt.x >> obj.tgt.y >> obj.tgt.z;
	is >> obj.uv.x >> obj.uv.y;

	return is;
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
	os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ']';
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
	os << '[' << vec.x << ", " << vec.y << ']';
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vertex_NFG& obj)
{
	os << "pos:" << obj.pos << "; norm:" << obj.norm << "; binorm:" << obj.binorm
		<< "; tgt:" << obj.tgt << "; uv:" << obj.uv;
	return os;
}

Vertex_NFG::Vertex_NFG()
{
}

Vertex_NFG::Vertex_NFG(const Vertex_NFG& copy)
{
	pos.x = copy.pos.x;
	pos.y = copy.pos.y;
	pos.z = copy.pos.z;

	binorm.x = copy.binorm.x;
	binorm.y = copy.binorm.y;
	binorm.z = copy.binorm.z;

	norm.x = copy.norm.x;
	norm.y = copy.norm.y;
	norm.z = copy.norm.z;

	tgt.x = copy.tgt.x;
	tgt.y = copy.tgt.y;
	tgt.z = copy.tgt.z;

	color.x = copy.color.x;
	color.y = copy.color.y;
	color.z = copy.color.z;

	uv.x = copy.uv.x;
	uv.y = copy.uv.y;
}
