#pragma once


#include "RefCounter.hpp"
#include <source_location>
#include <type_traits>
#include <iostream>


template <class Type>
class TSharedPtr : public TSmartPointerBase<Type> {
public:
	typedef typename std::remove_extent<Type>::type ElementType;

public:
	template <class UType>
	friend class TWeakPtr;

#if defined(_DEBUG)
	std::source_location MyCreateLocation;
#endif

private:

	template <class UType>
	constexpr void MoveConstructFrom(TSharedPtr<UType>&& InRhs) noexcept
	{
		this->MyPtr = InRhs.MyPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
		InRhs.MyPtr = nullptr;
		InRhs.MyRefCounter = nullptr;
	}
	template <class UType>
	constexpr void CopyConstructFrom(const TSharedPtr<UType>& InRhs) noexcept
	{
		if (InRhs.MyRefCounter)
		{
			InRhs.MyRefCounter->IncRef();
		}
		this->MyPtr = InRhs.MyPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
	}

	template <class UType>
	constexpr void MoveConstructFrom(TSharedPtr<UType>&& InRhs, ElementType* InPtr) noexcept
	{
		if (InRhs.MyRefCounter)
		{
			InRhs.MyRefCounter->IncRef();
		}
		this->MyPtr = InPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
		InRhs.MyPtr = nullptr;
		InRhs.MyRefCounter = nullptr;
	}
	template <class UType>
	constexpr void CopyConstructFrom(const TSharedPtr<UType>& InRhs, ElementType* InPtr) noexcept
	{
		if (InRhs.MyRefCounter)
		{
			InRhs.MyRefCounter->IncRef();
		}
		this->MyPtr = InPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
	}

public:

#if defined(_DEBUG)
	constexpr TSharedPtr(std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{}

	constexpr TSharedPtr(std::nullptr_t null, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{}

	template <class UType>
	explicit TSharedPtr(UType* InPtr, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->MyPtr = InPtr;
		this->MyRefCounter = new TRefCounter();
	}

	constexpr TSharedPtr(const TSharedPtr<Type>& InRhs, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->CopyConstructFrom(InRhs);
	}

	template <class UType>
	constexpr TSharedPtr(const TSharedPtr<UType>& InRhs, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->CopyConstructFrom<UType>(InRhs);
	}

	constexpr TSharedPtr(TSharedPtr<Type>&& InRhs, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->MoveConstructFrom<Type>(std::move(InRhs));
	}

	template <class UType>
	constexpr TSharedPtr(TSharedPtr<UType>&& InRhs, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->MoveConstructFrom<UType>(std::move(InRhs));
	}

	template <class UType>
	constexpr TSharedPtr(const TSharedPtr<UType>& InRhs, ElementType* InPtr, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->CopyConstructFrom<UType>(InRhs, InPtr);
	}
	template <class UType>
	constexpr TSharedPtr(TSharedPtr<UType>&& InRhs, ElementType* InPtr, std::source_location InLoc = std::source_location::current()) noexcept :
		MyCreateLocation(InLoc)
	{
		this->MoveConstructFrom<UType>(std::move(InRhs), InPtr);
	}

#else
	constexpr TSharedPtr() noexcept {}

	constexpr TSharedPtr(std::nullptr_t) noexcept {}

	template <class UType>
	explicit TSharedPtr(UType* InPtr) noexcept
	{
		this->MyPtr = InPtr;
		this->MyRefCounter = new TRefCounter();
	}

	constexpr TSharedPtr(const TSharedPtr<Type>& InRhs) noexcept
	{
		this->CopyConstructFrom(InRhs);
	}

	template <class UType>
	constexpr TSharedPtr(const TSharedPtr<UType>& InRhs) noexcept
	{
		this->CopyConstructFrom<UType>(InRhs);
	}

	constexpr TSharedPtr(TSharedPtr<Type>&& InRhs) noexcept
	{
		this->MoveConstructFrom<Type>(std::move(InRhs));
	}

	template <class UType>
	constexpr TSharedPtr(TSharedPtr<UType>&& InRhs) noexcept
	{
		this->MoveConstructFrom<UType>(std::move(InRhs));
	}

	template <class UType>
	constexpr TSharedPtr(const TSharedPtr<UType>& InRhs, ElementType* InPtr) noexcept
	{
		this->CopyConstructFrom<UType>(InRhs, InPtr);
	}
	template <class UType>
	constexpr TSharedPtr(TSharedPtr<UType>&& InRhs, ElementType* InPtr) noexcept
	{
		this->MoveConstructFrom<UType>(std::move(InRhs), InPtr);
	}

#endif

	constexpr ~TSharedPtr() noexcept
	{
		this->DecreaseRef();
	}

	constexpr void Swap(TWeakPtr<Type>& InRhs) noexcept
	{
		this->_Swap(InRhs);
	}
	template <class UType>
	constexpr void Swap(TWeakPtr<UType>& InRhs) noexcept
	{
		this->_Swap(InRhs);
	}

	constexpr void Swap(TSharedPtr<Type>& InRhs) noexcept
	{
		this->_Swap(InRhs);
	}
	template <class UType>
	constexpr void Swap(TSharedPtr<UType>& InRhs) noexcept
	{
		this->_Swap(InRhs);
	}

	constexpr void Reset() noexcept
	{
		TSharedPtr<Type>().Swap(*this);
	}

	constexpr Type* Get() const noexcept
	{
		return this->_Get();
	}

	constexpr long UseCount() const noexcept
	{
		return this->_UseCount();
	}

	constexpr operator const bool() const noexcept
	{
		return Get() != nullptr;
	}

	constexpr TSharedPtr<Type>& operator=(const TSharedPtr<Type>& InRhs) noexcept
	{
		this->CopyConstructFrom<Type>(InRhs);
		return *this;
	}
	template <class UType>
	constexpr TSharedPtr<Type>& operator=(const TSharedPtr<UType>& InRhs) noexcept
	{
		this->CopyConstructFrom<UType>(InRhs);
		return *this;
	}

	constexpr TSharedPtr<Type>& operator=(TSharedPtr<Type>&& InRhs) noexcept
	{
		this->MoveConstructFrom<Type>(std::move(InRhs));
		return *this;
	}
	template <class UType>
	constexpr TSharedPtr<Type>& operator=(TSharedPtr<UType>&& InRhs) noexcept
	{
		this->MoveConstructFrom(std::move(InRhs));
		return *this;
	}

public:

	template <
		std::enable_if_t<!std::is_array_v<Type>, std::nullptr_t> = nullptr
	>
	constexpr Type& operator*() const noexcept
	{
		Assert(Get(), MyCreateLocation);
		return *Get();
	}

	template <
		std::enable_if_t<!std::is_array_v<Type>, std::nullptr_t> = nullptr
	>
	constexpr Type* operator->() const noexcept
	{
		Assert(Get(), MyCreateLocation);
		return Get();
	}
	
	template <
		class UType = Type,
		std::enable_if_t<std::is_array_v<UType>, std::nullptr_t> = nullptr
	>
	constexpr Type& operator[](ptrdiff_t InIndex) const noexcept
	{
		Assert(Get(), MyCreateLocation);
		return Get()[InIndex];
	}

};

template <class Type, class... Args>
inline std::enable_if_t<!std::is_array_v<Type>, TSharedPtr<Type>> MakeShared(Args&&... InArgs) noexcept
{
	return TSharedPtr<Type>(new Type(InArgs...));
}
template <class Type>
inline std::enable_if_t<!std::is_array_v<Type>, TSharedPtr<Type>> MakeShared(std::source_location InLoc = std::source_location::current()) noexcept
{
#if defined(_DEBUG)
	return TSharedPtr<Type>(new Type(), InLoc);
#else
	return TSharedPtr<Type>(new Type());
#endif
}
template <class Type>
inline std::enable_if_t<std::is_array_v<Type>, TSharedPtr<Type>> MakeShared(std::size_t InCount, std::source_location InLoc = std::source_location::current()) noexcept
{
#if defined(_DEBUG)
	return TSharedPtr<Type[]>(new Type[InCount], InLoc);
#else
	return TSharedPtr<Type[]>(new Type[InCount]);
#endif
}

template <
	class UType,
	class Type,
	typename std::enable_if_t<std::is_base_of_v<Type, UType>, std::nullptr_t> = nullptr
>
inline TSharedPtr<UType> DynamicCast(TSharedPtr<Type> InPtr) noexcept
{
	const auto Ptr = dynamic_cast<typename TSharedPtr<UType>::ElementType*>(InPtr.Get());
	if (Ptr)
	{
		return TSharedPtr<UType>(InPtr, Ptr);
	}
	return TSharedPtr<UType>();
}

template <
	class UType,
	class Type,
	typename std::enable_if_t<std::is_base_of_v<Type, UType>, std::nullptr_t> = nullptr
>
inline TSharedPtr<UType> StaticCast(TSharedPtr<Type> InPtr, std::source_location InLoc = std::source_location::current()) noexcept
{
	const auto Ptr = static_cast<typename TSharedPtr<UType>::ElementType*>(InPtr.Get());
#if defined(_DEBUG)
	if (Ptr)
	{
		return TSharedPtr<UType>(InPtr, Ptr, InLoc);
	}
	return TSharedPtr<UType>(InLoc);
#else
	if (Ptr)
	{
		return TSharedPtr<UType>(InPtr, Ptr);
	}
	return TSharedPtr<UType>();
#endif
}

template <
	class UType,
	class Type
>
inline TSharedPtr<UType> ConstCast(TSharedPtr<Type> InPtr, std::source_location InLoc = std::source_location::current()) noexcept
{
	const typename TSharedPtr<UType>::ElementType* Ptr = const_cast<typename TSharedPtr<UType>::ElementType*>(InPtr.Get());
#if defined(_DEBUG)
	return TSharedPtr<UType>(InPtr, Ptr, InLoc);
#else
	return TSharedPtr<UType>(InPtr, Ptr);
#endif
}
