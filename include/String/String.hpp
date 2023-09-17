#pragma once


#include "Utf32String.hpp"
#include "Utf32StringView.hpp"

#include "Utf8String.hpp"
#include "Utf8StringView.hpp"

#include "EncodingConverter.hpp"


#include <iostream>

template <class _Traits>
std::basic_ostream<char, _Traits>& operator<<(std::basic_ostream<char, _Traits>& _Ostr, TUtf32StringView _Val)
{
	TChar Utf8Char;
	for (char32_t Char : _Val)
	{
		if (Char <= 0x7F)
		{
			Utf8Char.MyData[0] = static_cast<char8_t>(Char);
			_Ostr.put(Char);
			continue;
		}

		if (Char <= 0x7FF)
		{
			Utf8Char.MyData[0] = 0b11000000 | (Char >> 6);
			Utf8Char.MyData[1] = 0b10000000 | (Char & 0b00111111);
			_Ostr.write(reinterpret_cast<char*>(Utf8Char.MyData), 2);
			continue;
		}

		if (Char <= 0xFFFF) {
			Utf8Char.MyData[0] = 0b11100000 | (Char >> 12);
			Utf8Char.MyData[1] = 0b10000000 | ((Char >> 6) & 0b00111111);
			Utf8Char.MyData[2] = 0b10000000 | (Char & 0b00111111);
			_Ostr.write(reinterpret_cast<char*>(Utf8Char.MyData), 3);
			continue;
		}

		Utf8Char.MyData[0] = 0b11110000 | (Char >> 18);
		Utf8Char.MyData[1] = 0b10000000 | ((Char >> 12) & 0b00111111);
		Utf8Char.MyData[2] = 0b10000000 | ((Char >> 6) & 0b00111111);
		Utf8Char.MyData[3] = 0b10000000 | (Char & 0b00111111);
		_Ostr.write(reinterpret_cast<char*>(Utf8Char.MyData), 4);
	}
	return _Ostr;
}

template <class _Traits>
std::basic_ostream<char, _Traits>& operator<<(std::basic_ostream<char, _Traits>& _Ostr, TUtf8StringView _Val)
{
	for (TChar Char : _Val)
	{
		_Ostr.write(reinterpret_cast<char*>(Char.MyData), TChar::CharSize(Char.MyData[0]));
	}
	return _Ostr;
}

template <class _Traits>
std::basic_ostream<char, _Traits>& operator<<(std::basic_ostream<char, _Traits>& _Ostr, const TUtf32String& _Val)
{
	TChar Utf8Char;
	for (char32_t Char : _Val)
	{
		if (Char <= 0x7F)
		{
			Utf8Char.MyData[0] = static_cast<char8_t>(Char);
			_Ostr.put(Char);
			continue;
		}

		if (Char <= 0x7FF)
		{
			Utf8Char.MyData[0] = 0b11000000 | (Char >> 6);
			Utf8Char.MyData[1] = 0b10000000 | (Char & 0b00111111);
			_Ostr.write(reinterpret_cast<char*>(Utf8Char.MyData), 2);
			continue;
		}

		if (Char <= 0xFFFF) {
			Utf8Char.MyData[0] = 0b11100000 | (Char >> 12);
			Utf8Char.MyData[1] = 0b10000000 | ((Char >> 6) & 0b00111111);
			Utf8Char.MyData[2] = 0b10000000 | (Char & 0b00111111);
			_Ostr.write(reinterpret_cast<char*>(Utf8Char.MyData), 3);
			continue;
		}

		Utf8Char.MyData[0] = 0b11110000 | (Char >> 18);
		Utf8Char.MyData[1] = 0b10000000 | ((Char >> 12) & 0b00111111);
		Utf8Char.MyData[2] = 0b10000000 | ((Char >> 6) & 0b00111111);
		Utf8Char.MyData[3] = 0b10000000 | (Char & 0b00111111);
		_Ostr.write(reinterpret_cast<char*>(Utf8Char.MyData), 4);
	}
	return _Ostr;
}

template <class _Traits>
std::basic_ostream<char, _Traits>& operator<<(std::basic_ostream<char, _Traits>& _Ostr, const TUtf8String& _Val)
{
	for (TChar Char : _Val)
	{
		_Ostr.write(reinterpret_cast<char*>(Char.MyData), TChar::CharSize(Char.MyData[0]));
	}
	return _Ostr;
}
