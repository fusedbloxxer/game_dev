#include "stdafx.h"
#include "Controls.h"
#include <iostream>
#include <string>

Controls atoc(const char* str)
{
	if (strcmp(str, "MOVE_CAMERA_POSITIVE_X") == 0)
	{
		return MOVE_CAMERA_POSITIVE_X;
	}
	else if (strcmp(str, "MOVE_CAMERA_NEGATIVE_X") == 0) {
		return MOVE_CAMERA_NEGATIVE_X;
	}
	else if (strcmp(str, "MOVE_CAMERA_POSITIVE_Y") == 0) {
		return MOVE_CAMERA_POSITIVE_Y;
	}
	else if (strcmp(str, "MOVE_CAMERA_NEGATIVE_Y") == 0) {
		return MOVE_CAMERA_NEGATIVE_Y;
	}
	else if (strcmp(str, "MOVE_CAMERA_POSITIVE_Z") == 0) {
		return MOVE_CAMERA_POSITIVE_Z;
	}
	else if (strcmp(str, "MOVE_CAMERA_NEGATIVE_Z") == 0) {
		return MOVE_CAMERA_NEGATIVE_Z;
	}
	else if (strcmp(str, "ROTATE_CAMERA_POSITIVE_X") == 0) {
		return ROTATE_CAMERA_POSITIVE_X;
	}
	else if (strcmp(str, "ROTATE_CAMERA_NEGATIVE_X") == 0) {
		return ROTATE_CAMERA_NEGATIVE_X;
	}
	else if (strcmp(str, "ROTATE_CAMERA_POSITIVE_Y") == 0) {
		return ROTATE_CAMERA_POSITIVE_Y;
	}
	else if (strcmp(str, "ROTATE_CAMERA_NEGATIVE_Y") == 0) {
		return ROTATE_CAMERA_NEGATIVE_Y;
	}
	else if (strcmp(str, "ROTATE_CAMERA_POSITIVE_Z") == 0) {
		return ROTATE_CAMERA_POSITIVE_Z;
	}
	else if (strcmp(str, "ROTATE_CAMERA_NEGATIVE_Z") == 0) {
		return ROTATE_CAMERA_NEGATIVE_Z;
	}
	else if (strcmp(str, "SCENE_WIREFRAME_TRUE") == 0) {
		return SCENE_WIREFRAME_TRUE;
	}
	else if (strcmp(str, "SCENE_WIREFRAME_FALSE") == 0) {
		return SCENE_WIREFRAME_FALSE;
	}
	else {
		std::cerr << "INVALID STRING-CONTROL CONVERSION: " << str << std::endl;
		abort();
	}
}
