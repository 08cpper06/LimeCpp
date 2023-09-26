#pragma once


#include "RefCounter.hpp"


template <class Type>
class TSharedPtr : public TSmartPointerBase<Type> {
public:
	typedef typename std::remove_extent<Type>::type ElementType;

public:
	template <class UType>
	friend class TWeakPtr;

private:

	template <class UType>
	void MoveConstructFrom(TSharedPtr<UType>&& InRhs) noexcept
	{
		this->MyPtr = InRhs.MyPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
		InRhs.MyPtr = nullptr;
		InRhs.MyRefCounter = nullptr;
	}
	template <class UType>
	void CopyConstructFrom(const TSharedPtr<UType>& InRhs) noexcept
	{
		if (InRhs.MyRefCounter)
		{
			InRhs.MyRefCounter->IncRef();
		}
		this->MyPtr = InRhs.MyPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
	}

	template <class UType>
	void MoveConstructFrom(TSharedPtr<UType>&& InRhs, ElementType* InPtr) noexcept
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
	void CopyConstructFrom(const TSharedPtr<UType>& InRhs, ElementType* InPtr) noexcept
	{
		if (InRhs.MyRefCounter)
		{
			InRhs.MyRefCounter->IncRef();
		}
		this->MyPtr = InPtr;
		this->MyRefCounter = InRhs.MyRefCounter;
	}

public:
	TSharedPtr() noexcept {}

	TSharedPtr(std::nullptr_t) noexcept {}

	template <class UType>
	explicit TSharedPtr(UType* InPtr) noexcept
	{
		this->MyPtr = InPtr;
		this->MyRefCounter = new TRefCounter();
	}

	TSharedPtr(const TSharedPtr<Type>& InRhs) noexcept
	{
		this->CopyConstructFrom(InRhs);
	}

	template <class UType>
	TSharedPtr(const TSharedPtr<UType>& InRhs) noexcept
	{
		this->CopyConstructFrom<UType>(InRhs);
	}

	TSharedPtr(TSharedPtr<Type>&& InRhs) noexcept
	{
		this->MoveConstructFrom<Type>(std::move(InRhs));
	}

	template <class UType>
	TSharedPtr(TSharedPtr<UType>&& InRhs) noexcept
	{
		this->MoveConstructFrom<UType>(std::move(InRhs));
	}

	template <class UType>
	TSharedPtr(const TSharedPtr<UType>& InRhs, ElementType* InPtr) noexcept
	{
		this->CopyConstructFrom<UType>(InRhs, InPtr);
	}
	template <class UType>
	TSharedPtr(TSharedPtr<UType>&& InRhs, ElementType* InPtr) noexcept
	{
		this->MoveConstructFrom<UType>(std::move(InRhs), InPtr);
	}

	~TSharedPtr() noexcept
	{
		this->DecreaseRef();
	}

	void Swap(TWeakPtr<Type>& InRhs) noexcept
	{
		this->_Swap(InRhs);
	}
	template <class UType>
	void Swap(TWeakPtr<UType>& InRhs) noexcept
	{
		this->_Swap(InRhs);
	}

	void Reset() noexcept
	{
		TSharedPtr<Type>().Swap(*this);
	}

	Type* Get() const noexcept
	{
		return this->_Get();
	}

	long UseCount() const noexcept
	{
		return this->_UseCount();
	}

	operator const bool() const noexcept
	{
		return Get() != nullptr;
	}

	TSharedPtr<Type>& operator=(const TSharedPtr<Type>& InRhs) noexcept
	{
		this->CopyConstructFrom<Type>(InRhs);
		return *this;
	}
	template <class UType>
	TSharedPtr<Type>& operator=(const TSharedPtr<UType>& InRhs) noexcept
	{
		this->CopyConstructFrom<UType>(InRhs);
		return *this;
	}

	TSharedPtr<Type>& operator=(TSharedPtr<Type>&& InRhs) noexcept
	{
		this->MoveConstructFrom<Type>(std::move(InRhs));
		return *this;
	}
	template <class UType>
	TSharedPtr<Type>& operator=(TSharedPtr<UType>&& InRhs) noexcept
	{
		this->MoveConstructFrom(std::move(InRhs));
		return *this;
	}

public:
	Type& operator*() const noexcept
	{
		return *Get();
	}

	Type* operator->() const noexcept
	{
		return Get();
	}

};

template <class Type, class... Args>
inline TSharedPtr<Type> MakeShared(Args&&... InArgs) noexcept
{
	return TSharedPtr<Type>(new Type(InArgs...));
}
template <class Type>
inline TSharedPtr<Type> MakeShared() noexcept
{
	return TSharedPtr<Type>(new Type());
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

template <class Type>
class TSharedPtr<Type []>;