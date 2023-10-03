#pragma once


#include "Core/Option.hpp"

#include "Tokenize/SyntaxFunction.hpp"

#include "String/Utf32String.hpp"
#include "String/Utf32StringView.hpp"
#include "String/Utf8String.hpp"
#include "String/Utf8StringView.hpp"


namespace Lime {

	TOption<size_t> ToUInt(const TUtf32String& InStr) noexcept
	{
		auto Itr = InStr.begin();
		if (!Syntax::IsDigit(*Itr))
		{
			return DefaultErrorType::Error;
		}
		size_t Ret = 0;
		while (Itr != InStr.end())
		{
			Ret *= 10;
			Ret += *Itr - U'0';
			++Itr;
		}
		return Ret;
	}
	TOption<size_t> ToUInt(TUtf32StringView InStr) noexcept
	{
		auto Itr = InStr.begin();
		if (!Syntax::IsDigit(*Itr))
		{
			return DefaultErrorType::Error;
		}
		size_t Ret = 0;
		while (Itr != InStr.end())
		{
			Ret *= 10;
			Ret += *Itr - U'0';
			++Itr;
		}
		return Ret;
	}

	TOption<int> ToInt(const TUtf32String& InStr) noexcept
	{
		auto Itr = InStr.begin();
		bool IsNegative = false;
		if (*Itr == U'-')
		{
			IsNegative = true;
			++Itr;
		}
		else if (*Itr != U'+')
		{
			++Itr;
		}
		else if (!Syntax::IsDigit(*Itr))
		{
			return DefaultErrorType::Error;
		}

		int Ret = 0;
		while (Itr != InStr.end())
		{
			Ret *= 10;
			Ret += *Itr - U'0';
			++Itr;
		}
		return IsNegative ? -Ret : Ret;
	}
	TOption<int> ToInt(TUtf32StringView InStr) noexcept
	{
		auto Itr = InStr.begin();
		bool IsNegative = false;
		if (*Itr == U'-')
		{
			IsNegative = true;
			++Itr;
		}
		else if (*Itr != U'+')
		{
			++Itr;
		}
		else if (!Syntax::IsDigit(*Itr))
		{
			return DefaultErrorType::Error;
		}

		int Ret = 0;
		while (Itr != InStr.end())
		{
			Ret *= 10;
			Ret += *Itr - U'0';
			++Itr;
		}
		return IsNegative ? -Ret : Ret;
	}

}