#pragma once


#include "../Std.hpp"

#include "String/EncodingConverter.hpp"
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


class Tokenize {
public:
	static Lime::TList<TToken> Analyze(TUtf32StringView InSource);

private:
	static void Initialize();

	static bool IsInitialized()
	{
		return InitializeFlag;
	}

	template <class CheckBlankFunc>
	static TOption<THashString> FindKeyword(const std::set<THashString>& List, TUtf32StringView::TIterator Itr, CheckBlankFunc&& Func)
	{
		for (const auto& KeyWord : List)
		{
			TUtf32StringView Str = KeyWord.GetString();
			if (Itr.StartWith(Str))
			{
				TUtf32StringView::TIterator NextChar = Itr + (KeyWord.GetString().CharCount());
				if (Func(*NextChar))
				{
					TOption<TUtf32StringView> Str = KeyWord.GetString();
					return *Str;
				}
			}
		}
		return DefaultErrorType::Error;
	}

	template <class Func>
	static TOption<THashString> Parse(TUtf32StringView::TIterator InItr, Func&& InFunc)
	{
		TUtf32StringView::TIterator Bgn = InItr;
		while (InFunc(*InItr))
		{
			++InItr;
		}
		if (Bgn != InItr)
		{
			TUtf32StringView Str(Bgn, InItr);
			return THashString(Str);
		}
		return DefaultErrorType::Error;
	}

private:
	static Lime::TSet<THashString> KeywordList;
	static Lime::TSet<THashString> SignList;
	static inline bool InitializeFlag = false;
};

