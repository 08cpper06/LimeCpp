#pragma once


#include "../Std.hpp"


enum class DefaultErrorType {
	NoError,
	Error,
};

struct None {};

template <class Type, class ErrorType = DefaultErrorType>
class TOption {
public:
	constexpr TOption() noexcept :
		MyValue(Type()),
		MyErrorCode(ErrorType(0))
	{}
	constexpr ~TOption() noexcept {}

	template <class... Args>
	constexpr TOption(Args&&... InArgs) noexcept :
		MyValue(InArgs...),
		MyErrorCode(ErrorType())
	{}

	constexpr TOption(const Type& InVal) noexcept :
		MyValue(InVal),
		MyErrorCode(ErrorType())
	{}

	constexpr TOption(Type&& InVal) noexcept :
		MyValue(InVal),
		MyErrorCode(ErrorType())
	{}

	constexpr TOption(const ErrorType& InErrorCode) noexcept :
		MyValue(Type()),
		MyErrorCode(InErrorCode)
	{}

	constexpr TOption(ErrorType&& InErrorCode) noexcept :
		MyValue(Type()),
		MyErrorCode(InErrorCode)
	{}

	operator bool() const noexcept
	{
		return size_t(MyErrorCode) == 0;
	}

	constexpr Type& operator*()
	{
		return MyValue;
	}

	constexpr const Type& operator*() const
	{
		return MyValue;
	}

	constexpr const Type* operator->() const
	{
		if constexpr (size_t(MyErrorCode))
		{
			return &MyValue;
		}
		return nullptr;
	}
	constexpr Type* operator->() noexcept
	{
		if constexpr (size_t(MyErrorCode))
		{
			return &MyValue;
		}
		return nullptr;
	}

	constexpr ErrorType GetStatus() const noexcept
	{
		return MyErrorCode;
	}

	constexpr void Reset() noexcept
	{
		MyValue = Type();
		MyErrorCode = ErrorType(0);
	}

CLASS_PRIVATE:
	Type MyValue;
	ErrorType MyErrorCode;
};