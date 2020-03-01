#include "stdafx.h"
#include "Trigger.h"

std::string Trigger::inputToOutput(const GLint& input)
{
	if (input == Trigger::Type::MOVE_UP)
	{
		return "MOVE_UP";
	}
	else if (input == Trigger::Type::MOVE_DOWN)
	{
		return "MOVE_DOWN";
	}
	else if (input == Trigger::Type::MOVE_LEFT)
	{
		return "MOVE_LEFT";
	}
	else if (input == Trigger::Type::MOVE_RIGHT)
	{
		return "MOVE_RIGHT";
	}
	else if (input == Trigger::Type::MAIN_AREA_BACKGROUND)
	{
		return "MAIN_AREA_BACKGROUND";
	}
	else if (input == Trigger::Type::OBJECT_COLLISION)
	{
		return "OBJECT_COLLISION";
	}
	else
	{
		throw std::runtime_error{ "Invalid Trigger::Type - std::string conversion." };
	}
}

GLint Trigger::outputToInput(const std::string& output)
{
	if (output == "MOVE_UP")
	{
		return Type::MOVE_UP;
	}
	else if (output == "MOVE_DOWN")
	{
		return Type::MOVE_DOWN;
	}
	else if (output == "MOVE_LEFT")
	{
		return Type::MOVE_LEFT;
	}
	else if (output == "MOVE_RIGHT")
	{
		return Type::MOVE_RIGHT;
	}
	else if (output == "MAIN_AREA_BACKGROUND")
	{
		return Type::MAIN_AREA_BACKGROUND;
	}
	else if (output == "OBJECT_COLLISION")
	{
		return Type::OBJECT_COLLISION;
	}
	else
	{
		throw std::runtime_error{ "Invalid std::string - Trigger::Type conversion." };
	}
}
