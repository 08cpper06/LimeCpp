#include "Tokenize/Tokenizer.hpp"
#include "Tokenize/SyntaxFunction.hpp"


Lime::TSet<THashString> Tokenizer::KeywordList;
Lime::TSet<THashString> Tokenizer::SignList;


void Tokenizer::Analyze(TSourceContext& InContext)
{
	if (!InitializeFlag)
	{
		Initialize();
	}
	TUtf32StringView Source = TUtf32StringView(InContext.MySource.Bytes(), InContext.MySource.CharCount());

	TOption<THashString> Token;
	THashString Hash;
	TUtf32StringView::TIterator BaseItr;
	size_t Line = 1;
	TUtf32StringView::TIterator Itr = Source.begin();

	while (*Itr)
	{
		Hash = THashString();
		/* skip space */
		Token = Parse(Itr, Syntax::IsSpace);
		if (Token)
		{
			BaseItr = Itr;
			Itr += Token->GetString().CharCount();
			continue;
		}
		/* new line */
		if (*Itr == U'\n')
		{
			++Itr;
			++Line;
			continue;
		}
		/* skip comment */
		if (Itr.StartWith(U"/*"))
		{
			Itr += 2;
			while (*Itr)
			{
				if (Itr.StartWith(U"*/"))
				{
					Itr += 2;
					break;
				}
				++Itr;
			}
			continue;
		}
		if (Itr.StartWith(U"//"))
		{
			Itr += 2;
			while (*Itr != U'\n' && *Itr != U'\0')
			{
				++Itr;
			}
			++Line;
			continue;
		}

		/* parse string */
		if (Itr.StartWith(U'\"'))
		{
			++Itr;
			BaseItr = Itr;
			while (*Itr != U'\"' && *Itr != U'\0')
			{
				++Itr;
			}
			InContext.MyTokens.emplace_back(TUtf32StringView(BaseItr, Itr), BaseItr, Itr, Line, TokenType::StringLiteral);
			++Itr;
			continue;
		}

		/* parse char */
		if (Itr.StartWith(U'\''))
		{
			++Itr;
			BaseItr = Itr;
			while (*Itr != U'\'' && *Itr != U'\0')
			{
				++Itr;
			}
			InContext.MyTokens.emplace_back(TUtf32StringView(BaseItr, Itr), BaseItr, Itr, Line, TokenType::CharLiteral);
			++Itr;
			continue;
		}

		/* parse keyword */
		Token = FindKeyword(KeywordList, Itr, [](char32_t InChar) -> bool {
			return Syntax::IsSpace(InChar);
		});
		if (Token)
		{
			BaseItr = Itr;
			Itr += Token->GetString().CharCount();
			InContext.MyTokens.emplace_back(*Token, BaseItr, Itr, Line, TokenType::Keyword);
			continue;
		}

		/* parse sign */
		Token = FindKeyword(SignList, Itr, [](char32_t InChar) -> bool {
			return true;
		});
		if (Token)
		{
			BaseItr = Itr;
			Itr += Token->GetString().CharCount();
			InContext.MyTokens.emplace_back(*Token, BaseItr, Itr, Line, TokenType::Sign);
			continue;
		}

		if (Syntax::IsGraphicChar(*Itr))
		{
			BaseItr = Itr;
			++Itr;
			InContext.MyTokens.emplace_back(THashString(*BaseItr), BaseItr, Itr, Line, TokenType::Sign);
			continue;
		}

		/* parse number */
		Token = Parse(Itr, Syntax::IsDigit);
		if (Token)
		{
			BaseItr = Itr;
			Itr += Token->GetString().CharCount();
			InContext.MyTokens.emplace_back(*Token, BaseItr, Itr, Line, TokenType::Number);
			continue;
		}

		/* parse identifer */
		char32_t OldChar = U'\0';
		Token = Parse(Itr, [&OldChar](char32_t InChar) -> bool {
			bool Ret = Syntax::IsNoDigit(InChar) || (OldChar && Syntax::IsDigit(InChar));
			OldChar = InChar;
			return Ret;
		});
		if (Token)
		{
			BaseItr = Itr;
			Itr += Token->GetString().CharCount();
			InContext.MyTokens.emplace_back(*Token, BaseItr, Itr, Line, TokenType::Ident);
			continue;
		}

		LIME_ERROR(u8"Could not invalid Token");
	}

	BaseItr = Itr++;
	InContext.MyTokens.emplace_back(THashString(U'\0'), BaseItr, Itr, Line, TokenType::Sign);
}

void Tokenizer::Initialize()
{
	KeywordList.insert(THashString(U"return"));
	KeywordList.insert(THashString(U"if"));
	KeywordList.insert(THashString(U"else"));
	KeywordList.insert(THashString(U"for"));
	KeywordList.insert(THashString(U"while"));
	KeywordList.insert(THashString(U"int"));
	KeywordList.insert(THashString(U"sizeof"));
	KeywordList.insert(THashString(U"char"));
	KeywordList.insert(THashString(U"struct"));
	KeywordList.insert(THashString(U"union"));
	KeywordList.insert(THashString(U"short"));
	KeywordList.insert(THashString(U"long"));
	KeywordList.insert(THashString(U"void"));
	KeywordList.insert(THashString(U"typedef"));
	KeywordList.insert(THashString(U"_Bool"));
	KeywordList.insert(THashString(U"enum"));
	KeywordList.insert(THashString(U"static"));
	KeywordList.insert(THashString(U"goto"));
	KeywordList.insert(THashString(U"break"));
	KeywordList.insert(THashString(U"continue"));
	KeywordList.insert(THashString(U"switch"));
	KeywordList.insert(THashString(U"case"));
	KeywordList.insert(THashString(U"default"));
	KeywordList.insert(THashString(U"extern"));
	KeywordList.insert(THashString(U"_Alignof"));
	KeywordList.insert(THashString(U"_Alignas"));
	KeywordList.insert(THashString(U"do"));
	KeywordList.insert(THashString(U"signed"));
	KeywordList.insert(THashString(U"unsigned"));
	KeywordList.insert(THashString(U"const"));
	KeywordList.insert(THashString(U"volatile"));
	KeywordList.insert(THashString(U"auto"));
	KeywordList.insert(THashString(U"register"));
	KeywordList.insert(THashString(U"restrict"));
	KeywordList.insert(THashString(U"__restrict"));
	KeywordList.insert(THashString(U"__restrict__"));
	KeywordList.insert(THashString(U"_Noreturn"));
	KeywordList.insert(THashString(U"float"));
	KeywordList.insert(THashString(U"double"));
	KeywordList.insert(THashString(U"typeof"));
	KeywordList.insert(THashString(U"asm"));
	KeywordList.insert(THashString(U"_Thread_local"));
	KeywordList.insert(THashString(U"__thread"));
	KeywordList.insert(THashString(U"_Atomic"));
	KeywordList.insert(THashString(U"__attribute__"));

	SignList.insert(THashString(U"<<="));
	SignList.insert(THashString(U">>="));
	SignList.insert(THashString(U"..."));
	SignList.insert(THashString(U"=="));
	SignList.insert(THashString(U"!="));
	SignList.insert(THashString(U"<="));
	SignList.insert(THashString(U">="));
	SignList.insert(THashString(U"->"));
	SignList.insert(THashString(U"+="));
	SignList.insert(THashString(U"-="));
	SignList.insert(THashString(U"*="));
	SignList.insert(THashString(U"/="));
	SignList.insert(THashString(U"++"));
	SignList.insert(THashString(U"--"));
	SignList.insert(THashString(U"%="));
	SignList.insert(THashString(U"&="));
	SignList.insert(THashString(U"|="));
	SignList.insert(THashString(U"^="));
	SignList.insert(THashString(U"&&"));
	SignList.insert(THashString(U"||"));
	SignList.insert(THashString(U"<<"));
	SignList.insert(THashString(U">>"));
	SignList.insert(THashString(U"##"));
}
