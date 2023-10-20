#pragma once


#include <algorithm>
#include <cstdio>
#include <vector>
#include <span>
#include <map>
#include <memory>
#include <list>
#include <set>
#include <variant>
#include <mutex>
#include <functional>
#include <stack>

/* non-override */
#include <initializer_list>
#include <type_traits>
#include <source_location>


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

#define CONSOLE_COLOR(R, G, B) "\x1b[38;2;" #R ";" #G ";" #B "m"
#define DEFAULT_COLOR CONSOLE_COLOR(200, 200, 200)

namespace Lime {

	typedef std::size_t size_t;
	typedef std::ptrdiff_t ptrdiff_t;

	template <class Type, class TAllocator = std::allocator<Type>>
	using TArray = std::vector<Type, TAllocator>;

	template <class... TArgs>
	using TTuple = std::tuple<TArgs...>;

	template <class Type, Lime::size_t Extent = std::dynamic_extent>
	using TSpan = std::span<Type, Extent>;

	template <class Key, class Value>
	using TMap = std::map<Key, Value>;

	template <class First, class Second>
	using TPair = std::pair<First, Second>;

	template <class Type>
	using TList = std::list<Type>;

	template <class Type>
	using TSet = std::set<Type>;

	template <class... Types>
	using TVariant = std::variant<Types...>;

	template <class Type>
	using TStack = std::stack<Type>;

	template <class... TArgs>
	void LimeLog(LimeLogType InLogType, const char8_t* InMessage, TArgs&&... InArgs)
	{
		switch (InLogType) {
		case LimeLogType::Error:
			std::fprintf(stderr, reinterpret_cast<const char*>(InMessage), InArgs...);
			exit(1);
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
