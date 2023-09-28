#pragma once


#include <atomic>


template <class Type>
class TSharedPtr;


class TRefCounter {
public:
	TRefCounter(const TRefCounter&) = delete;
	TRefCounter& operator=(const TRefCounter&) = delete;

	constexpr TRefCounter() noexcept = default;
	~TRefCounter() = default;

	void IncRef() noexcept
	{
		++MyRefCount;
	}
	void IncWeakRef() noexcept
	{
		++MyWeakRefCount;
	}
	bool DecRef() noexcept
	{
		if ((--MyRefCount) == 0)
		{
			DecWeakRef();
			return true;
		}
		return false;
	}
	bool DecWeakRef() noexcept
	{
		return --MyWeakRefCount == 0;
	}

	bool IsDestroyMe() const noexcept
	{
		return MyRefCount.load() == 0 && MyWeakRefCount.load() == 0;
	}

public:
	std::atomic<long> MyRefCount{ 1 };
	std::atomic<long> MyWeakRefCount { 1 };

};

template <class Type>
class TSmartPointerBase {
public:
	typedef typename std::remove_extent<Type>::type ElementType;

	template <class UType>
	friend class TSharedPtr;

	template <class UType>
	friend class TWeakPtr;

	TSmartPointerBase(const TSmartPointerBase&) = delete;
	TSmartPointerBase& operator=(const TSmartPointerBase&) = delete;

	virtual void Destroy() noexcept
	{
		if constexpr (std::is_array_v<Type>)
		{
			delete[] this->MyPtr;
		}
		else
		{
			delete this->MyPtr;
		}
		MyPtr = nullptr;
	}
protected:
	void DecreaseWeakRef() noexcept
	{
		if (MyRefCounter)
		{
			if (MyRefCounter->DecWeakRef())
			{
				delete std::exchange(MyRefCounter, nullptr);
			}
			MyPtr = nullptr;
			MyRefCounter = nullptr;
		}
	}

	void DecreaseRef() noexcept
	{
		if (MyRefCounter)
		{
			if (MyRefCounter->DecRef())
			{
				Destroy();
				DecreaseWeakRef();
			}
			MyPtr = nullptr;
			MyRefCounter = nullptr;
		}
	}
	void SetPtr(TRefCounter* InRef) noexcept
	{
		DecreaseRef();
		MyRefCounter = InRef;
		if (MyRefCounter)
		{
			MyRefCounter->IncRef();
		}
	}

protected:
	constexpr TSmartPointerBase() = default;
	~TSmartPointerBase() = default;

	ElementType* _Get() const noexcept
	{
		return MyPtr;
	}

	long _UseCount() const noexcept
	{
		return MyRefCounter ? MyRefCounter->MyRefCount.load() : 0;
	}

	void _Swap(TSmartPointerBase<Type>& InRhs) noexcept
	{
		std::swap(MyPtr, InRhs.MyPtr);
		std::swap(MyRefCounter, InRhs.MyRefCounter);
	}
	template <class UType>
	void _Swap(TSmartPointerBase<UType>& InRhs) noexcept
	{
		std::swap(MyPtr, InRhs.MyPtr);
		std::swap(MyRefCounter, InRhs.MyRefCounter);
	}

protected:
	template <class UType>
	void MoveConstructFrom(TSmartPointerBase<UType>&& InRhs) noexcept
	{
		MyPtr = InRhs.MyPtr;
		MyRefCounter = InRhs.MyRefCounter;

		InRhs.MyPtr = nullptr;
		InRhs.MyRefCounter = nullptr;
	}

	template <class UType>
	void WeakConstructFrom(const TSmartPointerBase<UType>& InRhs) noexcept
	{
		if (InRhs.MyRefCounter)
		{
			MyPtr = InRhs.MyPtr;
			MyRefCounter = InRhs.MyRefCounter;
			MyRefCounter->IncWeakRef();
		}
	}

protected:
	TRefCounter* MyRefCounter { nullptr };
	ElementType* MyPtr { nullptr };
};