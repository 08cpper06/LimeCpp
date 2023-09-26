#pragma once


#include <algorithm>
#include <cstdio>
#include <vector>
#include <span>
#include <map>
#include <memory>
#include <list>
#include <set>

/* non-override */
#include <initializer_list>
#include <type_traits>


#include "Memory/SharedPtr.hpp"
#include "Memory/WeakPtr.hpp"


enum class LimeLogType {
	Error,
	Log
};

#if defined(_DEBUG)
#define CLASS_PRIVATE public
#else
#define CLASS_PRIVATE private
#endif

namespace Lime {

	typedef std::size_t size_t;
	typedef std::ptrdiff_t ptrdiff_t;

	template <class Type, class TAllocator = std::allocator<Type>>
	using TArray = std::vector<Type, TAllocator>;

	template <class Type, Lime::size_t Extent = std::dynamic_extent>
	using TSpan = std::span<Type, Extent>;

	template <class Key, class Value>
	using TMap = std::map<Key, Value>;

	template <class Type>
	using TList = std::list<Type>;

	template <class Type>
	using TSet = std::set<Type>;

	template <class... TArgs>
	void LimeLog(LimeLogType InLogType, const char8_t* InMessage, TArgs&&... InArgs)
	{
		switch (InLogType) {
		case LimeLogType::Error:
			exit(1);
			std::fprintf(stderr, reinterpret_cast<const char*>(InMessage), InArgs...);
			break;
		case LimeLogType::Log:
			std::printf(reinterpret_cast<const char*>(InMessage), InArgs...);
			break;
		}
	}

}

#define LIME_ERROR(Message, ...) do { Lime::LimeLog(LimeLogType::Error, u8"[Line:%d]", __LINE__); \
	Lime::LimeLog(LimeLogType::Error, u8"%s", Message, __VA_ARGS__); \
	Lime::LimeLog(LimeLogType::Error, u8"(%s)\n", __LINE__, Message, __FILE__); \
} while(false)
