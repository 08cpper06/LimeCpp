#pragma once


#include <cstddef>


template <class Type>
class TSharedPtr {
private:
	typedef typename std::remove_extent<Type>::type ElementType;

private:
	void DecreaseRefCount() noexcept
	{
		if (MyRefCount)
		{
			if (--(*MyRefCount))
			{
				delete MyRefCount;
				delete MyPtr;
				std::exchange(MyPtr, nullptr);
				std::exchange(MyRefCount, nullptr);
			}
		}
	}

	void SetPtr(Type* InPtr, std::size_t* Counter = nullptr) noexcept
	{
		DecreaseRefCount();
		MyPtr = InPtr;
		if (MyPtr)
		{
			if (Counter)
			{
				MyRefCount = new std::size_t(0);
			}
			++(*MyRefCount);
		}
	}

public:
	TSharedPtr() noexcept
	{
		SetPtr(nullptr);
	}

	TSharedPtr(const TSharedPtr<Type>& InRhs)
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
	}

	template <
		class UType,
		typename std::enable_if<std::is_convertible_v<Type, UType>, std::nullptr_t>::type = nullptr
	>
	TSharedPtr(const TSharedPtr<UType>& InRhs)
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
	}

	TSharedPtr(TSharedPtr<Type>&& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		InRhs.DecreaseRefCount();
	}

	template <
		class UType,
		typename std::enable_if<std::is_convertible_v<Type, UType>, std::nullptr_t>::type = nullptr
	>
	TSharedPtr(TSharedPtr<Type>&& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		InRhs.DecreaseRefCount();
	}

	~TSharedPtr() noexcept
	{
		SetPtr(nullptr);
	}

	template <
		class UType,
		typename std::enable_if<std::is_convertible_v<Type, UType>, std::nullptr_t>::type = nullptr
	>
	TSharedPtr(UType* InPtr) noexcept
	{
		SetPtr(InPtr);
	}

	void Reset() noexcept
	{
		DecreaseRefCount();
	}

	Type* Get() const noexcept
	{
		return MyPtr;
	}

	std::size_t UseCount() const noexcept
	{
		return *MyRefCount;
	}

public:
	operator const bool() const noexcept
	{
		return MyPtr;
	}

public:
	TSharedPtr<Type>& operator=(const TSharedPtr<Type>& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		return *this;
	}
	TSharedPtr<Type>& operator=(TSharedPtr<Type>&& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		InRhs.DecreaseRefCount();
		return *this;
	}

public:
	Type& operator*() const
	{
		return *Get();
	}

	Type* operator->() const noexcept
	{
		return Get();
	}

private:
	std::size_t* MyRefCount;
	Type* MyPtr;
};

template <class Type, class... Args>
inline TSharedPtr<Type> MakeShared(Args&&... InArgs)
{
	return new Type(InArgs...);
}

template <class Type>
class TSharedPtr<Type []>;