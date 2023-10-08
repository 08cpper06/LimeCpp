#pragma once


#include "BasicString.hpp"
#include "Utf32StringView.hpp"


class TUtf32String : public TBasicString {
public:
	class TConstIterator {
	public:
		constexpr TConstIterator() noexcept :
			MyHead(nullptr),
			MyCurrent(nullptr),
			MyEnd(nullptr)
		{}

		constexpr TConstIterator(const char32_t* InHead, char32_t* InCurrent, const char32_t* InEnd) :
			MyHead(InHead),
			MyCurrent(InCurrent),
			MyEnd(InEnd)
		{}

		constexpr ~TConstIterator() noexcept = default;

		constexpr char32_t& operator*()
		{
			return *MyCurrent;
		}
		constexpr const char32_t& operator*() const
		{
			return *MyCurrent;
		}

		constexpr operator const bool() const noexcept
		{
			return MyCurrent != MyEnd;
		}

		constexpr TConstIterator& operator++()
		{
			++MyCurrent;
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
			--MyCurrent;
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
			return MyCurrent - InRhs.MyCurrent;
		}
		constexpr TConstIterator& operator-(Lime::size_t InOffset) const noexcept
		{
			TConstIterator Ret = *this;
			Ret.MyCurrent -= InOffset;
			return Ret;
		}
		constexpr TConstIterator& operator-=(Lime::size_t InOffset)
		{
			MyCurrent -= InOffset;
			return *this;
		}

		constexpr TConstIterator& operator+(Lime::size_t InOffset) const
		{
			TConstIterator Ret = *this;
			Ret.MyCurrent += InOffset;
			return Ret;
		}
		constexpr TConstIterator& operator+=(Lime::size_t InOffset)
		{
			MyCurrent += InOffset;
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

		constexpr bool StartWith(const char32_t InChar) const noexcept
		{
			return (MyEnd - MyCurrent) >= 1 && *MyCurrent == InChar;
		}
		constexpr bool StartWith(TUtf32StringView InStr) const noexcept
		{
			TConstIterator Itr = *this;
			for (char32_t Char : InStr)
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
		const char32_t* MyHead;
		char32_t* MyCurrent;
		const char32_t* MyEnd;
	};

	class TIterator {
	public:
		constexpr TIterator() noexcept :
			MyHead(nullptr),
			MyCurrent(nullptr),
			MyEnd(nullptr)
		{}

		constexpr TIterator(const char32_t* InHead, char32_t* InCurrent, const char32_t* InEnd) :
			MyHead(InHead),
			MyCurrent(InCurrent),
			MyEnd(InEnd)
		{}

		constexpr ~TIterator() noexcept = default;

		constexpr char32_t& operator*()
		{
			return *MyCurrent;
		}
		constexpr const char32_t& operator*() const
		{
			return *MyCurrent;
		}

		constexpr operator const bool() const noexcept
		{
			return MyCurrent != MyEnd;
		}

		constexpr TIterator& operator++()
		{
			++MyCurrent;
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
			--MyCurrent;
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
			return MyCurrent - InRhs.MyCurrent;
		}
		constexpr TIterator& operator-(Lime::size_t InOffset) const noexcept
		{
			TIterator Ret = *this;
			Ret.MyCurrent -= InOffset;
			return Ret;
		}
		constexpr TIterator& operator-=(Lime::size_t InOffset)
		{
			MyCurrent -= InOffset;
			return *this;
		}

		constexpr TIterator& operator+(Lime::size_t InOffset) const
		{
			TIterator Ret = *this;
			Ret.MyCurrent += InOffset;
			return Ret;
		}
		constexpr TIterator& operator+=(Lime::size_t InOffset)
		{
			MyCurrent += InOffset;
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

		constexpr bool StartWith(const char32_t InChar) const noexcept
		{
			return (MyEnd - MyCurrent) >= 1 && *MyCurrent == InChar;
		}
		constexpr bool StartWith(TUtf32StringView InStr) const noexcept
		{
			TIterator Itr = *this;
			for (char32_t Char : InStr)
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
		const char32_t* MyHead;
		char32_t* MyCurrent;
		const char32_t* MyEnd;
	};

public:
	constexpr TUtf32String() :
		MyData({ U'\0' })
	{}
	constexpr TUtf32String(char32_t InChar) :
		MyData({ InChar, U'\0' })
	{}
	constexpr TUtf32String(const char32_t* InStr, Lime::size_t InLength)
	{
		MyData.resize(InLength + 1, U'\0');
		for (char32_t& Char : MyData)
		{
			Char = *InStr++;
		}
	}
	template <class UIterator>
	constexpr TUtf32String(UIterator InStart, UIterator InEnd) :
		TUtf32String(InStart.MyCurrent, InEnd - InStart)
	{}
	constexpr TUtf32String(const char32_t* InStr)
	{
		Lime::size_t Count = 1;
		for (const char32_t* Itr = InStr; *Itr != U'\0'; ++Itr)
		{
			++Count;
		}
		MyData.resize(Count, '\0');
		for (char32_t& Char : MyData)
		{
			Char = *InStr++;
		}
	}

	constexpr ~TUtf32String() {}

	constexpr TUtf32String(const TUtf32String& InRhs) :
		MyData(InRhs.MyData)
	{}
	constexpr TUtf32String(TUtf32String&& InRhs) noexcept :
		MyData(std::exchange(InRhs.MyData, {}))
	{}

	TUtf32String& operator=(const char32_t* InRhs)
	{
		TUtf32StringView Rhs(InRhs);
		MyData.resize(Rhs.CharCount() + 1, U'\0');
		TUtf32StringView::TConstIterator Itr = Rhs.cbegin();
		for (char32_t& Char : MyData)
		{
			Char = *Itr++;
		}
		return *this;
	}

	constexpr TUtf32String& operator=(const TUtf32String& InRhs) noexcept
	{
		MyData = InRhs.MyData;
		return *this;
	}

	constexpr TUtf32String& operator=(TUtf32String&& InRhs) noexcept
	{
		MyData = std::exchange(InRhs.MyData, {});
		return *this;
	}

	constexpr Lime::size_t CharCount() const noexcept override
	{
		return MyData.size() > 0 ? MyData.size() - 1 : 0;
	}
	constexpr Lime::size_t BufferSize() const noexcept override
	{
		return MyData.size() > 0 ? MyData.size() - 1 : 0;
	}

	constexpr bool IsEmpty() const noexcept override
	{
		return MyData.empty() || MyData.at(0) == U'\0';
	}

	constexpr void Reset() override
	{
		MyData.clear();
	}
	constexpr void ShrinkOfFit() override
	{
		MyData.shrink_to_fit();
	}

	constexpr void ReSize(Lime::size_t InNewSize) override
	{
		MyData.resize(InNewSize, U'\0');
	}
	constexpr void Reserve(Lime::size_t InNewSize) override
	{
		MyData.reserve(InNewSize);
	}

	constexpr const char32_t* Bytes() const noexcept
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
		return TConstIterator(Bytes(), const_cast<char32_t*>(MyData.data()), Bytes() + BufferSize());
	}
	constexpr TConstIterator end() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char32_t*>(MyData.data()) + BufferSize(), Bytes() + BufferSize());
	}

	constexpr TConstIterator cbegin() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char32_t*>(MyData.data()), Bytes() + BufferSize());
	}
	constexpr TConstIterator cend() const noexcept
	{
		return TConstIterator(Bytes(), const_cast<char32_t*>(Bytes()) + BufferSize(), Bytes() + BufferSize());
	}

	constexpr bool StartWith(const char32_t InChar) const noexcept
	{
		return BufferSize() >= 1 && MyData[0] == InChar;
	}

	constexpr bool StartWith(const TUtf32StringView InStr) const noexcept
	{
		TConstIterator Itr = cbegin();
		for (char32_t Char : InStr)
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
	TUtf32String& operator+=(const char32_t InChar) noexcept
	{
		if (!MyData.empty() && MyData.back() == U'\0')
		{
			MyData.pop_back();
		}
		if (BufferSize() + 2 > MyData.capacity())
		{
			Reserve(BufferSize() + 2);
		}
		MyData.push_back(InChar);
		MyData.push_back(U'\0');
		return *this;
	}
	
	TUtf32String& operator+=(const char32_t* InStr) noexcept
	{
		TUtf32StringView Rhs = InStr;
		if (!MyData.empty() && MyData.back() == U'\0')
		{
			MyData.pop_back();
		}
		Lime::size_t Lneght = 0;
		if (MyData.size() + Rhs.CharCount() > MyData.capacity())
		{
			Reserve(MyData.size() + Rhs.CharCount());
		}
		for (char32_t Char : Rhs)
		{
			MyData.push_back(Char);
		}
		MyData.push_back(U'\0');
		return *this;
	}
	TUtf32String& operator+=(const TUtf32String& InStr) noexcept
	{
		if (!MyData.empty() && MyData.back() == U'\0')
		{
			MyData.pop_back();
		}
		if (MyData.size() + InStr.CharCount() > MyData.capacity())
		{
			Reserve(MyData.size() + InStr.CharCount());
		}
		for (char32_t Char : InStr)
		{
			MyData.push_back(Char);
		}
		MyData.push_back(U'\0');
		return *this;
	}
	TUtf32String& operator+=(const TUtf32StringView InStr) noexcept
	{
		if (!MyData.empty() && MyData.back() == U'\0')
		{
			MyData.pop_back();
		}
		if (MyData.size() + InStr.CharCount() > MyData.capacity())
		{
			Reserve(MyData.size() + InStr.CharCount());
		}
		for (char32_t Char : InStr)
		{
			MyData.push_back(Char);
		}
		MyData.push_back(U'\0');
		return *this;
	}

	TUtf32String operator+(const TUtf32String& InStr) const noexcept
	{
		TUtf32String Str = *this;
		return (Str += InStr.Bytes());
	}
	TUtf32String operator+(const char32_t* InStr) const noexcept
	{
		TUtf32String Str = *this;
		return (Str += InStr);
	}

	TUtf32String operator+(const char32_t InChar) const noexcept
	{
		TUtf32String Str = *this;
		return Str += InChar;
	}

public:
	constexpr bool operator==(const TUtf32String& InStr) const noexcept
	{
		if (CharCount() != InStr.CharCount())
		{
			return false;
		}
		TConstIterator LhsItr = cbegin();
		TUtf32String::TConstIterator RhsItr = InStr.cbegin();
		for (; LhsItr && RhsItr;)
		{
			if (*LhsItr++ != *RhsItr++)
			{
				return false;
			}
		}
		return true;
	}
	constexpr bool operator==(const TUtf32StringView InStr) const noexcept
	{
		if (CharCount() != InStr.CharCount())
		{
			return false;
		}
		TConstIterator LhsItr = cbegin();
		TUtf32StringView::TConstIterator RhsItr = InStr.cbegin();
		for (;LhsItr && RhsItr;)
		{
			if (*LhsItr++ != *RhsItr++)
			{
				return false;
			}
		}
		return true;
	}

	constexpr bool operator!=(const TUtf32String& InStr) const noexcept
	{
		return !(*this == InStr);
	}
	constexpr bool operator!=(const TUtf32StringView InStr) const noexcept
	{
		return !(*this == InStr);
	}

CLASS_PRIVATE:
	Lime::TArray<char32_t> MyData;
};

inline TUtf32String operator+(const char32_t* InLhs, const TUtf32String& InRhs) noexcept
{
	TUtf32String Str = InLhs;
	return Str + InRhs;
}

inline TUtf32String operator+(const char32_t InLhs, const TUtf32String& InRhs) noexcept
{
	TUtf32String Str = InLhs;
	return Str + InRhs;
}

inline TUtf32String operator+(const TUtf32StringView InLhs, const TUtf32StringView InRhs) noexcept
{
	return TUtf32String(InLhs.Bytes(), InLhs.CharCount()) + TUtf32String(InRhs.Bytes(), InRhs.CharCount());
}

inline TUtf32String operator+(bool InLhs, const TUtf32String& InRhs) noexcept = delete;
inline TUtf32String operator+(bool InLhs, const TUtf32StringView InRhs) noexcept = delete;
inline TUtf32String operator+(const TUtf32String& InLhs, bool InRhs) noexcept = delete;
inline TUtf32String operator+(const TUtf32StringView InLhs, bool InRhs) noexcept = delete;
