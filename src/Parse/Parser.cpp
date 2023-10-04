#include "Parse/Parser.hpp"
#include "String/StringUtility.hpp"


void Parser::Analyze(TSourceContext& InContext)
{
	TSharedPtr<TAstBlockNode> Block = MakeShared<TAstBlockNode>();
	Block->MyBlockName = U"Global";
	InContext.MyParseResult.CurrentBlock = InContext.MyParseResult.MyVariableTable.AddBlock(nullptr, Block->MyBlockName);
	TSharedPtr<TAstBaseNode> Node = nullptr;
	Lime::TTokenIterator Itr = InContext.MyTokens.begin();
	while (Itr->MyLetter != U'\0')
	{
		Node = Parser::ParseStmt(InContext.MyParseResult, Itr);
		if (!Node)
		{
			break;
		}
		Block->MyNodes.push_back(Node);
	}
	InContext.MyParseResult.MyASTRoot = Block;
}

PARSE_FUNCTION_IMPLEMENT(ParseBlock)
{
	TSharedPtr<TAstBlockNode> Block = MakeShared<TAstBlockNode>();
	Block->MyBlockName = U"Anonymous_" + OutResult.GenerateUniqueStr();
	TSharedPtr<TBlockEntry> OldBlockEntry = OutResult.CurrentBlock;
	OutResult.CurrentBlock = OutResult.MyVariableTable.AddBlock(OutResult.CurrentBlock, Block->MyBlockName);
	TSharedPtr<TAstBaseNode> Node = nullptr;
	if (InItr->MyLetter.MyHashValue != U'{')
	{
		return nullptr;
	}
	++InItr;
	while (InItr->MyLetter.MyHashValue != U'}')
	{
		if (InItr->MyLetter.MyHashValue == U';')
		{
			++InItr;
			continue;
		}
		Node = Parser::ParseStmt(OutResult, InItr);
		if (!Node)
		{
			return Block;
		}
		Block->MyNodes.push_back(Node);
	}
	OutResult.CurrentBlock = OldBlockEntry;
	++InItr;
	return Block;
}

PARSE_FUNCTION_IMPLEMENT(ParseValue)
{
	TSharedPtr<TAstBaseNode> Expr = Parser::ParseParenthess(OutResult, InItr);
	if (Expr)
	{
		return Expr;
	}

	Lime::TTokenIterator TmpItr = InItr;
	bool IsHasNumber = false;
	Lime::TTokenIterator StartItr;

	if (TmpItr->MyType == TokenType::Number)
	{
		StartItr = TmpItr;
	}
	else if (TmpItr->MyType == TokenType::Ident)
	{
		TSharedPtr<TAstVarNode> Var = MakeShared<TAstVarNode>();
		Var->MyName = TmpItr;
		TOption<TVarInfo> VarInfo = OutResult.CurrentBlock->GetInfo(TmpItr->MyLetter);
		if (!VarInfo)
		{
			TUtf32String Message = U'`';
			Message += TmpItr->MyLetter.GetString();
			Message += U"` is not defined";
			InItr = ++TmpItr;

			return OutResult.MakeError(TmpItr, Message);
		}
		Var->MyBlock = OutResult.MyVariableTable.GetBlock(VarInfo->MyScope);
		Var->MyType = VarInfo->MyType;
		InItr = ++TmpItr;
		return Var;
	}
	else
	{
		switch (TmpItr->MyLetter.MyHashValue) {
		case U'+':
		case U'-':
			StartItr = TmpItr++;
			break;
		case U'.':
			StartItr = TmpItr;
			break;
		default:
			return nullptr;
		}
	}

	ValueType ValueType = ValueType::Unknown;
	if (TmpItr->MyType == TokenType::Number)
	{
		IsHasNumber = true;
		ValueType = ValueType::Int32;
		++TmpItr;
	}

	if (TmpItr->MyLetter.MyHashValue == U'.')
	{
		if (!IsHasNumber)
		{
			StartItr = TmpItr;
		}
		++TmpItr;
		if (TmpItr->MyType == TokenType::Number)
		{
			IsHasNumber = true;
			++TmpItr;
			ValueType = ValueType::Float;
		}
	}
	if (!IsHasNumber)
	{
		return nullptr;
	}

	TSharedPtr<TAstValNode> Node = MakeShared<TAstValNode>();
	Node->MyStartItr = StartItr;
	Node->MyEndItr = TmpItr;
	Node->MyType = ValueType;

	InItr = TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseAddSub)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TAstBaseNode> Lhs = Parser::ParseMulDiv(OutResult, TmpItr);
	if (!Lhs)
	{
		return nullptr;
	}
	TSharedPtr<TAstAddSubNode> Node;
	TSharedPtr<TAstBaseNode> Value;
	while (Lhs)
	{
		switch (TmpItr->MyLetter.MyHashValue) {
		case U'+':
		case U'-':
			Node = MakeShared<TAstAddSubNode>();
			Node->MyLhs = Lhs;
			Node->MyOperator = TmpItr;
			++TmpItr;
			Value = Parser::ParseMulDiv(OutResult, TmpItr);
			if (!Value)
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of `*` or `/`");
				Node->MyRhs = Error;
				InItr = TmpItr;
				return Node;
			}
			else
			{
				Node->MyRhs = Value;
			}
			Lhs = Node;
			break;
		default:
			InItr = TmpItr;
			return Lhs;
		}
	}
	/* not reachable */
	return nullptr;
}

PARSE_FUNCTION_IMPLEMENT(ParseMulDiv)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TAstBaseNode> Lhs = Parser::ParseUnary(OutResult, TmpItr);
	if (!Lhs)
	{
		return nullptr;
	}
	TSharedPtr<TAstMulDivNode> Node = nullptr;
	TSharedPtr<TAstBaseNode> Value = nullptr;
	while (Lhs)
	{
		switch (TmpItr->MyLetter.MyHashValue) {
		case U'*':
		case U'/':
			Node = MakeShared<TAstMulDivNode>();
			Node->MyLhs = Lhs;
			Node->MyOperator = TmpItr;
			++TmpItr;
			Value = Parser::ParseUnary(OutResult, TmpItr);
			if (!Value)
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of `*` or `/`");
				Node->MyRhs = Error;
				InItr = TmpItr;
				return Node;
			}
			else
			{
				Node->MyRhs = Value;
			}
			Lhs = Node;
			break;
		default:
			InItr = TmpItr;
			return Lhs;
		}
	}
	/* not reachable */
	return nullptr;
}

PARSE_FUNCTION_IMPLEMENT(ParseExpr)
{
	if (InItr->MyLetter.MyHashValue == U'\0')
	{
		return OutResult.MakeError(InItr, U"Eof detected");
	}
	TSharedPtr<TAstBaseNode> Node = Parser::ParseVariableDefinition(OutResult, InItr);
	if (Node)
	{
		return Node;
	}
	return Parser::ParseAssign(OutResult, InItr);
}

PARSE_FUNCTION_IMPLEMENT(ParseParenthess)
{
	Lime::TTokenIterator TmpItr = InItr;
	Lime::TTokenIterator StartItr = TmpItr;
	if (TmpItr->MyLetter.MyHashValue == U'(')
	{
		++TmpItr;
		TSharedPtr<TAstBaseNode> Node = Parser::ParseExpr(OutResult, TmpItr);
		if (TmpItr->MyLetter.MyHashValue == U')')
		{
			InItr = ++TmpItr;
			return Node;
		}
		TSharedPtr<TAstParenthessNode> Error = MakeShared<TAstParenthessNode>();
		Error->MyError = OutResult.MakeError(StartItr, U"Not found `)`");
		Error->MyExpr = Node;
		InItr = TmpItr;
		return Error;
	}
	return nullptr;
}

PARSE_FUNCTION_IMPLEMENT(ParseUnary)
{
	if (InItr->MyLetter.MyHashValue != U'+' &&
		InItr->MyLetter.MyHashValue != U'-')
	{
		return Parser::ParseValue(OutResult, InItr);
	}
	TSharedPtr<TAstUnaryNode> Node = MakeShared<TAstUnaryNode>();
	Node->MyOperator = InItr;
	Node->MyExpr = Parser::ParseValue(OutResult, ++InItr);
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseEquality)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TAstBaseNode> Lhs = Parser::ParseRelational(OutResult, TmpItr);
	if (!Lhs)
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found expression Lhs of equality");
		InItr = TmpItr;
		return Error;
	}
	if (TmpItr->MyLetter != THashString(U"==") &&
		TmpItr->MyLetter != THashString(U"!="))
	{
		InItr = TmpItr;
		return Lhs;
	}
	TSharedPtr<TAstEqualityNode> Node = MakeShared<TAstEqualityNode>();
	Node->MyLhs = Lhs;
	Node->MyOperator = TmpItr;
	++TmpItr;
	Node->MyRhs = Parser::ParseRelational(OutResult, TmpItr);
	if (!(Node->MyRhs))
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of equality");
		InItr = TmpItr;
		return Error;
	}
	InItr = TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseAssign)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TAstBaseNode> Lhs = Parser::ParseEquality(OutResult, TmpItr);
	if (!Lhs)
	{
		return nullptr;
	}
	TSharedPtr<TAstAssignNode> Node = nullptr;
	TSharedPtr<TAstBaseNode> Value = nullptr;
	while (Lhs)
	{
		switch (TmpItr->MyLetter.MyHashValue) {
		case U'=':
			Node = MakeShared<TAstAssignNode>();
			Node->MyLhs = Lhs;
			++TmpItr;
			Value = Parser::ParseAssign(OutResult, TmpItr);
			if (!Value)
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Expected Rhs of `=`");
				Node->MyRhs = Error;
				InItr = TmpItr;
				return Node;
			}
			else
			{
				Node->MyRhs = Value;
			}
			Lhs = Node;
			break;
		default:
			InItr = TmpItr;
			return Lhs;
		}
	}
	/* not reachable */
	return nullptr;
}

PARSE_FUNCTION_IMPLEMENT(ParseStmt)
{
	if (InItr->MyLetter == U'\0')
	{
		return nullptr;
	}
	if (InItr->MyLetter == U';')
	{
		++InItr;
		return Parser::ParseStmt(OutResult, InItr);
	}
	TSharedPtr<TAstBaseNode> Node = nullptr;

	Node = Parser::ParseFunctionDefinition(OutResult, InItr);
	if (Node)
	{
		return Node;
	}

	Node = Parser::ParseIf(OutResult, InItr);
	if (Node)
	{
		return Node;
	}

	Node = Parser::ParseWhile(OutResult, InItr);
	if (Node)
	{
		return Node;
	}

	Node = Parser::ParseFor(OutResult, InItr);
	if (Node)
	{
		return Node;
	}

	Node = Parser::ParseReturn(OutResult, InItr);
	if (Node)
	{
		return Node;
	}

	Node = Parser::ParseExpr(OutResult, InItr);
	if (Node && Node->StaticClass() == TAstErrorNode().StaticClass())
	{
		return Node;
	}

	if (InItr->MyLetter.MyHashValue != U';')
	{
		TSharedPtr<TAstExprNode> Expr = MakeShared<TAstExprNode>();
		Expr->MyNode = OutResult.MakeError(InItr, U"Not found `;`");
		return Expr;
	}
	if (InItr->MyLetter.MyHashValue != U'\0')
	{
		++InItr;
	}
	return Node;

}

PARSE_FUNCTION_IMPLEMENT(ParseRelational)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TAstBaseNode> Lhs = Parser::ParseAddSub(OutResult, TmpItr);
	if (!Lhs)
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Lhs of relational");
		InItr = TmpItr;
		return Error;
	}

	if (TmpItr->MyLetter != U'<' &&
		TmpItr->MyLetter != U'>' &&
		TmpItr->MyLetter != THashString(U"<=") &&
		TmpItr->MyLetter != THashString(U">="))
	{
		InItr = TmpItr;
		return Lhs;
	}
	TSharedPtr<TAstRelationalNode> Node = MakeShared<TAstRelationalNode>();
	Node->MyLhs = Lhs;
	Node->MyOperator = TmpItr;

	++TmpItr;

	Node->MyRhs = Parser::ParseAddSub(OutResult, TmpItr);
	if (!(Node->MyRhs))
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of relational");
		InItr = TmpItr;
		return Error;
	}
	InItr = TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseReturn)
{
	Lime::TTokenIterator TmpItr = InItr;
	if (TmpItr->MyLetter != U"return")
	{
		return nullptr;
	}
	++TmpItr;
	TSharedPtr<TAstReturnNode> Node = MakeShared<TAstReturnNode>();
	Node->MyExpr = Parser::ParseExpr(OutResult, TmpItr);
	if (TmpItr->MyLetter.MyHashValue != U';')
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found semicolon");
		InItr = TmpItr;
		return Error;
	}
	InItr = ++TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseIf)
{
	Lime::TTokenIterator TmpItr = InItr;
	if (TmpItr->MyLetter != THashString(U"if"))
	{
		return nullptr;
	}
	++TmpItr;
	if (TmpItr->MyLetter.MyHashValue != U'(')
	{
		InItr = TmpItr;
		return OutResult.MakeError(InItr, U"Not found `(` after `if`");
	}
	++TmpItr;
	TSharedPtr<TAstBaseNode> Eval = Parser::ParseExpr(OutResult, TmpItr);
	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect expression `)`");
	}
	++TmpItr;
	TSharedPtr<TAstIfNode> Node = MakeShared<TAstIfNode>();
	Node->MyEvalExpr = Eval;
	Node->MyTrueExpr = Parser::ParseBlock(OutResult, TmpItr);
	if (!(Node->MyTrueExpr))
	{
		Node->MyTrueExpr = Parser::ParseStmt(OutResult, TmpItr);
		if (!(Node->MyTrueExpr))
		{
			InItr = TmpItr;
			return OutResult.MakeError(InItr, U"Expect true expr");
		}
	}
	if (TmpItr->MyLetter != THashString(U"else"))
	{
		InItr = TmpItr;
		return Node;
	}
	++TmpItr;
	Node->MyFalseExpr = Parser::ParseBlock(OutResult, TmpItr);
	if (!(Node->MyFalseExpr))
	{
		Node->MyFalseExpr = Parser::ParseStmt(OutResult, TmpItr);
		if (!(Node->MyFalseExpr))
		{
			InItr = TmpItr;
			return OutResult.MakeError(InItr, U"Expect false expr");
		}
	}
	InItr = TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseWhile)
{
	Lime::TTokenIterator TmpItr = InItr;
	if (TmpItr->MyLetter != THashString(U"while"))
	{
		return nullptr;
	}
	++TmpItr;
	if (TmpItr->MyLetter.MyHashValue != U'(')
	{
		InItr = TmpItr;
		return OutResult.MakeError(InItr, U"Not found `(` after `while`");
	}
	++TmpItr;

	TSharedPtr<TAstBaseNode> Eval = Parser::ParseExpr(OutResult, TmpItr);
	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(InItr, U"Expect expression `)`");
	}
	TSharedPtr<TAstWhileNode> Node = MakeShared<TAstWhileNode>();
	Node->MyEvalExpr = Eval;
	++TmpItr;

	if (TmpItr->MyLetter.MyHashValue == U'{')
	{
		Node->MyBlockExpr = Parser::ParseBlock(OutResult, TmpItr);
	}
	else
	{
		Node->MyBlockExpr = Parser::ParseExpr(OutResult, TmpItr);
		if (TmpItr->MyLetter.MyHashValue != U';')
		{
			InItr = TmpItr;
			return OutResult.MakeError(InItr, U"Not found block for `while`");
		}
		++TmpItr;
	}

	if (!(Node->MyBlockExpr))
	{
		InItr = TmpItr;
		return OutResult.MakeError(InItr, U"Not found block for `while`");
	}
	InItr = TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseFor)
{
	Lime::TTokenIterator TmpItr = InItr;
	if (TmpItr->MyLetter != THashString(U"for"))
	{
		return nullptr;
	}
	++TmpItr;

	if (TmpItr->MyLetter.MyHashValue != U'(')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `(`");
	}
	++TmpItr;

	TSharedPtr<TAstBaseNode> InitializeExpr = nullptr;
	if (TmpItr->MyLetter.MyHashValue != U';')
	{
		InitializeExpr = Parser::ParseExpr(OutResult, TmpItr);
	}

	if (TmpItr->MyLetter.MyHashValue != U';')
	{
		InItr = ++TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `;`");
	}
	++TmpItr;

	TSharedPtr<TAstBaseNode> ConditionExpr = nullptr;
	if (TmpItr->MyLetter.MyHashValue != U';')
	{
		ConditionExpr = Parser::ParseExpr(OutResult, TmpItr);
	}
	if (TmpItr->MyLetter.MyHashValue != U';')
	{
		InItr = ++TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `;`");
	}
	++TmpItr;

	TSharedPtr<TAstBaseNode> UpdateExpr = nullptr;
	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		UpdateExpr = Parser::ParseExpr(OutResult, TmpItr);
	}
	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `)`");
	}
	++TmpItr;

	TSharedPtr<TAstForNode> Node = MakeShared<TAstForNode>();
	Node->MyInitExpr = InitializeExpr;
	Node->MyCondExpr = ConditionExpr;
	Node->MyUpdateExpr = UpdateExpr;

	Node->MyBlockExpr = Parser::ParseBlock(OutResult, TmpItr);
	if (!(Node->MyBlockExpr))
	{
		Node->MyBlockExpr = Parser::ParseStmt(OutResult, TmpItr);
		if (!(Node->MyBlockExpr))
		{
			InItr = TmpItr;
			return OutResult.MakeError(InItr, U"not found block for `for`");
		}
	}
	InItr = TmpItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseFunctionDefinition)
{
	Lime::TTokenIterator TmpItr = InItr;
	if (!OutResult.MyVarTypes.IsDefined(TmpItr->MyLetter))
	{
		return nullptr;
	}
	TSharedPtr<TAstFunctionDefinitionNode> Node = MakeShared<TAstFunctionDefinitionNode>();
	Node->MyReturnType = *OutResult.MyVarTypes.GetInfo(TmpItr->MyLetter);
	++TmpItr;
	Node->MyFunctionName = TmpItr;
	++TmpItr;
	if (TmpItr->MyLetter.MyHashValue != U'(')
	{
		return nullptr; /* Variable Definition? */
	}
	++TmpItr;

	/* TODO : Parse Args */

	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `)`");
	}
	++TmpItr;

	Node->MyBlockExpr = Parser::ParseBlock(OutResult, TmpItr);
	if (Node->MyBlockExpr && Node->MyBlockExpr->StaticClass() == TAstBlockNode().StaticClass())
	{
		TSharedPtr<TAstBlockNode> Block = DynamicCast<TAstBlockNode>(Node->MyBlockExpr);
		Block->MyBlockName = TUtf32String(U"Block_") + Node->MyFunctionName->MyLetter.GetString().Bytes();
	}

	InItr = TmpItr;

	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseVariableDefinition)
{
	Lime::TTokenIterator TmpItr = InItr;
	TOption<TVarTypeInfo> TypeInfo = OutResult.MyVarTypes.GetInfo(TmpItr->MyLetter);
	if (!TypeInfo)
	{
		return nullptr;
	}
	++TmpItr;
	TSharedPtr<TAstVariableDefinitionNode> Node = MakeShared<TAstVariableDefinitionNode>();
	Node->MyType = *TypeInfo;
	Node->MyName = TmpItr;
	Node->MyBlock = OutResult.CurrentBlock;

	if (OutResult.CurrentBlock->IsDefined(TmpItr->MyLetter))
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"`" + TmpItr->MyLetter + U"` is already defined");
	}
	++TmpItr;

	Lime::size_t ArrayCount = 1;
	bool IsArray = false;
	if (TmpItr->MyLetter.MyHashValue == U'[')
	{
		IsArray = true;
		++TmpItr;
		if (TmpItr->MyType == TokenType::Number)
		{
			/* explicit array count */
			ArrayCount = *Lime::ToUInt(TmpItr->MyLetter.GetString());
			++TmpItr;
		}

		if (TmpItr->MyLetter.MyHashValue != U']')
		{
			InItr = TmpItr;
			return OutResult.MakeError(TmpItr, U"Expect `]`");
		}
		++TmpItr;
	}

	Node->MyIsArray = IsArray;
	Node->MyArrayCount = ArrayCount;
	OutResult.CurrentBlock->Define(Node->MyName->MyLetter, *TypeInfo, IsArray, ArrayCount);

	if (TmpItr->MyLetter.MyHashValue == U';')
	{
		InItr = TmpItr;
		return Node;
	}

	InItr = TmpItr;
	if (TmpItr->MyLetter.MyHashValue == U'=')
	{
		if (IsArray)
		{
			Lime::TArray<TSharedPtr<TAstBaseNode>> List(ArrayCount);
			++TmpItr;
			if (TmpItr->MyLetter.MyHashValue != U'{')
			{
				InItr = TmpItr;
				return OutResult.MakeError(TmpItr, U"InitializerList should be start `{`");
			}
			++TmpItr;
			TSharedPtr<TAstInitializerListNode> InitialValues = MakeShared<TAstInitializerListNode>();
			while (TmpItr->MyLetter.MyHashValue != U'}')
			{
				TSharedPtr<TAstBaseNode> ElementNode = Parser::ParseExpr(OutResult, TmpItr);
				if (TmpItr->MyLetter.MyHashValue == U'}')
				{
					InitialValues->MyLists.push_back(ElementNode);
					break;
				}
				if (TmpItr->MyLetter.MyHashValue != U',' || !ElementNode.Get())
				{
					InItr = TmpItr;
					return OutResult.MakeError(TmpItr, U"invalid expression was found in `{ }`");
				}
				InitialValues->MyLists.push_back(ElementNode);
				++TmpItr;
			}
			Node->MyInitializeExpr = InitialValues;
			InItr = ++TmpItr;
			if (InitialValues->MyLists.size() > ArrayCount)
			{
				TUtf32String Message = U"InitializerList too much elements : " + ToUtf32String(InitialValues->MyLists.size());
				Message += U" (expected less than " + ToUtf32String(ArrayCount) + U")";
				InitialValues->MyError = OutResult.MakeError(InItr,  Message);
			}
		}
		else
		{
			Node->MyInitializeExpr = Parser::ParseExpr(OutResult, ++InItr);
		}
	}

	return Node;
}
