#pragma once
#include "..\Utilities\utilities.h"
#include "ObjectConverter.h"
#include <string>

class Trigger : public ObjectConverter<GLint, std::string>
{
	Trigger() {}

public:
	enum Type
	{
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,

		MAIN_AREA_BACKGROUND,

		OBJECT_COLLISION
	};

	static std::string inputToOutput(const GLint& input);
	static GLint outputToInput(const std::string& output);
};