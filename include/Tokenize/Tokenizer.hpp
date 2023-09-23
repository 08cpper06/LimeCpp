#pragma once


#include "../Std.hpp"

#include "String/EncodingConverter.hpp"
#include "String/HashString.hpp"

#include "../SourceContext.hpp"


class Tokenizer {
public:
	static void Analyze(TSourceContext& InContext);

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

