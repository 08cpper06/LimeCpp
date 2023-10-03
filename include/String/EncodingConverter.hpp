#pragma once


#include "String.hpp"
#include "../Core/Option.hpp"


enum class ConvertEncodingError {
	NoError,
	InValidChar,
};

class String {
public:

	static TOption<TUtf8String, ConvertEncodingError> ConvertToUtf8(const char* InStr)
	{
		TUtf8String Str;
		for (const char* Itr = InStr; *Itr; ++Itr)
		{
			TOption<char32_t, ConvertEncodingError> Char = ConvertCharToUtf32(*reinterpret_cast<const char32_t*>(Itr));
			if (!Char)
			{
				return ConvertEncodingError::InValidChar;
			}
			Str += *Char;
		}
		return Str;
	}

	static TOption<TUtf8String, ConvertEncodingError> ConvertToUtf8(const char32_t* InStr)
	{
		TUtf8String Str;
		for (const char32_t* Itr = InStr; *Itr; ++Itr)
		{
			TChar Char = ConvertCharToUtf8(*Itr);
			Str += Char;
		}
		return Str;
	}

	static TOption<TUtf8String, ConvertEncodingError> ConvertToUtf8(const TUtf32StringView InStr)
	{
		TUtf8String Str;
		for (const auto Char : InStr)
		{
			if (!Char)
			{
				break;
			}
			Str += ConvertCharToUtf8(Char);
		}
		return Str;
	}

	static TOption<TUtf32String, ConvertEncodingError> ConvertToUtf32(const char* InStr)
	{
		TUtf32String Str;
		TOption<char32_t, ConvertEncodingError> Char;
		for (const char* Itr = InStr; *Itr;)
		{
			Char = ConvertCharToUtf32(TChar(reinterpret_cast<const char8_t*>(Itr)));
			if (!Char)
			{
				return Char.GetStatus();
			}
			Str += *Char;
			Itr += TChar::CharSize(*Itr);
		}
		return Str;
	}

	static TOption<TUtf32String, ConvertEncodingError> ConvertToUtf32(const char8_t* InStr)
	{
		TUtf32String Str;
		TOption<char32_t, ConvertEncodingError> Char;
		for (const char8_t* Itr = InStr; *Itr;)
		{
			Char = ConvertCharToUtf32(TChar(Itr));
			if (!Char)
			{
				return Char.GetStatus();
			}
			Str += *Char;
			Itr += TChar::CharSize(*Itr);
		}
		return Str;
	}

	static TOption<TUtf32String, ConvertEncodingError> ConvertToUtf32(const TUtf8StringView InStr)
	{
		TUtf32String Str;
		TOption<char32_t, ConvertEncodingError> OutChar;
		for (const TChar Char : InStr)
		{
			if (!Char)
			{
				break;
			}
			OutChar = ConvertCharToUtf32(Char);
			if (!OutChar)
			{
				return OutChar.GetStatus();
			}
			Str += *OutChar;
		}
		return Str;
	}

	static constexpr TOption<char32_t, ConvertEncodingError> ConvertCharToUtf32(TChar InChar)
	{
		char32_t Char = 0;
		size_t Length = TChar::CharSize(InChar.MyData[0]);

		switch (Length) {
		case 4:
			Char = InChar.MyData[0] & 0b111;
			break;
		case 3:
			Char = InChar.MyData[0] & 0b1111;
			break;
		case 2:
			Char = InChar.MyData[0] & 0b11111;
			break;
		case 1:
			Char = InChar.MyData[0];
			return Char;
		case 0:
			return ConvertEncodingError::InValidChar;
		}

		for (size_t Index = 1; Index < Length; ++Index)
		{
			if (InChar.MyData[Index] >> 6 != 0b10)
			{
				return ConvertEncodingError::InValidChar;
			}
			Char = (Char << 6) | (InChar.MyData[Index] & 0b111111);
		}

		return Char;
	}

	static constexpr TChar ConvertCharToUtf8(char32_t InChar)
	{
		TChar Char;
		if (InChar <= 0x7F)
		{
			Char.MyData[0] = static_cast<char8_t>(InChar);
			return Char;
		}

		if (InChar <= 0x7FF)
		{
			Char.MyData[0] = 0b11000000 | (InChar >> 6);
			Char.MyData[1] = 0b10000000 | (InChar & 0b00111111);
			return Char;
		}

		if (InChar <= 0xFFFF) {
			Char.MyData[0] = 0b11100000 | (InChar >> 12);
			Char.MyData[1] = 0b10000000 | ((InChar >> 6) & 0b00111111);
			Char.MyData[2] = 0b10000000 | (InChar & 0b00111111);
			return Char;
		}

		Char.MyData[0] = 0b11110000 | (InChar >> 18);
		Char.MyData[1] = 0b10000000 | ((InChar >> 12) & 0b00111111);
		Char.MyData[2] = 0b10000000 | ((InChar >> 6) & 0b00111111);
		Char.MyData[3] = 0b10000000 | (InChar & 0b00111111);
		return Char;
	}
};

inline TUtf32String ToUtf32String(Lime::size_t InValue)
{
	TUtf32String Str = U"";
	char32_t Char;
	if (InValue)
	{
		do {
			Char = *String::ConvertCharToUtf32(TChar(u8"0123456789"[InValue % 10]));
			Str = Char + Str;
			InValue /= 10;
		} while (InValue != 0);
		return Str;
	}
	return U"0";
}
inline TUtf32String ToUtf32String(int InValue)
{
	TUtf32String Str = ToUtf32String(Lime::size_t(InValue > 0 ? InValue : -InValue));
	if (InValue < 0)
	{
		Str = U'-';
		InValue *= -1;
	}
	return Str;
}
