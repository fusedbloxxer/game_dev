#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include <iostream>

struct pos2D {
	GLfloat x, y;
};

struct pos3D {
	GLfloat x, y, z;
};

struct pos {
	GLint x, y, z;
};

class NFG
{
	friend std::istream& operator>>(std::istream& is, NFG& obj);
	friend std::ostream& operator<<(std::ostream& os, const NFG& obj);
	pos3D pos, norm, binorm, tgt;
	pos2D uv;
public:
	NFG(pos3D pos = { 0.0f, 0.0f, 0.0f }, pos3D norm = { 0.0f, 0.0f, 0.0f }, pos3D binorm = { 0.0f, 0.0f, 0.0f }, pos3D tgt = { 0.0f, 0.0f, 0.0f }, pos2D uv = { 0.0f, 0.0f });

	pos3D getPos() const;
	pos3D getNorm() const;
	pos3D getBinorm() const;
	pos3D getTgt() const;
	pos2D getUv() const;

	void setPos(pos3D pos);
	void setNorm(pos3D norm);
	void setBinorm(pos3D binorm);
	void setTgt(pos3D tgt);
	void setUv(pos2D uv);
};

std::istream& operator>>(std::istream& is, pos3D& obj);

std::istream& operator>>(std::istream& is, pos& obj);
std::ostream& operator<<(std::ostream& os, const pos& obj);