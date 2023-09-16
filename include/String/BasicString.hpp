#pragma once


#include "../Std.hpp"

class TBasicString {
public:
	virtual ~TBasicString() = default;

	/* ignore '\0' */
	virtual constexpr Lime::size_t CharCount() const noexcept = 0;
	/* ignore '\0' */
	virtual constexpr Lime::size_t BufferSize() const noexcept = 0;

	virtual constexpr bool IsEmpty() const noexcept = 0;

	virtual constexpr void Reset() = 0;
	virtual constexpr void ShrinkOfFit() = 0;

	virtual constexpr void ReSize(Lime::size_t InNewSize) = 0;
	virtual constexpr void Reserve(Lime::size_t InNewSize) = 0;
};

class TBasicStringView {
public:
	virtual ~TBasicStringView() = default;

	/* ignore '\0' */
	virtual constexpr Lime::size_t CharCount() const noexcept = 0;
	/* ignore '\0' */
	virtual constexpr Lime::size_t BufferSize() const noexcept = 0;

	virtual bool IsEmpty() const = 0;
	virtual void Reset() = 0;
};

class TChar {
public:
	static constexpr Lime::size_t CharSize(unsigned char InChar) noexcept
	{
		if (InChar < 128) { return 1; }

		if (InChar >= 0b11110000) { return 4; }
		if (InChar >= 0b11100000) { return 3; }
		if (InChar >= 0b11000000) { return 2; }

		return 0;
	}

	constexpr void Set(const char8_t* InChar, Lime::size_t InLength) noexcept
	{
		switch (InLength) {
			case 4:
				MyData[3] = InChar[3];
			case 3:
				MyData[2] = InChar[2];
			case 2:
				MyData[1] = InChar[1];
			case 1:
				MyData[0] = InChar[0];
				break;
			default:
				MyData[3] = MyData[2] = MyData[1] = MyData[0] = u8'\0';
				break;
		}
	}

public:
	constexpr TChar() noexcept
	{
		MyData[0] = MyData[1] = MyData[2] = MyData[3] = 0;
	}
	constexpr TChar(const char8_t* InChar) noexcept
	{
		size_t Len = CharSize(*InChar);
		Set(InChar, Len);
	}
	constexpr TChar(const char8_t* InChar, size_t Len) noexcept
	{
		Set(InChar, Len);
	}

	const char* operator&() const noexcept
	{
		return reinterpret_cast<const char*>(MyData);
	}

	constexpr operator bool() const noexcept
	{
		return MyData[0] != 0;
	}

	constexpr bool operator==(const TChar& Rhs) const noexcept
	{
		return MyData[0] == Rhs.MyData[0] &&
			MyData[1] == Rhs.MyData[1] &&
			MyData[2] == Rhs.MyData[2] &&
			MyData[3] == Rhs.MyData[3];
	}
	constexpr bool operator!=(const TChar& Rhs) const noexcept
	{
		return MyData[0] != Rhs.MyData[0] ||
			MyData[1] != Rhs.MyData[1] ||
			MyData[2] != Rhs.MyData[2] ||
			MyData[3] != Rhs.MyData[3];
	}

	constexpr bool operator>(const TChar& InRhs) const noexcept
	{
		return MyData[3] > InRhs.MyData[3] &&
			MyData[2] > InRhs.MyData[2] &&
			MyData[1] > InRhs.MyData[1] &&
			MyData[0] > InRhs.MyData[0];
	}
	constexpr bool operator<(const TChar& InRhs) const noexcept
	{
		return MyData[3] < InRhs.MyData[3] &&
			MyData[2] < InRhs.MyData[2] &&
			MyData[1] < InRhs.MyData[1] &&
			MyData[0] < InRhs.MyData[0];
	}

	constexpr bool operator>=(const TChar& InRhs) const noexcept
	{
		return MyData[3] >= InRhs.MyData[3] &&
			MyData[2] >= InRhs.MyData[2] &&
			MyData[1] >= InRhs.MyData[1] &&
			MyData[0] >= InRhs.MyData[0];
	}
	constexpr bool operator<=(const TChar& InRhs) const noexcept
	{
		return MyData[3] <= InRhs.MyData[3] &&
			MyData[2] <= InRhs.MyData[2] &&
			MyData[1] <= InRhs.MyData[1] &&
			MyData[0] <= InRhs.MyData[0];
	}

public:
	char8_t MyData[4];
};
