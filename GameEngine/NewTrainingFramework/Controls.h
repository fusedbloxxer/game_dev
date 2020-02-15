#pragma once
#include "..\Utilities\utilities.h"
#include <unordered_map>

namespace Controls {
	enum class Type
	{
		MOVE_CAMERA_POSITIVE_X,
		MOVE_CAMERA_NEGATIVE_X,
		MOVE_CAMERA_POSITIVE_Y,
		MOVE_CAMERA_NEGATIVE_Y,
		MOVE_CAMERA_POSITIVE_Z,
		MOVE_CAMERA_NEGATIVE_Z,

		ROTATE_CAMERA_POSITIVE_X,
		ROTATE_CAMERA_NEGATIVE_X,
		ROTATE_CAMERA_POSITIVE_Y,
		ROTATE_CAMERA_NEGATIVE_Y,
		ROTATE_CAMERA_POSITIVE_Z,
		ROTATE_CAMERA_NEGATIVE_Z,

		SCENE_WIREFRAME_TRUE,
		SCENE_WIREFRAME_FALSE,

		NEXT_CAMERA,
		PREVIOUS_CAMERA,

		MODE_DEBUG
	};

	Type atoc(const char* str);
	const char* ctoa(const Type& type);

	GLubyte atok(const char* str);
}