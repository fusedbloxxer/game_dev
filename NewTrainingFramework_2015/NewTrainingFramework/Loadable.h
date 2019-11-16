#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

class Loadable
{
public:
	virtual void load() = 0;
	virtual ~Loadable();
};