#pragma once

enum Controls
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
	SCENE_WIREFRAME_FALSE
};

Controls atoc(const char* str);

unsigned char atok(const char* str);