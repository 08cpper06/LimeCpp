#pragma once


class Syntax {
public:
	static constexpr bool IsSpace(char32_t InChar)
	{
		switch (InChar) {
		case U' ':
		case U'\t':
			return true;
		}
		return false;
	}

	static constexpr bool IsAlphaUpperCase(char32_t InChar)
	{
		return InChar >= U'A' && InChar <= U'Z';
	}

	static constexpr bool IsAlphaLowerCase(char32_t InChar)
	{
		return InChar >= U'a' && InChar <= U'z';
	}

	static constexpr bool IsDigit(char32_t InChar)
	{
		return (InChar >= U'0' && InChar <= U'9');
	}

	static constexpr bool IsNoDigit(char32_t InChar)
	{
		return InChar == U'_' || IsAlphaLowerCase(InChar) || IsAlphaUpperCase(InChar);
	}

	static constexpr bool IsGraphicChar(char32_t InChar)
	{
		switch (InChar) {
		case U'!':
		case U'\"':
		case U'\'':
		case U'%':
		case U'&':
		case U'#':
		case U'(':
		case U')':
		case U'*':
		case U'+':
		case U',':
		case U'-':
		case U'.':
		case U'/':
		case U':':
		case U';':
		case U'<':
		case U'=':
		case U'>':
		case U'?':
		case U'[':
		case U'\\':
		case U']':
		case U'^':
		case U'_':
		case U'{':
		case U'|':
		case U'}':
		case U'~':
			return true;
		}
		return false;
	}

	static constexpr bool IsHexaDecimalDigit(char32_t InChar)
	{
		return IsDigit(InChar) || (InChar >= U'a' && InChar <= U'f') || (InChar >= U'A' && InChar <= U'F');
	}

	static constexpr bool IsNonZeroDigit(char32_t InChar)
	{
		return IsDigit(InChar) && InChar != U'0';
	}

	static constexpr bool IsOctalDigit(char32_t InChar)
	{
		return InChar >= '0' && InChar <= '7';
	}

};