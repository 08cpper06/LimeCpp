#include "Parse/Parser.hpp"
#include "String/StringUtility.hpp"
#include "Parse/Object.hpp"


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

PARSE_FUNCTION_IMPLEMENT(ParseBlockNoCreate)
{
	TSharedPtr<TAstBlockNode> Block = MakeShared<TAstBlockNode>();
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
		if (Node->StaticClass() == TAstReturnNode().StaticClass())
		{
			Block->ReturnList.push_back(StaticCast<TAstReturnNode>(Node));
		}
		else if (!Node->EvaluateType())
		{
			if (Node->StaticClass() == TAstIfNode().StaticClass())
			{
				TSharedPtr<TAstIfNode> IfNode = StaticCast<TAstIfNode>(Node);
				if (IfNode->MyTrueExpr && IfNode->MyTrueExpr->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(IfNode->MyTrueExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
				if (IfNode->MyFalseExpr && IfNode->MyFalseExpr->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(IfNode->MyTrueExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
			}
			else if (Node->StaticClass() == TAstWhileNode().StaticClass())
			{
				TSharedPtr<TAstWhileNode> WhileNode = StaticCast<TAstWhileNode>(Node);
				if (WhileNode->MyBlockExpr && WhileNode->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(WhileNode->MyBlockExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
			}
			else if (Node->StaticClass() == TAstForNode().StaticClass())
			{
				TSharedPtr<TAstForNode> ForNode = StaticCast<TAstForNode>(Node);
				if (ForNode->MyBlockExpr && ForNode->MyBlockExpr->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(ForNode->MyBlockExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
			}
		}
	}
	++InItr;
	return Block;
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
	Block->MyStartItr = InItr++;
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
		if (Node->StaticClass() == TAstReturnNode().StaticClass())
		{
			Block->ReturnList.push_back(StaticCast<TAstReturnNode>(Node));
		}
		else if (!Node->EvaluateType())
		{
			if (Node->StaticClass() == TAstIfNode().StaticClass())
			{
				TSharedPtr<TAstIfNode> IfNode = StaticCast<TAstIfNode>(Node);
				if (IfNode->MyTrueExpr && IfNode->MyTrueExpr->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(IfNode->MyTrueExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
				if (IfNode->MyFalseExpr && IfNode->MyFalseExpr->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(IfNode->MyTrueExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
			}
			else if (Node->StaticClass() == TAstWhileNode().StaticClass())
			{
				TSharedPtr<TAstWhileNode> WhileNode = StaticCast<TAstWhileNode>(Node);
				if (WhileNode->MyBlockExpr && WhileNode->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(WhileNode->MyBlockExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
			}
			else if (Node->StaticClass() == TAstForNode().StaticClass())
			{
				TSharedPtr<TAstForNode> ForNode = StaticCast<TAstForNode>(Node);
				if (ForNode->MyBlockExpr && ForNode->MyBlockExpr->StaticClass() == TAstBlockNode().StaticClass())
				{
					const Lime::TArray<TSharedPtr<TAstReturnNode>>& ChildReturnList = StaticCast<TAstBlockNode>(ForNode->MyBlockExpr)->ReturnList;
					Block->ReturnList.insert(Block->ReturnList.end(), ChildReturnList.begin(), ChildReturnList.end());
				}
			}
		}
	}
	Block->MyEndItr = InItr++;
	OutResult.CurrentBlock = OldBlockEntry;
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
		TSharedPtr<TVarInfo> VarInfo = OutResult.CurrentBlock->GetInfo(TmpItr->MyLetter);
		if (!VarInfo)
		{
			TSharedPtr<TAstBaseNode> CallNode = Parser::ParseFunctionCall(OutResult, TmpItr);
			InItr = TmpItr;
			if (!CallNode)
			{
				TUtf32String Message = U'`';
				Message += TmpItr->MyLetter.GetString();
				Message += U"` is not defined";
				InItr = ++TmpItr;
				return OutResult.MakeError(TmpItr, Message);
			}
			return CallNode;
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

	TSharedPtr<TTypeInfo> ValueType;
	if (TmpItr->MyType == TokenType::Number)
	{
		IsHasNumber = true;
		ValueType = OutResult.MyTypeTable.GetInfo(U"int");
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
			ValueType = OutResult.MyTypeTable.GetInfo(U"float");
		}
	}
	if (!IsHasNumber)
	{
		return nullptr;
	}

	if (!ValueType)
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Unknown type");
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
			if (TSharedPtr<TTypeInfo> LhsType = Node->MyLhs->EvaluateType())
			{
				Node->MyLhsType = LhsType;
			}
			else
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of `*` or `/`");
				Node->MyLhs = Error;
				InItr = TmpItr;
				return Node;
			}
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

			if (TSharedPtr<TTypeInfo> RhsType = Node->MyRhs->EvaluateType())
			{
				Node->MyRhsType = RhsType;
			}
			else
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of `*` or `/`");
				Node->MyRhs = Error;
				InItr = TmpItr;
				return Node;
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
			if (TSharedPtr<TTypeInfo> LhsType = Node->MyLhs->EvaluateType())
			{
				Node->MyLhsType = LhsType;
			}
			else
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of `*` or `/`");
				Node->MyLhs = Error;
				InItr = TmpItr;
				return Node;
			}
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

			if (TSharedPtr<TTypeInfo> RhsType = Node->MyRhs->EvaluateType())
			{
				Node->MyRhsType = RhsType;
			}
			else
			{
				TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(TmpItr, U"Not found Rhs of `*` or `/`");
				Node->MyRhs = Error;
				InItr = TmpItr;
				return Node;
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
	TSharedPtr<TAstBaseNode> Node;
	Node = Parser::ParseFunctionCall(OutResult, InItr);
	if (Node)
	{
		return Node;
	}

	Node = Parser::ParseCharInitialization(OutResult, InItr);
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
	TSharedPtr<TAstParenthessNode> ParenthessNode = MakeShared<TAstParenthessNode>();
	ParenthessNode->MyStartItr = TmpItr;
	if (TmpItr->MyLetter.MyHashValue == U'(')
	{
		++TmpItr;
		TSharedPtr<TAstBaseNode> Node = Parser::ParseExpr(OutResult, TmpItr);
		if (TmpItr->MyLetter.MyHashValue == U')')
		{
			ParenthessNode->MyEndItr = TmpItr;
			InItr = ++TmpItr;
			return Node;
		}
		ParenthessNode->MyError = OutResult.MakeError(StartItr, U"Not found `)`");
		ParenthessNode->MyExpr = Node;
		InItr = TmpItr;
		return ParenthessNode;
	}
	return nullptr;
}

PARSE_FUNCTION_IMPLEMENT(ParseUnary)
{
	if (InItr->MyLetter.MyHashValue != U'+' &&
		InItr->MyLetter.MyHashValue != U'-' &&
		InItr->MyLetter != U"++" &&
		InItr->MyLetter != U"--")
	{
		return Parser::ParsePosfixUnary(OutResult, InItr);
	}
	TSharedPtr<TAstPrefixUnaryNode> Node = MakeShared<TAstPrefixUnaryNode>();
	Node->MyOperator = InItr;
	Node->MyExpr = Parser::ParseValue(OutResult, ++InItr);
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParsePosfixUnary)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TAstBaseNode> Value = Parser::ParseValue(OutResult, TmpItr);
	if (TmpItr->MyLetter == U"++" ||
		TmpItr->MyLetter == U"--")
	{
		TSharedPtr<TAstPostfixUnaryNode> Node = MakeShared<TAstPostfixUnaryNode>();
		Node->MyExpr = Value;
		Node->MyOperator = TmpItr;
		InItr = ++TmpItr;
		return Node;
	}
	/* array index reference */
	if ((Value && Value->StaticClass() == TAstVarNode().StaticClass()) &&
		TmpItr->MyLetter.MyHashValue == U'[')
	{
		TSharedPtr<TAstArrayReference> Node = MakeShared<TAstArrayReference>();
		TSharedPtr<TAstVarNode> VarNode = StaticCast<TAstVarNode>(Value);
		Node->MyStartItr = TmpItr++;
		if (TSharedPtr<TBlockEntry> BlockEntry = VarNode->MyBlock.Lock())
		{
			TSharedPtr<TVarInfo> VarInfo = BlockEntry->GetInfo(VarNode->MyName->MyLetter);
			if (VarInfo && VarInfo->MyIsArray)
			{
				Node->MyArrayInfo = VarInfo;
				Node->MyIndex = Parser::ParseExpr(OutResult, TmpItr);
				
				if (TmpItr->MyLetter.MyHashValue != U']')
				{
					InItr = TmpItr;
					return OutResult.MakeError(TmpItr, U"expected a `]`");
				}
				Node->MyEndItr = TmpItr;
				InItr = ++TmpItr;
				/* Check index range if constant index */
				if (TSharedPtr<TObject> IndexObject = Node->MyIndex->Evaluate())
				{
					if (int64_t* Index = IndexObject->GetInteger())
					{
						if (*Index < 0 || *Index >= VarInfo->MyArrayCount)
						{
							InItr = TmpItr;
							Node->MyError = OutResult.MakeError(TmpItr, U"out of range");
						}
					}
					else
					{
						InItr = TmpItr;
						Node->MyError = OutResult.MakeError(TmpItr, U"array index should be integer");
					}
				}
				return Node;

			}
		}
	}

	/* TODO : `.` or `->` */

	InItr = TmpItr;
	return Value;
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
			Node->MyOperator = TmpItr++;
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

	Node = Parser::ParseVariableDefinition(OutResult, InItr);
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
	Node->MyPosition = InItr;
	if (TmpItr->MyLetter.MyHashValue != U';')
	{
		Node->MyExpr = Parser::ParseExpr(OutResult, TmpItr);
		if (Node->MyExpr && !Node->MyExpr->EvaluateType())
		{
			InItr = TmpItr;
			while (InItr->MyLetter.MyHashValue != U';' && InItr->MyLetter.MyHashValue != U'\0')
			{
				++InItr;
			}
			Node->MyExpr = OutResult.MakeError(Node->MyPosition, U"expected expression");
			return Node;
		}
		if (TmpItr->MyLetter.MyHashValue != U';')
		{
			InItr = TmpItr;
			Node->MyExpr = OutResult.MakeError(TmpItr, U"Not found semicolon");
			return Node;
		}
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
	TSharedPtr<TAstIfNode> Node = MakeShared<TAstIfNode>();
	Node->MyIfPosition = TmpItr++;
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
	Node->MyElsePosition = TmpItr++;
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
	TSharedPtr<TAstWhileNode> Node = MakeShared<TAstWhileNode>();
	Node->MyPosition = TmpItr++;
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
		InitializeExpr = Parser::ParseVariableDefinition(OutResult, TmpItr);
		if (!InitializeExpr)
		{
			InitializeExpr = Parser::ParseExpr(OutResult, TmpItr);
		}
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
		ConditionExpr = Parser::ParseVariableDefinition(OutResult, TmpItr);
		if (!ConditionExpr)
		{
			ConditionExpr = Parser::ParseExpr(OutResult, TmpItr);
		}
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
		UpdateExpr = Parser::ParseVariableDefinition(OutResult, TmpItr);
		if (!UpdateExpr)
		{
			UpdateExpr = Parser::ParseExpr(OutResult, TmpItr);
		}
	}
	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `)`");
	}
	++TmpItr;

	TSharedPtr<TAstForNode> Node = MakeShared<TAstForNode>();
	Node->MyPosition = InItr;
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
	if (!OutResult.MyTypeTable.IsDefined(TmpItr->MyLetter))
	{
		return nullptr;
	}
	TSharedPtr<TAstFunctionDefinitionNode> Node = MakeShared<TAstFunctionDefinitionNode>();
	Node->MyReturnType = OutResult.MyTypeTable.GetInfo(TmpItr->MyLetter);
	++TmpItr;
	Node->MyFunctionName = TmpItr;
	++TmpItr;
	if (TmpItr->MyLetter.MyHashValue != U'(')
	{
		return nullptr; /* Variable Definition? */
	}

	Lime::TArray<THashString> Arguments;
	OutResult.CurrentBlock = OutResult.MyVariableTable.AddBlock(OutResult.CurrentBlock, U"Block_" + Node->MyFunctionName->MyLetter);

	do {
		++TmpItr;
		/* Check whether this function is undefined */
		TSharedPtr<TTypeInfo> ArgumentsTypeInfo = OutResult.MyTypeTable.GetInfo(TmpItr->MyLetter);
		if (ArgumentsTypeInfo)
		{
			Arguments.push_back(TmpItr->MyLetter);
			TSharedPtr<TAstBaseNode> TmpNode = ParseVariableDefinition(OutResult, TmpItr);
			if (TmpNode->StaticClass() == TAstVariableDefinitionNode().StaticClass())
			{
				TSharedPtr<TAstVariableDefinitionNode> VarDefineNode = StaticCast<TAstVariableDefinitionNode>(TmpNode);
				OutResult.CurrentBlock->Define(VarDefineNode->MyName->MyLetter, ArgumentsTypeInfo, VarDefineNode->MyIsArray, VarDefineNode->MyArrayCount);
				TSharedPtr<TVarInfo> Info = OutResult.CurrentBlock->GetInfo(VarDefineNode->MyName->MyLetter);
				Node->MyArguments.push_back({ ArgumentsTypeInfo, Info });
			}
			else if (TmpNode->StaticClass() == TAstErrorNode().StaticClass())
			{
				return TmpNode;
			}
			else
			{
				InItr = TmpItr;
				return OutResult.MakeError(TmpItr, U"Unknown error at function definition");
			}
		}
		else if (TmpItr->MyLetter != U')')
		{
			InItr = TmpItr;
			return OutResult.MakeError(TmpItr, U"Invalid function arguments");
		}
	}
	while (TmpItr->MyLetter.MyHashValue == U',');

	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `)`");
	}
	++TmpItr;

	Node->MyBlockExpr = Parser::ParseBlockNoCreate(OutResult, TmpItr);
	if (Node->MyBlockExpr && Node->MyBlockExpr->StaticClass() == TAstBlockNode().StaticClass())
	{
		TSharedPtr<TAstBlockNode> Block = StaticCast<TAstBlockNode>(Node->MyBlockExpr);
		Block->MyBlockName = U"Block_" + Node->MyFunctionName->MyLetter;

		for (TSharedPtr<TAstReturnNode> ReturnTypePtr : Block->ReturnList)
		{
			if (!(ReturnTypePtr->MyExpr))
			{
				if (Node->MyReturnType->MyName != THashString(U"void"))
				{
					Node->MyErrors.push_back(OutResult.MakeError(ReturnTypePtr->MyPosition, U"return value is not matched"));
				}
			}
			else if (TSharedPtr<TTypeInfo> ReturnType = ReturnTypePtr->MyExpr->EvaluateType())
			{
				CastErrorCode IsCastable;
				{
					IsCastable = ReturnType->IsCastable(Node->MyReturnType->MyName);
				}
				if ((Node->MyReturnType->MyName == THashString(U"void") && ReturnType->MyName != THashString(U"void")) ||
					IsCastable == CastErrorCode::NotCastable)
				{
					Node->MyErrors.push_back(OutResult.MakeError(ReturnTypePtr->MyPosition, U"return value is not matched"));
				}
				TUtf32String Message;
				switch (IsCastable) {
				case CastErrorCode::LossCast:
					Message = U"Argument type information drop cast(Expected : " + Node->MyReturnType->MyName + U", Actual : " + ReturnType->MyName + U")";
					ReturnTypePtr->MyWarning = OutResult.MakeWarning(ReturnTypePtr->MyPosition, Message);
					break;
				case CastErrorCode::ExplicitCastable:
					Message = U"Argument type needs explicit cast(Expected : " + Node->MyReturnType->MyName + U", Actual : " + ReturnType->MyName + U")";
					ReturnTypePtr->MyWarning = OutResult.MakeWarning(ReturnTypePtr->MyPosition, Message);
					break;
				}
			}
		}
	}
	OutResult.MyTypeTable.Define(TTypeInfo(Node->MyFunctionName->MyLetter, Arguments, Node->MyReturnType));
	InItr = TmpItr;

	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseStringInitialization)
{
	if (InItr->MyType != TokenType::StringLiteral)
	{
		return nullptr;
	}
	TSharedPtr<TAstInitializerListNode> Node = MakeShared<TAstInitializerListNode>();
	Node->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"char");
	Node->MyStartItr = Node->MyEndItr = InItr;

	Lime::size_t Index = 0;
	TSharedPtr<TAstStringValNode> CharNode;
	for (char32_t Char : InItr->MyLetter.GetString())
	{
		CharNode = MakeShared<TAstStringValNode>();
		CharNode->MyOffset = Index++;
		CharNode->MyPosition = InItr;
		Node->MyLists.push_back(CharNode);
	}
	/* Insert null character */
	CharNode = MakeShared<TAstStringValNode>();
	CharNode->MyOffset = -1;
	CharNode->MyPosition = InItr;
	Node->MyLists.push_back(CharNode);
	++InItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseCharInitialization)
{
	if (InItr->MyType != TokenType::CharLiteral)
	{
		return nullptr;
	}
	if (InItr->MyLetter.GetString().CharCount() > 1)
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(InItr, U"multi-character character constant");
		++InItr;
		return Error;
	}
	if (InItr->MyLetter.GetString().CharCount() == 0)
	{
		TSharedPtr<TAstErrorNode> Error = OutResult.MakeError(InItr, U"empty character constant");
		++InItr;
		return Error;
	}
	TSharedPtr<TAstValNode> Node = MakeShared<TAstValNode>();
	Node->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"char");
	Node->MyStartItr = InItr;
	Node->MyEndItr = ++InItr;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseVariableDefinition)
{
	Lime::TTokenIterator TmpItr = InItr;
	TSharedPtr<TTypeInfo> TypeInfo = OutResult.MyTypeTable.GetInfo(TmpItr->MyLetter);
	if (!TypeInfo)
	{
		return nullptr;
	}
	if (TmpItr->MyLetter == U"void")
	{
		return OutResult.MakeError(TmpItr, U"`void` type variable cannot be defined");
	}
	++TmpItr;
	TSharedPtr<TAstVariableDefinitionNode> Node = MakeShared<TAstVariableDefinitionNode>();
	Node->MyType = TypeInfo;
	Node->MyName = TmpItr;
	Node->MyBlock = OutResult.CurrentBlock;

	if (OutResult.CurrentBlock->IsDefined(TmpItr->MyLetter))
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"`" + TmpItr->MyLetter + U"` is already defined");
	}
	++TmpItr;

	Lime::size_t ArrayCount = 0;
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

	if (TmpItr->MyLetter.MyHashValue == U';')
	{
		Node->MyArrayCount = ArrayCount;
		OutResult.CurrentBlock->Define(Node->MyName->MyLetter, TypeInfo, IsArray, ArrayCount);
		InItr = TmpItr;
		return Node;
	}

	InItr = TmpItr;
	OutResult.CurrentBlock->Define(Node->MyName->MyLetter, TypeInfo, IsArray, ArrayCount);
	TSharedPtr<TVarInfo> VariableInfo = OutResult.CurrentBlock->GetInfo(Node->MyName->MyLetter);

	/* Check whether value initialize */
	if (TmpItr->MyLetter.MyHashValue == U'=')
	{
		if (IsArray)
		{
			++TmpItr;
			if (Node->MyType->MyName == U"char")
			{
				if (TSharedPtr<TAstInitializerListNode> StringNode = StaticCast<TAstInitializerListNode>(ParseStringInitialization(OutResult, TmpItr)))
				{
					Node->MyInitializeExpr = StringNode;
					Lime::size_t ParseArrayCount = 0;
					for (TSharedPtr<TAstBaseNode> Element : StringNode->MyLists)
					{
						TSharedPtr<TAstStringValNode> CharItem = StaticCast<TAstStringValNode>(Element);
						VariableInfo->MyObject.push_back(MakeShared<TObject>(Node->MyType, CharItem->Evaluate()));
						++ParseArrayCount;
					}
					if (!ArrayCount)
					{
						Node->MyArrayCount = ParseArrayCount;
						VariableInfo->MyIsArray = ParseArrayCount;
					}
					else
					{
						if (ArrayCount < ParseArrayCount)
						{
							TUtf32String Message = U"InitializerList too much elements : " + ToUtf32String(ParseArrayCount);
							Message += U" (expected less than " + ToUtf32String(ArrayCount) + U")";
							StringNode->MyError = OutResult.MakeError(InItr, Message);
						}

					}
					Node->MyInitializeExpr = StringNode;
					InItr = TmpItr;
					return Node;
				}
			} /* end of `if (Node->MyType->MyName == U"char")` */
			Lime::TArray<TSharedPtr<TAstBaseNode>> List(ArrayCount);
			if (TmpItr->MyLetter.MyHashValue != U'{')
			{
				InItr = TmpItr;
				return OutResult.MakeError(TmpItr, U"InitializerList should be start `{`");
			}
			TSharedPtr<TAstInitializerListNode> InitialValues = MakeShared<TAstInitializerListNode>();
			InitialValues->MyStartItr = TmpItr++;
			while (TmpItr->MyLetter.MyHashValue != U'}')
			{
				TSharedPtr<TAstBaseNode> ElementNode = Parser::ParseExpr(OutResult, TmpItr);
				if (TmpItr->MyLetter.MyHashValue == U'}')
				{
					InitialValues->MyLists.push_back(ElementNode);
					VariableInfo->MyObject.push_back(MakeShared<TObject>(Node->MyType, ElementNode->Evaluate()));
					InitialValues->MyEndItr = TmpItr;
					break;
				}
				if (TmpItr->MyLetter.MyHashValue != U',' || !ElementNode.Get())
				{
					InItr = TmpItr;
					return OutResult.MakeError(TmpItr, U"invalid expression was found in `{ }`");
				}
				InitialValues->MyLists.push_back(ElementNode);
				VariableInfo->MyObject.push_back(MakeShared<TObject>(Node->MyType, ElementNode->Evaluate()));
				++TmpItr;
			}
			Node->MyInitializeExpr = InitialValues;
			InItr = ++TmpItr;
			if (!ArrayCount)
			{
				ArrayCount = InitialValues->MyLists.size();
			}
			else if (InitialValues->MyLists.size() > ArrayCount)
			{
				TUtf32String Message = U"InitializerList too much elements : " + ToUtf32String(InitialValues->MyLists.size());
				Message += U" (expected less than " + ToUtf32String(ArrayCount) + U")";
				InitialValues->MyError = OutResult.MakeError(InItr,  Message);
			}
		}
		else /* if (IsArray) */
		{
			++InItr;
			if (!(Node->MyInitializeExpr = Parser::ParseCharInitialization(OutResult, InItr)))
			{
				Node->MyInitializeExpr = Parser::ParseExpr(OutResult, InItr);
			}
			VariableInfo->MyObject.push_back(MakeShared<TObject>(Node->MyType, Node->MyInitializeExpr->Evaluate()));
		}
	}

	Node->MyArrayCount = ArrayCount;
	VariableInfo->MyIsArray = ArrayCount;
	return Node;
}

PARSE_FUNCTION_IMPLEMENT(ParseFunctionCall)
{
	Lime::TTokenIterator TmpItr = InItr;
	Lime::TTokenIterator SaveFunctionName = InItr;

	TSharedPtr<TTypeInfo> FunctionDefineInfo = OutResult.MyTypeTable.GetInfo(TmpItr->MyLetter);
	TSharedPtr<TAstFunctionCallNode> Node = MakeShared<TAstFunctionCallNode>();
	Node->MyFunction = FunctionDefineInfo;
	Node->MyPosition = TmpItr++;
	
	if (TmpItr->MyLetter.MyHashValue != U'(')
	{
		return nullptr;
	}

	Lime::TArray<TSharedPtr<TAstBaseNode>> ArgumentNodeList;
	do {
		++TmpItr;
		TSharedPtr<TAstBaseNode> ArgumentNode = Parser::ParseValue(OutResult, TmpItr);
		if (!ArgumentNode)
		{
			InItr = TmpItr;
			return OutResult.MakeError(TmpItr, U"Expected expression");
		}

		ArgumentNodeList.push_back(ArgumentNode);
	} while (TmpItr->MyLetter.MyHashValue == U',');

	if (TmpItr->MyLetter.MyHashValue != U')')
	{
		InItr = TmpItr;
		return OutResult.MakeError(TmpItr, U"Expect `)`");
	}
	++TmpItr;
	InItr = TmpItr;

	/* Check whether this function is defined */
	if (!FunctionDefineInfo  || !(FunctionDefineInfo->IsFunction()))
	{
		return OutResult.MakeError(SaveFunctionName, U'`' + SaveFunctionName->MyLetter + U"` is not defined");
	}

	/* Check wther these argument type is matched */
	Lime::size_t ArgIdx = 0;
	const Lime::TArray<THashString>& ExpectArgument = FunctionDefineInfo->MyMemberVariable;
	for (TSharedPtr<TAstBaseNode> Argument : ArgumentNodeList)
	{
		if (Argument->StaticClass() == TAstErrorNode().StaticClass())
		{
			Node->MyArguments.push_back({ nullptr, Argument });
		}
		else
		{
			TSharedPtr<TTypeInfo> ActualArgumentType = Argument->EvaluateType();
			if (!ActualArgumentType) /* Check whether this argument is evaluatable type */
			{
				Node->MyArguments.push_back({ nullptr, OutResult.MakeError(TmpItr, U"Expected expression") });
			}
			else if (ArgIdx > FunctionDefineInfo->MyMemberVariable.size()) /* Check whether argument count is too much */
			{
				Node->MyArguments.push_back({ nullptr, OutResult.MakeError(TmpItr, U"Too much arguments") });
			}
			else /* Check whether argument is matched */
			{
				CastErrorCode IsCastable = ActualArgumentType->IsCastable(ExpectArgument[ArgIdx]);
				TUtf32String Message;
				switch (IsCastable) {
				case CastErrorCode::NotCastable:
					Message = U"Argument type is not matched(Expected : " + ExpectArgument[ArgIdx] + U", Actual : " + ActualArgumentType->MyName + U")";
					Node->MyArguments.push_back({ Argument, OutResult.MakeError(TmpItr, Message) });
					break;
				case CastErrorCode::LossCast:
					Message = U"Argument type information drop cast(Expected : " + ExpectArgument[ArgIdx] + U", Actual : " + ActualArgumentType->MyName + U")";
					Node->MyArguments.push_back({ Argument, OutResult.MakeWarning(TmpItr, Message) });
					break;
				case CastErrorCode::ExplicitCastable:
					Message = U"Argument type needs explicit cast(Expected : " + ExpectArgument[ArgIdx] + U", Actual : " + ActualArgumentType->MyName + U")";
					Node->MyArguments.push_back({ Argument, OutResult.MakeWarning(TmpItr, Message) });
					break;
				case CastErrorCode::Castable:
					Node->MyArguments.push_back({ Argument, nullptr });
					break;
				}
			}
		}
		++ArgIdx;
	}

	/* Check whether this function argument count is matched */
	if (Node->MyArguments.size() < FunctionDefineInfo->MyMemberVariable.size())
	{
		Node->MyError = OutResult.MakeError(SaveFunctionName, U'`' + SaveFunctionName->MyLetter + U"` function is too short arguments");
	}

	return Node;
}
