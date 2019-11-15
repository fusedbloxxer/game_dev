#pragma once

using GLint = int;

class Loadable
{
public:
	virtual void load() = 0;
	virtual ~Loadable();
};