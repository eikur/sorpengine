#pragma once

#include <windows.h>
#include <stdio.h>
#include "Globals.hpp"

namespace
{
	void _log(const char file[], int line, const char* format, ...)
	{
		static char tmp_string[4096];
		static char tmp_string2[4096];
		static va_list  ap;

		va_start(ap, format);
		vsprintf_s(tmp_string, 4096, format, ap);
		va_end(ap);
		sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
		OutputDebugString(tmp_string2);
	}
}

namespace Utils
{
	template<typename... Args>
	void log(const char* format, Args...args)
	{
		_log(__FILE__, __LINE__, format, args... );
	}

	
}