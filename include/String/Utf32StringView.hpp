#pragma once


#include "BasicString.hpp"
#include "String.hpp"


class TUtf32StringView : public TBasicStringView {
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

		constexpr char32_t operator*()
		{
			return *MyCurrent;
		}
		constexpr const char32_t& operator*() const
		{
			return *MyCurrent;
		}

		constexpr operator const bool() const
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

		constexpr TConstIterator& operator+(Lime::size_t InOffset)
		{
			TConstIterator Ret = *this;
			Ret.MyCurrent += InOffset;
			return Ret;
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

	CLASS_PRIVATE:
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

		constexpr char32_t operator*()
		{
			return *MyCurrent;
		}
		constexpr const char32_t& operator*() const
		{
			return *MyCurrent;
		}

		constexpr operator const bool() const
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

		constexpr TIterator& operator+(Lime::size_t InOffset)
		{
			TIterator Ret = *this;
			Ret.MyCurrent += InOffset;
			return Ret; ;
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

	CLASS_PRIVATE:
		const char32_t* MyHead;
		char32_t* MyCurrent;
		const char32_t* MyEnd;
	};
public:
	constexpr TUtf32StringView() :
		MyData()
	{}
	constexpr TUtf32StringView(const char32_t* InStr, Lime::size_t InLength)
	{
		MyData = Lime::TSpan<char32_t>(const_cast<char32_t*>(InStr), InLength);
	}
	constexpr TUtf32StringView(const char32_t* InStr)
	{
		Lime::size_t Count = 0;
		for (const char32_t* Itr = InStr; *Itr != U'\0'; ++Itr)
		{
			++Count;
		}
		MyData = Lime::TSpan<char32_t>(const_cast<char32_t*>(InStr), Count);
	}

	constexpr ~TUtf32StringView() {}

	constexpr TUtf32StringView(const TUtf32StringView& InRhs) :
		MyData(InRhs.MyData)
	{}
	constexpr TUtf32StringView(TUtf32StringView&& InRhs) noexcept :
		MyData(std::exchange(InRhs.MyData, {}))
	{}

	constexpr TUtf32StringView& operator=(const TUtf32StringView& InRhs)
	{
		MyData = InRhs.MyData;
		return *this;
	}

	constexpr TUtf32StringView& operator=(TUtf32StringView&& InRhs) noexcept
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
		return TConstIterator(MyData.data(), const_cast<char32_t*>(MyData.data()) + BufferSize(), MyData.data() + BufferSize());
	}

	constexpr bool StartWith(const char32_t InChar) const noexcept
	{
		return BufferSize() >= 1 && MyData[0] == InChar;
	}

	constexpr bool StartWith(TUtf32StringView InStr) const noexcept
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

CLASS_PRIVATE:
	Lime::TSpan<char32_t> MyData;
};
