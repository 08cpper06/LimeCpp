#pragma once

#include "String/Utf32String.hpp"
#include "String/Utf32StringView.hpp"

#include "Parse/ASTNode.hpp"


class TParseResult {
public:
	TSharedPtr<TAstErrorNode> MakeError(Lime::TTokenIterator InItr, TUtf32String InMessage)
	{
		TSharedPtr<TAstErrorNode> Error = MakeShared<TAstErrorNode>();
		Error->MyPosition = InItr;
		Error->MyMessage = InMessage;
		MyErrorList.push_back(Error);
		return Error;
	}
	TUtf32String GenerateUniqueStr()
	{
		return ToUtf32String(UniqueID++);
	}

	TSharedPtr<TAstBaseNode> MyASTRoot;
	Lime::TList<TSharedPtr<TAstErrorNode>> MyErrorList;
	TTypeTable MyTypeTable;
	TVariableTable MyVariableTable;

	TSharedPtr<TBlockEntry> CurrentBlock;

private:
	size_t UniqueID { 0 };
};


class TSourceContext {
public:
	TSourceContext(const TUtf32String& InSource) : 
		MySource(InSource)
	{}
	TSourceContext(const TSourceContext&) = delete;
	~TSourceContext() = default;
	
	TSourceContext& operator=(const TSourceContext&) = delete;
	
	TUtf32StringView Source() const
	{
		return TUtf32StringView(MySource.Bytes(), MySource.CharCount());
	}

	Lime::TTokenIterator TokenBegin() const
	{
		return MyTokens.cbegin();
	}
	Lime::TTokenIterator TokenEnd() const
	{
		return MyTokens.cend();
	}

	const Lime::TList<TToken>& Tokens() const
	{
		return MyTokens;
	}

	TSharedPtr<TAstBaseNode> ASTRoot() const
	{
		return MyParseResult.MyASTRoot;
	}

private: /* Tokenizer */
	TUtf32String MySource;
	Lime::TList<TToken> MyTokens;

private: /* Parser */
	TParseResult MyParseResult;

private:
	friend class Parser;
	friend class Tokenizer;
};