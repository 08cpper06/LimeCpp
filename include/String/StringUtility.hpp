#pragma once


#include "Core/Option.hpp"

#include "Tokenize/SyntaxFunction.hpp"

#include "String/Utf32String.hpp"
#include "String/Utf32StringView.hpp"
#include "String/Utf8String.hpp"
#include "String/Utf8StringView.hpp"


namespace Lime {

	inline TOption<size_t> ToUInt(const TUtf32String& InStr) noexcept
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
			Ret += static_cast<size_t>(*Itr) - U'0';
			++Itr;
		}
		return Ret;
	}
	inline TOption<size_t> ToUInt(TUtf32StringView InStr) noexcept
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
			Ret += static_cast<size_t>(*Itr) - U'0';
			++Itr;
		}
		return Ret;
	}

	inline TOption<int> ToInt(const TUtf32String& InStr) noexcept
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
	inline TOption<int> ToInt(TUtf32StringView InStr) noexcept
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

	inline TVariant<int64_t, double> EvalNumeric(TUtf32String InStr) noexcept
	{
		bool IsDot = false;
		int64_t IntValue = 0;
		double DoubleValue = 0;
		double Digit = 10;

		for (char32_t Char : InStr)
		{
			if (!Syntax::IsDigit(Char))
			{
				break;
			}
			else if (Char == U'.')
			{
				if (IsDot)
				{
					break;
				}
				IsDot = true;
				DoubleValue = static_cast<double>(IntValue);
			}
			if (IsDot)
			{
				DoubleValue = Digit * (static_cast<int64_t>(Char) - U'0');
				Digit /= 10;
			}
			else
			{
				IntValue *= 10;
				IntValue += static_cast<int64_t>(Char) - U'0';
			}
		}

		TVariant<int64_t, double> Value;
		if (IsDot)
		{
			Value = DoubleValue;
		}
		else
		{
			Value = IntValue;
		}
		return Value;
	}

}