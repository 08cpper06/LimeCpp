#pragma once


#include "BasicString.hpp"
#include "Utf8StringView.hpp"


class TUtf8String : public TBasicString {
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

		constexpr operator const bool() const noexcept
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
			--*this;
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
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
			{
				--Ret;
			}
			return Ret;
		}
		constexpr TConstIterator& operator-=(Lime::size_t InOffset)
		{
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
			{
				--*this;
			}
			return *this;
		}

		constexpr TConstIterator& operator+(Lime::size_t InOffset) const
		{
			TConstIterator Ret = *this;
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
			{
				++Ret;
			}
			return Ret;
		}
		constexpr TConstIterator& operator+=(Lime::size_t InOffset)
		{
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
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

	public:
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
		constexpr const TChar& operator*() const
		{
			return TChar(MyCurrent);
		}

		constexpr operator const bool() const noexcept
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
			++*this;
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
			--*this;
			return Ret;
		}

		constexpr Lime::ptrdiff_t operator-(const TIterator& InRhs) const noexcept
		{
			Lime::ptrdiff_t Count = 0;
			for (TIterator Itr = InRhs; Itr != *this; ++Itr)
			{
				++Count;
			}
			return Count;
		}
		constexpr TIterator& operator-(Lime::size_t InOffset) const noexcept
		{
			TIterator Ret = *this;
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
			{
				--Ret;
			}
			return Ret;
		}
		constexpr TIterator& operator-=(Lime::size_t InOffset)
		{
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
			{
				--*this;
			}
			return *this;
		}

		constexpr TIterator& operator+(Lime::size_t InOffset) const noexcept
		{
			TIterator Ret = *this;
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
			{
				++Ret;
			}
			return Ret;
		}
		constexpr TIterator& operator+=(Lime::size_t InOffset) noexcept
		{
			for (Lime::size_t Index = 0; Index < InOffset; ++Index)
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

	public:
		const char8_t* MyHead;
		char8_t* MyCurrent;
		const char8_t* MyEnd;
	};

public:
	constexpr TUtf8String() :
		MyData({u8'\0'}),
		MyCodePointCount(1)
	{}
	constexpr TUtf8String(TChar InChar) :
		MyData({ InChar, u8'\0'}),
		MyCodePointCount(2)
	{}
	constexpr TUtf8String(const char8_t* InStr, Lime::size_t InLength)
	{
		Lime::size_t Length = 0;
		Lime::size_t CharSize = 0;
		const char8_t* Itr = InStr;
		MyCodePointCount = 0;
		for (size_t Idx = 0; Idx < InLength; ++Idx)
		{
			CharSize = TChar::CharSize(*Itr);
			Itr += CharSize;
			Length += CharSize;
			++MyCodePointCount;
		}
		++MyCodePointCount;
		MyData.resize(Length + 1, u8'\0');
		for (Lime::size_t Index = 0; Index < Length; ++Index)
		{
			MyData[Index] = InStr[Index];
		}
	}
	template <class UIterator>
	constexpr TUtf8String(UIterator InStart, UIterator InEnd) :
		TUtf8String(InStart.MyCurrent, InEnd - InStart)
	{}
	constexpr TUtf8String(const char8_t* InStr)
	{
		Lime::size_t Length = 0;
		Lime::size_t CharSize = 0;
		const char8_t* Itr = InStr;
		MyCodePointCount = 0;
		while (*Itr)
		{
			CharSize = TChar::CharSize(*Itr);
			Itr += CharSize;
			Length += CharSize;
			++MyCodePointCount;
		}
		++MyCodePointCount;
		MyData.resize(Length + 1, u8'\0');
		Lime::size_t Index = 0;
		while (*InStr)
		{
			MyData[Index++] = *InStr++;
		}
	}

	constexpr ~TUtf8String() {}

	constexpr TUtf8String(const TUtf8String& InRhs) noexcept :
		MyData(InRhs.MyData),
		MyCodePointCount(InRhs.MyCodePointCount)
	{}
	constexpr TUtf8String(TUtf8String&& InRhs) noexcept :
		MyData(std::exchange(InRhs.MyData, {})),
		MyCodePointCount(std::exchange(InRhs.MyCodePointCount, {}))
	{}

	constexpr TUtf8String& operator=(const TUtf8String& InRhs)
	{
		MyData = InRhs.MyData;
		return *this;
	}

	constexpr TUtf8String& operator=(TUtf8String&& InRhs) noexcept
	{
		MyData = std::exchange(InRhs.MyData, {});
		return *this;
	}

	constexpr Lime::size_t CharCount() const noexcept override
	{
		return MyCodePointCount > 0 ? MyCodePointCount - 1 : 0;
	}
	constexpr Lime::size_t BufferSize() const noexcept override
	{
		return MyData.size() > 0 ? MyData.size() - 1 : 0;
	}

	constexpr bool IsEmpty() const noexcept override
	{
		return MyData.empty() || MyData.at(0) == u8'\0';
	}

	constexpr void Reset() override
	{
		MyData.clear();
	}
	constexpr void ShrinkToFit() override
	{
		MyData.shrink_to_fit();
	}

	constexpr void ReSize(Lime::size_t InNewSize) override
	{
		MyData.resize(InNewSize);
	}
	constexpr void Reserve(Lime::size_t InNewSize) override
	{
		MyData.reserve(InNewSize);
	}

	constexpr const char8_t* Bytes() const noexcept
	{
		return MyData.data();
	}

	constexpr TIterator begin() noexcept
	{
		return TIterator(Bytes(), MyData.data(), Bytes() + BufferSize());
	}
	constexpr TIterator end() noexcept
	{
		return TIterator(Bytes(), MyData.data() + BufferSize(), Bytes() + BufferSize());
	}
	constexpr TConstIterator begin() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char8_t*>(MyData.data()), Bytes() + BufferSize());
	}
	constexpr TConstIterator end() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char8_t*>(MyData.data()) + BufferSize(), Bytes() + BufferSize());
	}

	constexpr TConstIterator cbegin() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char8_t*>(MyData.data()), Bytes() + BufferSize());
	}
	constexpr TConstIterator cend() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char8_t*>(Bytes()) + BufferSize(), Bytes() + BufferSize());
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

	constexpr bool StartWith(const TUtf8StringView InStr) const noexcept
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

public:
	constexpr TUtf8String& operator+=(const TChar InChar) noexcept
	{
		if (!MyData.empty() && TChar(MyData.back()) == TChar(u8'\0'))
		{
			MyData.pop_back();
		}
		Lime::size_t CharSize = TChar::CharSize(InChar.MyData[0]);
		if (BufferSize() + CharSize + 1 > MyData.capacity())
		{
			Reserve(BufferSize() + CharSize + 1);
		}
		++MyCodePointCount;
		for (size_t Index = 0; Index < CharSize; ++Index)
		{
			MyData.push_back(InChar.MyData[Index]);
		}
		MyData.push_back(u8'\0');
		return *this;
	}
	constexpr TUtf8String& operator+=(const TUtf8StringView InStr) noexcept
	{
		if (!MyData.empty() && TChar(MyData.back()) == TChar(u8'\0'))
		{
			MyData.pop_back();
		}
		if (MyData.size() + InStr.CharCount() > MyData.capacity())
		{
			Reserve(MyData.size() + InStr.CharCount());
		}
		for (TChar Char : InStr)
		{
			for (Lime::size_t Index = 0; Char.MyData[Index] && Index < 4; ++Index)
			{
				MyData.push_back(Char.MyData[Index]);
			}
			++MyCodePointCount;
		}
		MyData.push_back(u8'\0');
		return *this;
	}
	constexpr TUtf8String& operator+=(const TUtf8String& InStr) noexcept
	{
		return *this += TUtf8StringView(InStr.Bytes());
	}

	TUtf8String operator+(const TUtf8String& InStr) const noexcept
	{
		TUtf8String Str = *this;
		return (Str += TUtf8StringView(InStr.Bytes()));
	}
	TUtf8String operator+(const TUtf8StringView InStr) const noexcept
	{
		TUtf8String Str = *this;
		return (Str += InStr);
	}

	TUtf8String operator+(const char8_t InChar) const noexcept
	{
		TUtf8String Str = *this;
		return Str += InChar;
	}

public:
	constexpr bool operator==(const TUtf8String& InStr) const noexcept
	{
		if (CharCount() != InStr.CharCount())
		{
			return false;
		}
		TConstIterator LhsItr = cbegin();
		TUtf8String::TConstIterator RhsItr = InStr.cbegin();
		for (; LhsItr && RhsItr;)
		{
			if (*LhsItr++ != *RhsItr++)
			{
				return false;
			}
		}
		return true;
	}
	constexpr bool operator==(const TUtf8StringView InStr) const noexcept
	{
		if (CharCount() != InStr.CharCount())
		{
			return false;
		}
		TConstIterator LhsItr = cbegin();
		TUtf8StringView::TConstIterator RhsItr = InStr.cbegin();
		for (;LhsItr && RhsItr;)
		{
			if (*LhsItr++ != *RhsItr++)
			{
				return false;
			}
		}
		return true;
	}

	constexpr bool operator!=(const TUtf8String& InStr) const noexcept
	{
		return !(*this == InStr);
	}
	constexpr bool operator!=(const TUtf8StringView InStr) const noexcept
	{
		return !(*this == InStr);
	}

CLASS_PRIVATE:
	Lime::TArray<char8_t> MyData;
	Lime::size_t MyCodePointCount;
};