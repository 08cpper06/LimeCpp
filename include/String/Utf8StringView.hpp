#pragma once


#include "BasicString.hpp"


class TUtf8StringView : public TBasicStringView {
public:
	class TConstIterator {
	public:
		constexpr TConstIterator() noexcept :
			MyHead(nullptr),
			MyCurrent(nullptr),
			MyEnd(nullptr)
		{}

		constexpr TConstIterator(const char8_t* InHead, char8_t* InCurrent, const char8_t* InEnd) :
			MyHead(InHead),
			MyCurrent(InCurrent),
			MyEnd(InEnd)
		{}

		constexpr ~TConstIterator() noexcept = default;

		constexpr TChar operator*()
		{
			return TChar(MyCurrent);
		}
		constexpr const TChar& operator*() const
		{
			return TChar(MyCurrent);
		}

		constexpr operator const bool() const
		{
			return MyCurrent != MyEnd;
		}

		constexpr TConstIterator& operator++()
		{
			MyCurrent += TChar::CharSize(*MyCurrent);
			return *this;
		}
		constexpr TConstIterator operator++(int)
		{
			TConstIterator Ret = *this;
			++*this;
			return Ret;
		}

		constexpr TConstIterator& operator--()
		{
			while (MyCurrent > MyHead && (*MyCurrent & 0b11110000))
			{
				--MyCurrent;
			}
			return *this;
		}
		constexpr TConstIterator operator--(int)
		{
			TConstIterator Ret = *this;
			--(*this);
			return Ret;
		}

		constexpr Lime::ptrdiff_t operator-(const TConstIterator& InRhs) const noexcept
		{
			Lime::size_t Count = 0;
			for (TConstIterator Itr = InRhs; Itr != *this; ++Itr)
			{
				++Count;
			}
			return Count;
		}
		constexpr TConstIterator& operator-(Lime::size_t InOffset) const noexcept
		{
			TConstIterator Ret = *this;
			while (InOffset--)
			{
				--Ret;
			}
			return Ret;
		}
		constexpr TConstIterator& operator-=(Lime::size_t InOffset)
		{
			MyCurrent -= InOffset;
			while (InOffset--)
			{
				--(*this);
			}
			return *this;
		}

		constexpr TConstIterator& operator+(Lime::size_t InOffset) const
		{
			TConstIterator Ret = *this;
			while (InOffset--)
			{
				++Ret;
			}
			return Ret;
		}
		constexpr TConstIterator& operator+=(Lime::size_t InOffset)
		{
			while (InOffset--)
			{
				++(*this);
			}
			return *this;
		}

		constexpr bool operator==(const TConstIterator& InRhs) const noexcept
		{
			return MyCurrent == InRhs.MyCurrent;
		}
		constexpr bool operator!=(const TConstIterator& InRhs) const noexcept
		{
			return MyCurrent != InRhs.MyCurrent;
		}
		constexpr bool operator>(const TConstIterator& InRhs) const noexcept
		{
			return MyCurrent > InRhs.MyCurrent;
		}
		constexpr bool operator<(const TConstIterator& InRhs) const noexcept
		{
			return MyCurrent < InRhs.MyCurrent;
		}
		constexpr bool operator>=(const TConstIterator& InRhs) const noexcept
		{
			return MyCurrent >= InRhs.MyCurrent;
		}
		constexpr bool operator<=(const TConstIterator& InRhs) const noexcept
		{
			return MyCurrent <= InRhs.MyCurrent;
		}

		constexpr bool StartWith(const char8_t InChar) const noexcept
		{
			if ((MyCurrent - MyEnd) >= 1)
			{
				TChar First(MyCurrent);
				return First == TChar(InChar);
			}
			return false;
		}

		constexpr bool StartWith(const TUtf8StringView InStr) const noexcept
		{
			TConstIterator Itr = *this;
			for (TChar Char : InStr)
			{
				if (!Itr || *Itr != Char)
				{
					return false;
				}
				++Itr;
			}
			return true;
		}

	CLASS_PRIVATE:
		const char8_t* MyHead;
		char8_t* MyCurrent;
		const char8_t* MyEnd;
	};

	class TIterator {
	public:
		constexpr TIterator() noexcept :
			MyHead(nullptr),
			MyCurrent(nullptr),
			MyEnd(nullptr)
		{}

		constexpr TIterator(const char8_t* InHead, char8_t* InCurrent, const char8_t* InEnd) :
			MyHead(InHead),
			MyCurrent(InCurrent),
			MyEnd(InEnd)
		{}

		constexpr ~TIterator() noexcept = default;

		constexpr TChar operator*()
		{
			return TChar(MyCurrent);
		}
		constexpr const TChar operator*() const
		{
			return TChar(MyCurrent);
		}

		constexpr operator const bool() const
		{
			return MyCurrent != MyEnd;
		}

		constexpr TIterator& operator++()
		{
			MyCurrent += TChar::CharSize(*MyCurrent);
			return *this;
		}
		constexpr TIterator operator++(int)
		{
			TIterator Ret = *this;
			++(*this);
			return Ret;
		}

		constexpr TIterator& operator--()
		{
			while (MyCurrent > MyHead && (*MyCurrent & 0b11110000))
			{
				--MyCurrent;
			}
			return *this;
		}
		constexpr TIterator operator--(int)
		{
			TIterator Ret = *this;
			--(*this);
			return Ret;
		}

		constexpr Lime::ptrdiff_t operator-(const TIterator& InRhs) const noexcept
		{
			Lime::size_t Count = 0;
			for (TIterator Itr = InRhs; Itr != *this; ++Itr)
			{
				++Count;
			}
			return Count;
		}
		constexpr TIterator& operator-(Lime::size_t InOffset) const noexcept
		{
			TIterator Ret = *this;
			while (InOffset--)
			{
				--Ret;
			}
			return Ret;
		}
		constexpr TIterator& operator-=(Lime::size_t InOffset)
		{
			while (InOffset--)
			{
				--(*this);
			}
			return *this;
		}

		constexpr TIterator& operator+(Lime::size_t InOffset) const
		{
			TIterator Ret = *this;
			while (InOffset--)
			{
				++Ret;
			}
			return Ret;
		}
		constexpr TIterator& operator+=(Lime::size_t InOffset)
		{
			while (InOffset--)
			{
				++(*this);
			}
			return *this;
		}

		constexpr bool operator==(const TIterator& InRhs) const noexcept
		{
			return MyCurrent == InRhs.MyCurrent;
		}
		constexpr bool operator!=(const TIterator& InRhs) const noexcept
		{
			return MyCurrent != InRhs.MyCurrent;
		}
		constexpr bool operator>(const TIterator& InRhs) const noexcept
		{
			return MyCurrent > InRhs.MyCurrent;
		}
		constexpr bool operator<(const TIterator& InRhs) const noexcept
		{
			return MyCurrent < InRhs.MyCurrent;
		}
		constexpr bool operator>=(const TIterator& InRhs) const noexcept
		{
			return MyCurrent >= InRhs.MyCurrent;
		}
		constexpr bool operator<=(const TIterator& InRhs) const noexcept
		{
			return MyCurrent <= InRhs.MyCurrent;
		}

		constexpr bool StartWith(const char8_t InChar) const noexcept
		{
			if ((MyCurrent - MyEnd) >= 1)
			{
				TChar First(MyCurrent);
				return First == TChar(InChar);
			}
			return false;
		}

		constexpr bool StartWith(const TUtf8StringView InStr) const noexcept
		{
			TIterator Itr = *this;
			for (TChar Char : InStr)
			{
				if (!Itr || *Itr != Char)
				{
					return false;
				}
				++Itr;
			}
			return true;
		}

	CLASS_PRIVATE:
		const char8_t* MyHead;
		char8_t* MyCurrent;
		const char8_t* MyEnd;
	};
public:
	constexpr TUtf8StringView() :
		MyData()
	{}
	constexpr TUtf8StringView(const char8_t* InStr, Lime::size_t InLength)
	{
		MyData = Lime::TSpan<char8_t>(const_cast<char8_t*>(InStr), InLength);
	}
	constexpr TUtf8StringView(const char8_t* InStr)
	{
		Lime::size_t Count = 0;
		for (const char8_t* Itr = InStr; *Itr != U'\0'; ++Itr)
		{
			++Count;
		}
		MyData = Lime::TSpan<char8_t>(const_cast<char8_t*>(InStr), Count);
	}

	constexpr ~TUtf8StringView() {}

	constexpr TUtf8StringView(const TUtf8StringView& InRhs) :
		MyData(InRhs.MyData)
	{}
	constexpr TUtf8StringView(TUtf8StringView&& InRhs) noexcept :
		MyData(std::exchange(InRhs.MyData, {}))
	{}

	constexpr TUtf8StringView& operator=(const TUtf8StringView& InRhs)
	{
		MyData = InRhs.MyData;
		return *this;
	}

	constexpr TUtf8StringView& operator=(TUtf8StringView&& InRhs) noexcept
	{
		MyData = std::exchange(InRhs.MyData, {});
		return *this;
	}

	constexpr Lime::size_t CharCount() const noexcept override
	{
		return MyData.size();
	}
	constexpr Lime::size_t BufferSize() const noexcept override
	{
		return MyData.size();
	}

	constexpr bool IsEmpty() const noexcept override
	{
		return MyData.empty() || *MyData.data()  == U'\0';
	}

	constexpr void Reset() override
	{
		std::exchange(MyData, {});
	}

	constexpr const char8_t* Bytes() const noexcept
	{
		return MyData.data();
	}

	constexpr TIterator begin() noexcept
	{
		return TIterator(MyData.data(), MyData.data(), MyData.data() + BufferSize());
	}
	constexpr TIterator end() noexcept
	{
		return TIterator(MyData.data(), MyData.data() + BufferSize(), MyData.data() + BufferSize());
	}
	constexpr TConstIterator begin() const noexcept
	{
		return TConstIterator(MyData.data(), MyData.data(), MyData.data() + BufferSize());
	}
	constexpr TConstIterator end() const noexcept
	{
		return TConstIterator(MyData.data(), MyData.data() + BufferSize(), MyData.data() + BufferSize());
	}

	constexpr TConstIterator cbegin() const noexcept
	{
		return TConstIterator(MyData.data(), MyData.data(), MyData.data() + BufferSize());
	}
	constexpr TConstIterator cend() const noexcept
	{
		return TConstIterator(MyData.data(), const_cast<char8_t*>(MyData.data()) + BufferSize(), MyData.data() + BufferSize());
	}

	constexpr bool StartWith(const char8_t InChar) const noexcept
	{
		if (CharCount() >= 1)
		{
			TChar First(MyData.data());
			return First == TChar(InChar);
		}
		return false;
	}

	constexpr bool StartWith(TUtf8StringView InStr) const noexcept
	{
		TConstIterator Itr = cbegin();
		for (TChar Char : InStr)
		{
			if (!Itr || *Itr != Char)
			{
				return false;
			}
			++Itr;
		}
		return true;
	}

CLASS_PRIVATE:
	Lime::TSpan<char8_t> MyData;
};
