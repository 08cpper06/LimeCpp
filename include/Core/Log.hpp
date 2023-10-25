#pragma once


#include <cstdio>


enum class LimeLogType {
	Error,
	Log
};

namespace Lime {
	template <class... TArgs>
	void LimeLog(LimeLogType InLogType, const char8_t* InMessage, TArgs&&... InArgs)
	{
		switch (InLogType) {
		case LimeLogType::Error:
			std::fprintf(stderr, reinterpret_cast<const char*>(InMessage), InArgs...);
			break;
		case LimeLogType::Log:
			std::printf(reinterpret_cast<const char*>(InMessage), InArgs...);
			break;
		}
	}
}