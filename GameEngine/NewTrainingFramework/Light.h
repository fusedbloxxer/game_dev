#pragma once
#include "..\Utilities\utilities.h"
#include "I_Printable.h"

class Light : public I_Printable
{
public:
	virtual ~Light() = 0;
};