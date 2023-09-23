#pragma once


#include "ASTNode.hpp"


class TParseResult {
public:
	TSharedPtr<TAstErrorNode> MakeError(Lime::TTokenIterator InItr, TUtf32StringView InMessage)
	{
		TSharedPtr<TAstErrorNode> Error = MakeShared<TAstErrorNode>();
		Error->MyPosition = InItr;
		Error->MyMessage = TUtf32String(InMessage.cbegin(), InMessage.cend());
		MyErrorList.push_back(Error);
		return Error;
	}

	TSharedPtr<TAstBaseNode> MyASTRoot;
	Lime::TList<TSharedPtr<TAstErrorNode>> MyErrorList;
	TVarTypeTable MyVarTypes;

	/* owner ship */
	Lime::TList<TToken> MyTokens;
};

#define PARSE_FUNCTION(FunctionName) \
	static TSharedPtr<TAstBaseNode> FunctionName(TParseResult& OutResult, Lime::TTokenIterator& InItr)

#define PARSE_FUNCTION_IMPLEMENT(FunctionName) \
	TSharedPtr<TAstBaseNode> Parser::FunctionName(TParseResult& OutResult, Lime::TTokenIterator& InItr)


class Parser {
public:
	static TParseResult Analyze(const Lime::TList<TToken>& InList);

CLASS_PRIVATE:
	PARSE_FUNCTION(ParseBlock);

	PARSE_FUNCTION(ParseValue);

	PARSE_FUNCTION(ParseAddSub);

	PARSE_FUNCTION(ParseMulDiv);

	PARSE_FUNCTION(ParseExpr);

	PARSE_FUNCTION(ParseParenthess);

	PARSE_FUNCTION(ParseUnary);

	PARSE_FUNCTION(ParseEquality);

	PARSE_FUNCTION(ParseAssign);

	PARSE_FUNCTION(ParseStmt);

	PARSE_FUNCTION(ParseRelational);

	PARSE_FUNCTION(ParseReturn);

	PARSE_FUNCTION(ParseIf);

	PARSE_FUNCTION(ParseWhile);

	PARSE_FUNCTION(ParseFor);

	PARSE_FUNCTION(ParseFunctionDefinition);
};
