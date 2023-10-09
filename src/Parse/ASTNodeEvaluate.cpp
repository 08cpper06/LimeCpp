#include "Parse/ASTNode.hpp"
#include "Std.hpp"
#include "String/StringUtility.hpp"


TSharedPtr<TObject> TAstErrorNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstWarningNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstBlockNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstValNode::Evaluate() const noexcept
{
	TSharedPtr<TObject> Object = MakeShared<TObject>();

	TUtf32String Str;
	for (Lime::TTokenIterator Itr = MyStartItr; Itr != MyEndItr; ++Itr)
	{
		Str += Itr->MyLetter;
	}

	Object->MyType = MyType;
	Object->MyValue = Lime::Eval(Str);
	return Object;
}

TSharedPtr<TObject> TAstAddSubNode::Evaluate() const noexcept
{
	TSharedPtr<TObject> Lhs = MyLhs->Evaluate();
	TSharedPtr<TObject> Rhs = MyRhs->Evaluate();
	if (!Lhs || !Rhs)
	{
		return nullptr;
	}
	TSharedPtr<TObject> Ret = MakeShared<TObject>();
	if (TSharedPtr<TTypeInfo> RetType = Lhs->MyType->EvaluateExprType(Rhs->MyType))
	{
		if (Lhs->MyType->MyName == RetType->MyName)
		{
			Ret->MyType = Lhs->MyType;
		}
		Ret->MyType = Rhs->MyType;
	}
	Ret->MyValue = Lhs->MyValue;
	switch (MyOperator->MyLetter.MyHashValue) {
	case U'+':
		Ret->Plus(*Rhs);
		break;
	case U'-':
		Ret->Minus(*Rhs);
		break;
	}
	return Ret;
}

TSharedPtr<TObject> TAstMulDivNode::Evaluate() const noexcept
{
	TSharedPtr<TObject> Lhs = MyLhs->Evaluate();
	TSharedPtr<TObject> Rhs = MyRhs->Evaluate();
	if (!Lhs || !Rhs)
	{
		return nullptr;
	}
	TSharedPtr<TObject> Ret = MakeShared<TObject>();
	if (TSharedPtr<TTypeInfo> RetType = Lhs->MyType->EvaluateExprType(Rhs->MyType))
	{
		if (Lhs->MyType->MyName == RetType->MyName)
		{
			Ret->MyType = Lhs->MyType;
		}
		else
		{
			Ret->MyType = Rhs->MyType;
		}
	}
	Ret->MyValue = Lhs->MyValue;
	switch (MyOperator->MyLetter.MyHashValue) {
	case U'/':
		Ret->Div(*Rhs);
		break;
	case U'*':
		Ret->Time(*Rhs);
		break;
	}
	return Ret;
}

TSharedPtr<TObject> TAstExprNode::Evaluate() const noexcept
{
	return MyNode ? MyNode->Evaluate() : nullptr;
}

TSharedPtr<TObject> TAstParenthessNode::Evaluate() const noexcept
{
	return MyExpr ? MyExpr->Evaluate() : nullptr;
}

TSharedPtr<TObject> TAstPrefixUnaryNode::Evaluate() const noexcept
{
	if (!MyExpr)
	{
		return nullptr;
	}
	int Sign = 1;
	if (MyOperator->MyLetter.MyHashValue == U'-')
	{
		Sign = -1;
	}
	TSharedPtr<TObject> Value = MyExpr->Evaluate();
	if (!Value)
	{
		return nullptr;
	}
	Value->Time(Sign);
	return Value;
}

TSharedPtr<TObject> TAstPostfixUnaryNode::Evaluate() const noexcept
{
	TSharedPtr<TObject> Value = MyExpr->Evaluate();
	if (!Value)
	{
		return nullptr;
	}
	TSharedPtr<TObject> Tmp = MakeShared<TObject>();
	Tmp->MyType = Value->MyType;
	Tmp->MyValue = Value->MyValue;
	if (MyOperator->MyLetter == U"++")
	{
		Value->Plus(1);
	}
	else if (MyOperator->MyLetter == U"--")
	{
		Value->Minus(1);
	}
	return Tmp;
}

TSharedPtr<TObject> TAstArrayReference::Evaluate() const noexcept
{
	if (!MyArrayInfo)
	{
		return nullptr;
	}
	TSharedPtr<TObject> IndexObject = MyIndex->Evaluate();
	int64_t Index = IndexObject->GetAsInteger();

	if (!MyArrayInfo || MyArrayInfo->MyArrayCount < Index || Index < 0)
	{
		return nullptr;
	}

	return MyArrayInfo->MyObject[MyIndex];
}

TSharedPtr<TObject> TAstEqualityNode::Evaluate() const noexcept
{
	if (!MyLhs || !MyRhs)
	{
		return nullptr;
	}
	TSharedPtr<TObject> Lhs = MyLhs->Evaluate();
	TSharedPtr<TObject> Rhs = MyRhs->Evaluate();
	if (!Lhs || !Rhs)
	{
		return nullptr;
	}

	TSharedPtr<TObject> Ret = MakeShared<TObject>();
	Ret->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"bool");
	if (MyOperator->MyLetter == U"==")
	{
		Ret->SetBool(*Lhs == *Rhs);
	}
	else if (MyOperator->MyLetter == U"!=")
	{
		Ret->SetBool(*Lhs != *Rhs);
	}

	return Ret;
}

TSharedPtr<TObject> TAstAssignNode::Evaluate() const noexcept
{
	return nullptr;
}

TSharedPtr<TObject> TAstRelationalNode::Evaluate() const noexcept
{
	if (!MyLhs || !MyRhs)
	{
		return nullptr;
	}
	TSharedPtr<TObject> Lhs = MyLhs->Evaluate();
	TSharedPtr<TObject> Rhs = MyRhs->Evaluate();
	if (!Lhs || !Rhs)
	{
		return nullptr;
	}

	TSharedPtr<TObject> Ret = MakeShared<TObject>();
	Ret->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"bool");
	if (MyOperator->MyLetter == U"<")
	{
		Ret->SetBool(*Lhs < *Rhs);
	}
	else if (MyOperator->MyLetter == U">")
	{
		Ret->SetBool(*Lhs > *Rhs);
	}
	else if (MyOperator->MyLetter == U"<=")
	{
		Ret->SetBool(*Lhs <= *Rhs);
	}
	else if (MyOperator->MyLetter == U">=")
	{
		Ret->SetBool(*Lhs >= *Rhs);
	}

	return Ret;
}

TSharedPtr<TObject> TAstVarNode::Evaluate() const noexcept
{
	return nullptr;
}

TSharedPtr<TObject> TAstReturnNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstIfNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstWhileNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstForNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstFunctionDefinitionNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstVariableDefinitionNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstInitializerListNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

TSharedPtr<TObject> TAstFunctionCallNode::Evaluate() const noexcept
{
	return nullptr; /* not evauatable */
}

