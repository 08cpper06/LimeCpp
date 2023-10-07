#pragma once


#include "ASTNode.hpp"
#include "SourceContext.hpp"


#define PARSE_FUNCTION(FunctionName) \
	static TSharedPtr<TAstBaseNode> FunctionName(TParseResult& OutResult, Lime::TTokenIterator& InItr)

#define PARSE_FUNCTION_IMPLEMENT(FunctionName) \
	TSharedPtr<TAstBaseNode> Parser::FunctionName(TParseResult& OutResult, Lime::TTokenIterator& InItr)


class Parser {
public:
	static void Analyze(TSourceContext&);

CLASS_PRIVATE:
	PARSE_FUNCTION(ParseBlock);

	PARSE_FUNCTION(ParseBlockNoCreate);

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

	PARSE_FUNCTION(ParseVariableDefinition);

	PARSE_FUNCTION(ParseFunctionCall);
};
