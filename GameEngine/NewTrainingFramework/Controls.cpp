#include "stdafx.h"
#include "..\Utilities\utilities.h"
#include "Controls.h"
#include <iostream>
#include <string>

Controls::Type Controls::atoc(const char* str)
{
	if (strcmp(str, "MOVE_CAMERA_POSITIVE_X") == 0)
	{
		return Controls::Type::MOVE_CAMERA_POSITIVE_X;
	}
	else if (strcmp(str, "MOVE_CAMERA_NEGATIVE_X") == 0) {
		return Controls::Type::MOVE_CAMERA_NEGATIVE_X;
	}
	else if (strcmp(str, "MOVE_CAMERA_POSITIVE_Y") == 0) {
		return Controls::Type::MOVE_CAMERA_POSITIVE_Y;
	}
	else if (strcmp(str, "MOVE_CAMERA_NEGATIVE_Y") == 0) {
		return Controls::Type::MOVE_CAMERA_NEGATIVE_Y;
	}
	else if (strcmp(str, "MOVE_CAMERA_POSITIVE_Z") == 0) {
		return Controls::Type::MOVE_CAMERA_POSITIVE_Z;
	}
	else if (strcmp(str, "MOVE_CAMERA_NEGATIVE_Z") == 0) {
		return Controls::Type::MOVE_CAMERA_NEGATIVE_Z;
	}
	else if (strcmp(str, "ROTATE_CAMERA_POSITIVE_X") == 0) {
		return Controls::Type::ROTATE_CAMERA_POSITIVE_X;
	}
	else if (strcmp(str, "ROTATE_CAMERA_NEGATIVE_X") == 0) {
		return Controls::Type::ROTATE_CAMERA_NEGATIVE_X;
	}
	else if (strcmp(str, "ROTATE_CAMERA_POSITIVE_Y") == 0) {
		return Controls::Type::ROTATE_CAMERA_POSITIVE_Y;
	}
	else if (strcmp(str, "ROTATE_CAMERA_NEGATIVE_Y") == 0) {
		return Controls::Type::ROTATE_CAMERA_NEGATIVE_Y;
	}
	else if (strcmp(str, "ROTATE_CAMERA_POSITIVE_Z") == 0) {
		return Controls::Type::ROTATE_CAMERA_POSITIVE_Z;
	}
	else if (strcmp(str, "ROTATE_CAMERA_NEGATIVE_Z") == 0) {
		return Controls::Type::ROTATE_CAMERA_NEGATIVE_Z;
	}
	else if (strcmp(str, "SCENE_WIREFRAME_TRUE") == 0) {
		return Controls::Type::SCENE_WIREFRAME_TRUE;
	}
	else if (strcmp(str, "SCENE_WIREFRAME_FALSE") == 0) {
		return Controls::Type::SCENE_WIREFRAME_FALSE;
	}
	else if (strcmp(str, "NEXT_CAMERA") == 0) {
		return Controls::Type::NEXT_CAMERA;
	}
	else if (strcmp(str, "PREVIOUS_CAMERA") == 0) {
		return Controls::Type::PREVIOUS_CAMERA;
	}
	else if (strcmp(str, "MODE_DEBUG") == 0) {
		return Controls::Type::MODE_DEBUG;
	}
	else {
		throw std::runtime_error{ "INVALID STRING-CONTROL CONVERSION" };
	}
}

const char* Controls::ctoa(const Type& type)
{
	switch (type)
	{
	case Controls::Type::MOVE_CAMERA_NEGATIVE_X: return "MOVE_CAMERA_NEGATIVE_X";
	case Controls::Type::MOVE_CAMERA_NEGATIVE_Y: return "MOVE_CAMERA_NEGATIVE_Y";
	case Controls::Type::MOVE_CAMERA_NEGATIVE_Z: return "MOVE_CAMERA_NEGATIVE_Z";
	case Controls::Type::MOVE_CAMERA_POSITIVE_X: return "MOVE_CAMERA_POSITIVE_X";
	case Controls::Type::MOVE_CAMERA_POSITIVE_Y: return "MOVE_CAMERA_POSITIVE_Y";
	case Controls::Type::MOVE_CAMERA_POSITIVE_Z: return "MOVE_CAMERA_POSITIVE_Z";
	case Controls::Type::ROTATE_CAMERA_POSITIVE_X: return "ROTATE_CAMERA_POSITIVE_X";
	case Controls::Type::ROTATE_CAMERA_POSITIVE_Y: return "ROTATE_CAMERA_POSITIVE_Y";
	case Controls::Type::ROTATE_CAMERA_POSITIVE_Z: return "ROTATE_CAMERA_POSITIVE_Z";
	case Controls::Type::ROTATE_CAMERA_NEGATIVE_X: return "ROTATE_CAMERA_NEGATIVE_X";
	case Controls::Type::ROTATE_CAMERA_NEGATIVE_Y: return "ROTATE_CAMERA_NEGATIVE_Y";
	case Controls::Type::ROTATE_CAMERA_NEGATIVE_Z: return "ROTATE_CAMERA_NEGATIVE_Z";
	case Controls::Type::SCENE_WIREFRAME_FALSE: return "SCENE_WIREFRAME_FALSE";
	case Controls::Type::SCENE_WIREFRAME_TRUE: return "SCENE_WIREFRAME_TRUE";
	case Controls::Type::NEXT_CAMERA: return "NEXT_CAMERA";
	case Controls::Type::PREVIOUS_CAMERA: return "PREVIOUS_CAMERA";
	case Controls::Type::MODE_DEBUG: return "MODE_DEBUG";
	default: throw std::invalid_argument{ "Invalidd controls enum type." };
	}
}

GLubyte Controls::atok(const char* str)
{
	if (strcmp(str, "VK_LEFT") == 0) {
		return VK_LEFT;
	}
	else if (strcmp(str, "VK_RIGHT") == 0) {
		return VK_RIGHT;
	}
	else if (strcmp(str, "VK_UP") == 0) {
		return VK_UP;
	}
	else if (strcmp(str, "VK_DOWN") == 0) {
		return VK_DOWN;
	}
	else if (strlen(str) > 0) {
		return str[0];
	}
	else {
		throw std::runtime_error{ "COULD NOT CONVERT STRING-KEY" };
	}
}
