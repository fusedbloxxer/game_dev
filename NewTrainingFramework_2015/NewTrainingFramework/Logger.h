#pragma once
#include "..\Utilities\utilities.h"

class Logger
{
	inline static int priority = 15;
public:
	static void wtf(const char* message, bool show = true);

	static void e(const char* message, bool show = true);

	static void d(const char* message, bool show = true);

	static void v(const GLchar* message, bool show = true);

	static void setMode(GLint priority);

	static GLint getMode();
private:
	static void setColor(GLint colorCode);
};