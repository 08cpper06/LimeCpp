#pragma once


#include <cstddef>
#include <atomic>


template <class Type>
class TSharedPtr {
private:
	typedef typename std::remove_extent<Type>::type ElementType;

private:
	void DecreaseRefCount() noexcept
	{
		if (MyRefCount)
		{
			if (--(*MyRefCount) == 0)
			{
				delete MyRefCount;
				delete MyPtr;
			}
			MyPtr = nullptr;
			MyRefCount = nullptr;
		}
	}

	void SetPtr(Type* InPtr, std::atomic<std::size_t>* InCounter = nullptr) noexcept
	{
		DecreaseRefCount();
		MyPtr = InPtr;
		if (MyPtr)
		{
			MyRefCount = InCounter;
			if (MyRefCount)
			{
				++(*MyRefCount);
			}
		}
	}

public:
	TSharedPtr() noexcept
	{
		SetPtr(nullptr);
	}

	TSharedPtr(std::nullptr_t) noexcept
	{
		SetPtr(nullptr);
	}

	TSharedPtr(const TSharedPtr<Type>& InRhs)
	{
		SetPtr(reinterpret_cast<Type*>(InRhs.MyPtr), const_cast<std::atomic<std::size_t>*>(InRhs.MyRefCount));
	}

	template <
		class UType
	>
	TSharedPtr(const TSharedPtr<UType>& InRhs)
	{
		SetPtr(reinterpret_cast<Type*>(InRhs.MyPtr), const_cast<std::atomic<std::size_t>*>(InRhs.MyRefCount));
	}

	TSharedPtr(TSharedPtr<Type>&& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		InRhs.DecreaseRefCount();
	}

	template <
		class UType
	>
	TSharedPtr(TSharedPtr<Type>&& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		InRhs.DecreaseRefCount();
	}

	~TSharedPtr() noexcept
	{
		DecreaseRefCount();
	}

	template <
		class UType
	>
	TSharedPtr(UType* InPtr) noexcept
	{
		SetPtr(InPtr, new std::atomic<std::size_t>());
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
		SetPtr(reinterpret_cast<Type*>(InRhs.MyPtr), const_cast<std::atomic<std::size_t>*>(InRhs.MyRefCount));
		return *this;
	}
	template <
		class UType
	>
	TSharedPtr<Type>& operator=(const TSharedPtr<UType>& InRhs) noexcept
	{
		SetPtr(reinterpret_cast<Type*>(InRhs.MyPtr), const_cast<std::atomic<std::size_t>*>(InRhs.MyRefCount));
		return *this;
	}

	TSharedPtr<Type>& operator=(TSharedPtr<Type>&& InRhs) noexcept
	{
		SetPtr(InRhs.MyPtr, InRhs.MyRefCount);
		InRhs.DecreaseRefCount();
		return *this;
	}
	template <
		class UType
	>
	TSharedPtr<Type>& operator=(TSharedPtr<UType>&& InRhs) noexcept
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
	friend class TSharedPtr;
	std::atomic<std::size_t>* MyRefCount { nullptr };
	Type* MyPtr { nullptr };
};

template <class Type, class... Args>
inline TSharedPtr<Type> MakeShared(Args&&... InArgs)
{
	return new Type(InArgs...);
}
template <class Type>
inline TSharedPtr<Type> MakeShared()
{
	return new Type();
}

template <
	class UType,
	class Type,
	typename std::enable_if_t<std::is_base_of_v<Type, UType>, std::nullptr_t> = nullptr
>
inline TSharedPtr<UType> DynamicCast(TSharedPtr<Type> InPtr)
{
	return InPtr;
}

template <class Type>
class TSharedPtr<Type []>;