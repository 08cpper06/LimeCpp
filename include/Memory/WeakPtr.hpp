#pragma once


#include "SharedPtr.hpp"


template <class Type>
class TWeakPtr : public TSmartPointerBase<Type> {
public:
	typedef typename std::remove_extent<Type>::type ElementType;

public:
	constexpr TWeakPtr() noexcept {}

	TWeakPtr(std::nullptr_t) noexcept {}

	~TWeakPtr() noexcept
	{
		this->DecreaseWeakRef();
	}

	template <class UType>
	TWeakPtr(const TSharedPtr<UType>& InRhs) noexcept
	{
		this->WeakConstructFrom(InRhs);
	}

	TWeakPtr(const TWeakPtr<Type>& InRhs) noexcept
	{
		this->WeakConstructFrom(InRhs);
	}
	template <class UType>
	TWeakPtr(const TWeakPtr<UType>& InRhs) noexcept
	{
		this->WeakConstructFrom(InRhs);
	}

	TWeakPtr(TWeakPtr<Type>&& InRhs) noexcept
	{
		this->MoveConstructFrom(InRhs);
	}
	template <class UType>
	TWeakPtr(TWeakPtr<UType>&& InRhs) noexcept
	{
		this->MoveConstructFrom(std::move(InRhs));
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

public:
	TWeakPtr<Type>& operator=(const TWeakPtr<Type>& InRhs) noexcept
	{
		TWeakPtr(InRhs).Swap(*this);
		return *this;
	}
	template <class UType>
	TWeakPtr<Type>& operator=(const TWeakPtr<UType>& InRhs) noexcept
	{
		TWeakPtr(InRhs).Swap(*this);
		return *this;
	}

	TWeakPtr<Type>& operator=(TWeakPtr<Type>&& InRhs) noexcept
	{
		TWeakPtr(std::move(InRhs)).Swap(*this);
		return *this;
	}
	template <class UType>
	TWeakPtr<Type>& operator=(TWeakPtr<UType>&& InRhs) noexcept
	{
		TWeakPtr(std::move(InRhs)).Swap(*this);
		return *this;
	}

	template <class UType>
	TWeakPtr<Type>& operator=(const TSharedPtr<UType>& InRhs) noexcept
	{
		TWeakPtr(InRhs).Swap(*this);
		return *this;
	}

	TSharedPtr<Type> Lock() const noexcept
	{
		TSharedPtr<Type> Ret;
		if (this->MyRefCounter && this->MyRefCounter->MyRefCount)
		{
			this->MyRefCounter->IncRef();
			Ret.MyPtr = this->MyPtr;
			Ret.MyRefCounter = this->MyRefCounter;
		}
		return Ret;
	}

	long UseCount() const noexcept
	{
		return this->_UseCount();
	}

	bool Expired() const noexcept
	{
		return UseCount() == 0;
	}

	void Reset() noexcept
	{
		TWeakPtr<Type>().Swap(*this);
	}

};