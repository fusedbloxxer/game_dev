#include "stdafx.h"
#include "Logger.h"
#include <iostream>
#include <windows.h>

void Logger::setMode(GLint priority)
{
	if (priority < 0 || priority > 15)
	{
		throw std::runtime_error{ "Wrong priority mode." };
	}

	Logger::priority = priority;
}

GLint Logger::getMode()
{
	return Logger::priority;
}

void Logger::setColor(GLint colorCode)
{
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colorCode);
}