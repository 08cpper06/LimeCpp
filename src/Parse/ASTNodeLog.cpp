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

TUtf32String TAstWarningNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Warning Line=";
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
	TUtf32String Str = InPrefix + U"<Val Type=\"" + MyType.MyName + U"\">";
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
	TUtf32String Str = InPrefix + U"<Operator Type=\"" + MyOperator->MyLetter + U"\">\n";
	Str += MyLhs->GetInfoString(InPrefix + U'\t');
	Str += MyRhs->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Operator>\n";
	return Str;
}

TUtf32String TAstMulDivNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Operator Type=\"" + MyOperator->MyLetter + U"\">\n";
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
	TUtf32String Str = InPrefix + U"<Unary Operator=" + MyOperator->MyLetter + U">\n";
	Str += MyExpr->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</Unary>\n";
	return Str;
}

TUtf32String TAstEqualityNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<Equality Type=\"" + MyOperator->MyLetter + U"\">\n";
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
	TUtf32String Str = InPrefix + U"<Relational Type=\"" + MyOperator->MyLetter + U"\">\n";
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
	TUtf32String Str = InPrefix + U"<Return";
	if (MyWarning)
	{
		Str += U" Warning=\"" + MyWarning->MyMessage + U'\"';
	}
	Str += U'>';
	if (MyExpr)
	{
		Str += U'\n' + MyExpr->GetInfoString(InPrefix + U'\t') + InPrefix;
	}
	Str += U"</Return>\n";
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
	Str += MyFunctionName->MyLetter + U"\" ReturnType=\"" + MyReturnType.MyName + U"\">\n";
	Str += InPrefix + U"\t<Arguments>";
	if (MyArguments.empty())
	{
		Str +=  U"</Arguments>\n";
	}
	else
	{
		Str += U'\n';
		for (const Lime::TPair<TTypeInfo, TVarInfo>& Argument : MyArguments)
		{
			if (Argument.second.MyIsArray)
			{
				Str += InPrefix + U"\t\t<Detail Type=\"";
				Str += Argument.first.MyName + U"[]";
				Str += U"\" Name=\"" + Argument.second.MyName + U"\" Count=\"";
				Str += ToUtf32String(Argument.second.MyArrayCount);
				Str += U"\"/>\n";
			}
			else
			{
				Str += InPrefix + U"\t\t<Detail Type=\"";
				Str += Argument.first.MyName;
				Str += U"\" Name=\"" + Argument.second.MyName + U"\"/>\n";
			}
		}
		Str += InPrefix + U'\t' + U"</Arguments>\n";
	}
	for (TSharedPtr<TAstErrorNode> Error : MyErrors)
	{
		Str += Error->GetInfoString(InPrefix + U'\t');
	}
	Str += MyBlockExpr->GetInfoString(InPrefix + U'\t');
	Str += InPrefix + U"</FunctionDefinition>\n";
	return Str;
}

TUtf32String TAstVariableDefinitionNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<VariableDefinition Type=\"";
	Str += MyType.MyName + U"\" Name=\"" + MyName->MyLetter;
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

TUtf32String TAstFunctionCallNode::GetInfoString(TUtf32String InPrefix) const
{
	TUtf32String Str = InPrefix + U"<FunctionCall Name=\"";
	Str += MyFunction.MyName;
	Str += U"\">\n";

	if (MyError)
	{
		Str += MyError->GetInfoString(InPrefix + U'\t');
		Str += InPrefix + U"</FunctionCall>\n";
		return Str;
	}

	if (MyArguments.empty())
	{
		Str += InPrefix + U"\t<Arguments></Arguments>\n";
	}
	else
	{
		Str += InPrefix + U"\t<Arguments>\n";
		for (Lime::TPair<TSharedPtr<TAstBaseNode>, TSharedPtr<TAstBaseNode>> Value : MyArguments)
		{
			if (Value.second && Value.second->StaticClass() == TAstErrorNode().StaticClass())
			{
				Str += Value.second->GetInfoString(InPrefix + U"\t\t");
				continue;
			}
			if (Value.first->StaticClass() == TAstValNode().StaticClass())
			{
				TSharedPtr<TAstValNode> TmpNode = StaticCast<TAstValNode>(Value.first);
				Str += InPrefix + U"\t\t<Detail Type=\"";
				Str += TmpNode->MyType.MyName;
				TUtf32String ValueStr;
				for (Lime::TTokenIterator Itr = TmpNode->MyStartItr; Itr != TmpNode->MyEndItr; ++Itr)
				{
					ValueStr += Itr->MyLetter.GetString();
				}
				Str += U"\" Value=\"" + ValueStr + U"\"";
			}
			else if (Value.first->StaticClass() == TAstVarNode().StaticClass())
			{
				TSharedPtr<TAstVarNode> TmpNode = StaticCast<TAstVarNode>(Value.first);
				TVarInfo Info;
				if (TmpNode)
				{
					if (TSharedPtr<TBlockEntry> Block = TmpNode->MyBlock.Lock())
					{
						Info = *Block->GetInfo(TmpNode->MyName->MyLetter);
					}
				}
				if (Info.MyIsArray)
				{
					Str += InPrefix + U"\t\t<Detail Type=\"";
					Str += Info.MyName + U"[]";
					Str += U"\" Name=\"" + Info.MyName + U"\" Count=\"";
					Str += ToUtf32String(Info.MyArrayCount);
					Str += U"\"";
				}
				else
				{
					Str += InPrefix + U"\t\t<Detail Type=\"";
					Str += Info.MyName;
					Str += U"\" Name=\"" + Info.MyName + U"\"";
				}
			}
			else if (Value.first->StaticClass() == TAstErrorNode().StaticClass() ||
					Value.first->StaticClass() == TAstWarningNode().StaticClass())
			{
				Str += Value.first->GetInfoString(InPrefix + U"\t\t");
			}

			if (Value.second)
			{
				TSharedPtr<TAstWarningNode> WarningNode = StaticCast<TAstWarningNode>(Value.second);
				Str += U" Warning=\"" + WarningNode->MyMessage + U'\"';
			}
			Str += U"/>\n";

		}
		Str += InPrefix + U"\t</Arguments>\n";
	}

	Str += InPrefix + U"</FunctionCall>\n";
	return Str;
}
