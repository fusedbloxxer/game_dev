#include "stdafx.h"
#include "NFG.h"
#include <string>

NFG::NFG(pos3D pos, pos3D norm, pos3D binorm, pos3D tgt, pos2D uv)
	:pos{ pos }, norm{ norm }, binorm{ binorm }, tgt{ tgt }, uv{ uv }
{
}

pos3D NFG::getPos() const
{
	return pos;
}

pos3D NFG::getNorm() const
{
	return norm;
}

pos3D NFG::getBinorm() const
{
	return binorm;
}

pos3D NFG::getTgt() const
{
	return tgt;
}

pos2D NFG::getUv() const
{
	return uv;
}

void NFG::setPos(pos3D pos)
{
	this->pos = pos;
}

void NFG::setNorm(pos3D norm)
{
	this->norm = norm;
}

void NFG::setBinorm(pos3D binorm)
{
	this->binorm = binorm;
}

void NFG::setTgt(pos3D tgt)
{
	this->tgt = tgt;
}

void NFG::setUv(pos2D uv)
{
	this->uv = uv;
}

std::istream& operator>>(std::istream& is, pos3D& obj)
{
	is >> obj.x >> obj.y >> obj.z;
	return is;
}

std::istream& operator>>(std::istream& is, pos& obj)
{
	is >> obj.x >> obj.y >> obj.z;
	return is;
}

std::ostream& operator<<(std::ostream& os, const pos& obj)
{
	os << '[' << obj.x << ", " << obj.y << ", " << obj.z << ']';
	return os;
}

std::istream& operator>>(std::istream& is, pos2D& obj)
{
	is >> obj.x >> obj.y;
	return is;
}

std::ostream& operator<<(std::ostream& os, const pos3D& obj)
{
	os << '[' << obj.x << ", " << obj.y << ", " << obj.z << ']';
	return os;
}

std::ostream& operator<<(std::ostream& os, const pos2D& obj)
{
	os << '[' << obj.x << ", " << obj.y << ']';
	return os;
}

std::istream& operator>>(std::istream& is, NFG& obj)
{
	is >> obj.pos >> obj.norm >> obj.binorm >> obj.tgt >> obj.uv;
	return is;
}

std::ostream& operator<<(std::ostream& os, const NFG& obj)
{
	os << "pos:" << obj.pos << "; norm:" << obj.norm << "; binorm:" << obj.binorm
		<< "; tgt:" << obj.tgt << "; uv:" << obj.uv;
	return os;
}