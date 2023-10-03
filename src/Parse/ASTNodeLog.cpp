#include "Parse/ASTNode.hpp"
#include <string>


TUtf32String TAstErrorNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Error Line=";
	TUtf32String LineNumber = *String::ConvertCharToUtf32(
		reinterpret_cast<const char8_t*>(std::to_string(MyPosition->MyLine).c_str()));
	Str += LineNumber;
	Str += U" Message=\"" + MyMessage + U"\"/>\n";
	return Str;
}

TUtf32String TAstBlockNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Block Name=\"" + MyBlockName + U"\">\n";
	for (const TSharedPtr<TAstBaseNode> Node : MyNodes)
	{
		Str += Node->GetInfoString(InPrefix + U"\t");
	}
	Str += InPrefix + U"</Block>\n";
	return Str;
}

TUtf32String TAstValNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Val Type=\"" + ToUtf32String(MyType) + U"\">";
	for (Lime::TTokenIterator Itr = MyStartItr; Itr != MyEndItr; ++Itr)
	{
		Str += Itr->MyLetter.GetString();
	}
	// Str += *ConvertToUtf32(*EndItr->Letter.GetString());
	Str += U"</Val>\n";
	return Str;
}

TUtf32String TAstAddSubNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Operator Type=\"" + char32_t(MyOperator->MyLetter.MyHashValue) + U"\">\n";
	Str += MyLhs->GetInfoString(InPrefix + U'\t');
	Str += MyRhs->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Operator>\n";
	return Str;
}

TUtf32String TAstMulDivNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Operator Type=\"" + char32_t(MyOperator->MyLetter.MyHashValue) + U"\">\n";
	Str += MyLhs->GetInfoString(InPrefix + U'\t');
	Str += MyRhs->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Operator>\n";
	return Str;
}

TUtf32String TAstExprNode::GetInfoString(TUtf32String InPrefix) const
{
	return MyNode->GetInfoString(InPrefix);
}

TUtf32String TAstParenthessNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str;
	Str += MyExpr->GetInfoString(InPrefix + U'\t');
	Str += MyError->GetInfoString(InPrefix + U'\t');
	return Str;
}

TUtf32String TAstUnaryNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Unary Operator=" + char32_t(MyOperator->MyLetter.MyHashValue) + U">\n";
	Str += MyExpr->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Unary>\n";
	return Str;
}

TUtf32String TAstEqualityNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Equality Type=\"" + char32_t(MyOperator->MyLetter.MyHashValue) + U"\">\n";
	Str += MyLhs->GetInfoString(InPrefix + U'\t');
	Str += MyRhs->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Equality>\n";
	return Str;
}

TUtf32String TAstAssignNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Assign>\n";
	Str += MyLhs->GetInfoString(InPrefix + U'\t');
	Str += MyRhs->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Assign>\n";
	return Str;
}

TUtf32String TAstRelationalNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Relational Type=\"" + char32_t(MyOperator->MyLetter.MyHashValue) + U"\">\n";
	Str += MyLhs->GetInfoString(InPrefix + U'\t');
	Str += MyRhs->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Relational>\n";
	return Str;
}

TUtf32String TAstVarNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Variable Type=\"";
	Str += MyType.MyName.GetString();
	Str += U"\" Name=\"";
	Str += MyName->MyLetter.GetString();
	Str += U"\"></Variable>\n";
	return Str;
}

TUtf32String TAstReturnNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Return>\n";
	Str += MyExpr->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Return>\n";
	return Str;
}

TUtf32String TAstIfNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<if>\n";
	Str += InPrefix + U"\t<Evaluation>\n";
	Str += MyEvalExpr->GetInfoString(InPrefix + U"\t\t");
	Str += InPrefix + U"\t</Evaluation>\n";

	Str += InPrefix + U"\t<IfTrue>\n";
	Str += MyTrueExpr->GetInfoString(InPrefix + U"\t\t");
	Str += InPrefix + U"\t</IfTrue>\n";

	if (MyFalseExpr)
	{
		Str += InPrefix + U"\t<IfFalse>\n";
		Str += MyFalseExpr->GetInfoString(InPrefix + U"\t\t");
		Str += InPrefix + U"\t</IfFalse>\n";
	}
	Str += InPrefix + U"</If>\n";
	return Str;
}

TUtf32String TAstWhileNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<While>\n";
	Str += InPrefix + U"\t<Evaluation>\n";
	Str += MyEvalExpr->GetInfoString(InPrefix + U"\t\t");
	Str += InPrefix + U"\t</Evaluation>\n";

	Str += InPrefix + U"\t<Block>\n";
	Str += MyBlockExpr->GetInfoString(InPrefix + U"\t\t");
	Str += InPrefix + U"\t</Block>\n";

	Str += InPrefix + U"</While>\n";
	return Str;
}

TUtf32String TAstForNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<For>\n";
	Str += InPrefix + U"\t<Initialize>";
	if (MyInitExpr)
	{
		Str += U"\n" + MyInitExpr->GetInfoString(InPrefix + U"\t\t");
		Str += U"\t" + InPrefix;
	}
	Str += U"</Initialize>\n";
	Str += InPrefix + U"\t<Condition>";
	if (MyCondExpr)
	{
		Str += U"\n" + MyCondExpr->GetInfoString(InPrefix + U"\t\t");
		Str += U"\t" + InPrefix;
	}
	Str += U"</Condition>\n";
	Str += InPrefix + U"\t<Continus>";
	if (MyUpdateExpr)
	{
		Str += U"\n" + MyUpdateExpr->GetInfoString(InPrefix + U"\t\t");
		Str += U"\t" + InPrefix;
	}
	Str += U"</Continus>\n";

	Str += MyBlockExpr->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</For>\n";
	return Str;
}

TUtf32String TAstFunctionDefinitionNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<FunctionDefinition Name=\"";
	Str += MyFunctionName->MyLetter.GetString();
	Str += U"\" ReturnType=\"";
	Str += MyReturnType.MyName.GetString();
	Str += U"\">\n";
	Str += MyBlockExpr->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</FunctionDefinition>\n";
	return Str;
}

TUtf32String TAstVariableDefinitionNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<VariableDefinition Type=\"";
	Str += MyType.MyName.GetString();
	Str += U"\" Name=\"";
	Str += MyName->MyLetter.GetString();
	if (MyIsArray)
	{
		Str += U"\" ArrayCount=\"";
		Str += ToUtf32String(MyArrayCount);
	}
	Str += U"\">";
	
	if (MyInitializeExpr)
	{
		Str += U'\n' + MyInitializeExpr->GetInfoString(InPrefix + U'\t') + InPrefix;
	}
	
	Str += U"</VariableDefinition>\n";
	return Str;
}

TUtf32String TAstInitializerListNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<InitializerList>\n";
	for (TSharedPtr<TAstBaseNode> Node : MyLists)
	{
		Str += Node->GetInfoString(InPrefix + U'\t');
	}
	Str += InPrefix + U"</InitializerList>\n";
	if (MyError)
	{
		Str += MyError->GetInfoString(InPrefix);
	}
	return Str;
}
