#pragma once


#include "String/Utf8String.hpp"
#include "String/Utf32String.hpp"
#include "String/HashString.hpp"


enum class TokenType {
	Unknown,
	Keyword,
	Sign,
	Number,
	Ident
};

inline TUtf32String ToUtf32String(TokenType InType) noexcept
{
	switch (InType) {
	case TokenType::Keyword:
		return U"Keyword";
	case TokenType::Sign:
		return U"Sign";
	case TokenType::Number:
		return U"Number";
	case TokenType::Ident:
		return U"Ident";
	}
	return U"Unknown";
}

inline TUtf8String ToUtf8String(TokenType InType) noexcept
{
	switch (InType) {
	case TokenType::Keyword:
		return u8"Keyword";
	case TokenType::Sign:
		return u8"Sign";
	case TokenType::Number:
		return u8"Number";
	case TokenType::Ident:
		return u8"Ident";
	}
	return u8"Unknown";
}

class TToken {
public:
	TToken(THashString InStr,
		TUtf32StringView::TIterator InStart, TUtf32StringView::TIterator InEnd,
		Lime::size_t InLine, TokenType InType
	) noexcept :
		MyLetter(InStr),
		MyStart(InStart),
		MyEnd(InEnd),
		MyLine(InLine),
		MyType(InType)
	{}

	TUtf8String GetInfoString() const
	{
		char8_t Buffer[1024] = {};
		size_t StartPos = MyStart.MyCurrent - MyStart.MyHead;
		size_t EndPos = MyEnd.MyCurrent - MyEnd.MyHead;
		if (MyLetter.MyHashValue != U'\0')
		{
			std::sprintf(reinterpret_cast<char*>(Buffer), "Line : %llu「%20s」(%3llu, %3llu) Length : %3llu (Type : %s)",
				MyLine,
				reinterpret_cast<const char*>(String::ConvertToUtf8(MyLetter.GetString())->Bytes()),
				StartPos, EndPos,
				EndPos - StartPos,
				reinterpret_cast<const char*>(ToUtf8String(MyType).Bytes())
			);
			return Buffer;
		}
		std::sprintf(reinterpret_cast<char*>(Buffer), "Line : %llu「%20s」(%3llu, %3llu) Length : %3llu",
			MyLine,
			"EOF",
			StartPos, EndPos,
			EndPos - StartPos
		);
		return Buffer;
	}

	THashString MyLetter;
	TUtf32StringView::TIterator MyStart;
	TUtf32StringView::TIterator MyEnd;
	Lime::size_t MyLine;
	TokenType MyType;
};
