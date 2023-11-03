#pragma once

#include <source_location>
#include <iostream>



#if defined(_DEBUG)
inline void Assert(bool InExpr, std::source_location InLoc = std::source_location::current())
{
	if (!InExpr)
	{
		std::cout << "Assertion error: " << InLoc.line() << "[" << InLoc.file_name() << "]" << std::endl;
		exit(1);
	}
}

#else

#define Assert(...)

#endif
