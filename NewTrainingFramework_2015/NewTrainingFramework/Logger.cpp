#include "stdafx.h"
#include "Logger.h"
#include <iostream>
#include <windows.h>

/*
The different color codes are

0   BLACK
1   BLUE
2   GREEN
3   CYAN
4   RED
5   MAGENTA
6   BROWN
7   LIGHTGRAY
8   DARKGRAY
9   LIGHTBLUE
10  LIGHTGREEN
11  LIGHTCYAN
12  LIGHTRED
13  LIGHTMAGENTA
14  YELLOW
15  WHITE
*/

// BIT OPERATIONS: verbose, debug, error, what a terrible failure
//                    3       2      1          0 

void Logger::wtf(const char* message, bool show)
{
	if (show && (priority & 1) != 0)
	{
		Logger::setColor(4);
		std::cout << "WTF: " << message << '\n';
		Logger::setColor(15);
	}
}

void Logger::e(const char* message, bool show)
{
	if (show && (priority & (1 << 1)) != 0)
	{
		Logger::setColor(2);
		std::cout << "ERROR: " << message << '\n';
		Logger::setColor(15);
	}
}

void Logger::d(const char* message, bool show)
{
	if (show && (priority & (1 << 2)) != 0)
	{
		Logger::setColor(1);
		std::cout << "DEBUG: " << message << '\n';
		Logger::setColor(15);
	}
}

void Logger::v(const GLchar* message, bool show)
{
	if (show && (priority & (1 << 3)) != 0)
	{
		Logger::setColor(14);
		std::cout << "VERBOSE: " << message << '\n';
		Logger::setColor(15);
	}
}

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