#include "Parse/ASTNode.hpp"
#include "Asm/BasicInstructBuilder.hpp"


void TAstErrorNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	/* not implement */
}

void TAstWarningNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	/* not implement */
}

void TAstBlockNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	Lime::size_t Pos = InBuilder.GetStackOffset();
	for (TSharedPtr<TAstBaseNode> Node : MyNodes)
	{
		Node->BuildIR(InBuilder);
	}
	InBuilder.SetStackOffset(Pos);
}

void TAstValNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, MyValue);
	InBuilder.PushStack(Operand);
}

void TAstStringValNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	/* not implement */
}

void TAstAddSubNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (IsStaticEvaluatable())
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Evaluate());
		TSharedPtr<TAsmBasicPushInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicPushInstruct>(Operand);
		Instruct->MyPosition = MyOperator;
		return;
	}

	Assert(MyRhs);
	MyRhs->BuildIR(InBuilder);
	Assert(MyLhs);
	MyLhs->BuildIR(InBuilder);

	TSharedPtr<TAsmBasicOperand> LhsOperand = InBuilder.PopStack();
	TSharedPtr<TAsmBasicOperand> RhsOperand = InBuilder.PopStack();

	/* add new item to stack if constant value */
	TSharedPtr<TAsmBasicOperand> TmpRhsOperand = LhsOperand;
	LhsOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Stack, TmpRhsOperand->MyValue->MyType);
	TSharedPtr<TAsmBasicMovInstruct> MovInstruct = InBuilder.CreateInstruct<TAsmBasicMovInstruct>();
	MovInstruct->MyPosition = MyOperator;
	MovInstruct->MyLhs = LhsOperand;
	MovInstruct->MyRhs = TmpRhsOperand;

	InBuilder.PushStack(LhsOperand);
	
	TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(MyOperator->MyLetter.MyHashValue);
	Instruct->MyLhs = LhsOperand;
	Instruct->MyRhs = RhsOperand;
	Instruct->MyPosition = MyOperator;

}

void TAstMulDivNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (IsStaticEvaluatable())
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Evaluate());
		InBuilder.PushStack(Operand);
		InBuilder.CreateInstruct<TAsmBasicPushInstruct>(Operand);
		return;
	}

	Assert(MyRhs);
	MyRhs->BuildIR(InBuilder);
	Assert(MyLhs);
	MyLhs->BuildIR(InBuilder);

	TSharedPtr<TAsmBasicOperand> LhsOperand = InBuilder.PopStack();
	TSharedPtr<TAsmBasicOperand> RhsOperand = InBuilder.PopStack();

	/* add new item to stack if constant value */
	TSharedPtr<TAsmBasicOperand> TmpRhsOperand = LhsOperand;
	LhsOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Stack, TmpRhsOperand->MyValue->MyType);
	TSharedPtr<TAsmBasicMovInstruct> MovInstruct = InBuilder.CreateInstruct<TAsmBasicMovInstruct>();
	MovInstruct->MyPosition = MyOperator;
	MovInstruct->MyLhs = LhsOperand;
	MovInstruct->MyRhs = TmpRhsOperand;
	InBuilder.PushStack(LhsOperand);

	TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(MyOperator->MyLetter.MyHashValue);
	Instruct->MyLhs = LhsOperand;
	Instruct->MyRhs = RhsOperand;
	Instruct->MyPosition = MyOperator;
}

void TAstExprNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	MyNode->BuildIR(InBuilder);
}

void TAstParenthessNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	MyExpr->BuildIR(InBuilder);
}

void TAstPrefixUnaryNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	Assert(MyExpr);
	MyExpr->BuildIR(InBuilder);

	if (MyOperator->MyLetter.MyHashValue == U'-')
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>();
		Operand->MyValue = MakeShared<TObject>();
		Operand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
		Operand->MyValue->MyValue = -1;
		Operand->MyOperandType = AsmBasicOperandType::Immidiate;

		TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(U'*');
		Instruct->MyLhs = InBuilder.PopStack();
		Instruct->MyRhs = Operand;

		Operand = MakeShared<TAsmBasicOperand>();
		Operand->MyOperandType = AsmBasicOperandType::Stack;
		Operand->MyValue = MakeShared<TObject>();
		Operand->MyValue->MyType = Instruct->MyLhs->MyValue->MyType;
		InBuilder.PushStack(Operand);
	}
	else if (MyOperator->MyLetter == U"++")
	{
		TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(U'+');
		Instruct->MyLhs = InBuilder.PopStack();
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, MakeShared<TObject>());
		Instruct->MyRhs = Operand;
		Operand->MyValue->MyValue = 1;
		Operand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");

		Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, MakeShared<TObject>());
		Operand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
		Operand->MyValue->MyValue = Instruct->MyLhs->MyValue->MyValue;
		InBuilder.PushStack(Operand);
	}
	else if (MyOperator->MyLetter == U"--")
	{
		TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(U'-');
		Instruct->MyLhs = InBuilder.PopStack();
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, MakeShared<TObject>());
		Instruct->MyRhs = Operand;
		Operand->MyValue->MyValue = 1;
		Operand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");

		Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, MakeShared<TObject>());
		Operand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
		Operand->MyValue->MyValue = Instruct->MyLhs->MyValue->MyValue;
		InBuilder.PushStack(Operand);
	}
	else
	{
		Panic("Invalid operator");
	}
}

void TAstPostfixUnaryNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	Assert(MyExpr);
	MyExpr->BuildIR(InBuilder);
	if (MyOperator->MyLetter == U"++" ||
		MyOperator->MyLetter == U"--")
	{

		TSharedPtr<TAsmBasicOperand> Operand = InBuilder.PopStack();
		TSharedPtr<TAsmBasicOperand> TmpOperand = nullptr;
		/* push stack */
		{
			TSharedPtr<TAsmBasicPushInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicPushInstruct>();
			Instruct->MyPosition = MyOperator;
			Instruct->MyValue = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Stack, Operand->MyValue);
			TmpOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, Instruct->MyValue->MyValue);
			InBuilder.PushStack(Instruct->MyValue);
			TmpOperand->MyName = Instruct->MyValue->MyName;
		}
		/* set value */
		{
			TSharedPtr<TAsmBasicMovInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicMovInstruct>();
			Instruct->MyPosition = MyOperator;
			Instruct->MyLhs = TmpOperand;
			Instruct->MyRhs = Operand;
		}

		TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(MyOperator->MyLetter.GetString().Bytes()[0]);
		Instruct->MyLhs = TmpOperand;

		TSharedPtr<TAsmBasicOperand> RhsOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, MakeShared<TObject>());
		Instruct->MyRhs = RhsOperand;
		RhsOperand->MyValue->MyValue = 1;
		RhsOperand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");

		InBuilder.PushStack(Operand);
	}
	else
	{
		Panic("Invalid operator");
	}
}

void TAstArrayReference::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	Assert(MyIndex);
	MyIndex->BuildIR(InBuilder);
	TSharedPtr<TAsmBasicOperand> Operand = InBuilder.PopStack();
	Operand->MyOperandType = AsmBasicOperandType::Address;
	Operand->MyValue->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
	*(Operand->MyValue->GetInteger()) += MyArrayInfo->MyStackIndex;

	InBuilder.PushStack(Operand);
	InBuilder.CreateInstruct<TAsmBasicPushInstruct>(Operand);
}

void TAstEqualityNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (IsStaticEvaluatable())
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Evaluate());
		InBuilder.PushStack(Operand);
		InBuilder.CreateInstruct<TAsmBasicPushInstruct>(Operand);
		return;
	}

	Assert(MyLhs);
	MyLhs->BuildIR(InBuilder);
	Assert(MyRhs);
	MyRhs->BuildIR(InBuilder);
	TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(MyOperator->MyLetter);
	Instruct->MyRhs = InBuilder.PopStack();
	Instruct->MyLhs = InBuilder.PopStack();
	TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Stack, Instruct->MyLhs->MyValue->MyType);
	InBuilder.PushStack(Operand);
}

void TAstAssignNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicMovInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicMovInstruct>();
	Assert(MyLhs);
	MyLhs->BuildIR(InBuilder);
	Assert(MyRhs);
	MyRhs->BuildIR(InBuilder);
	Instruct->MyRhs = InBuilder.PopStack();
	Instruct->MyLhs = InBuilder.PopStack();
	InBuilder.PushStack(Instruct->MyLhs);
}

void TAstRelationalNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (IsStaticEvaluatable())
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Evaluate());
		InBuilder.PushStack(Operand);
		InBuilder.CreateInstruct<TAsmBasicPushInstruct>(Operand);
		return;
	}

	const char32_t* Operator = MyOperator->MyLetter.GetString().Bytes();
	Assert(MyRhs);
	MyRhs->BuildIR(InBuilder);
	Assert(MyLhs);
	MyLhs->BuildIR(InBuilder);

	TSharedPtr<TAsmBasicOperand> LhsOperand = InBuilder.PopStack();
	TSharedPtr<TAsmBasicOperand> RhsOperand = InBuilder.PopStack();

	TSharedPtr<TAsmBasicBinInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicBinInstruct>(MyOperator->MyLetter);
	Instruct->MyLhs = LhsOperand;
	Instruct->MyRhs = RhsOperand;
	Instruct->MyPosition = MyOperator;
}

void TAstVarNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (TSharedPtr<TBlockEntry> Block = MyBlock.Lock())
	{
		TSharedPtr<TObject> Value = MakeShared<TObject>();
		Value->MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
		Value->MyValue = Block->GetInfo(MyName->MyLetter)->MyStackIndex;
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, Value, MyName->MyLetter);
		InBuilder.PushStack(Operand);
	}
}

void TAstReturnNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (MyExpr)
	{
		MyExpr->BuildIR(InBuilder);
	}
	Lime::size_t ReturnElementOffset = InBuilder.GetStackPos();
	TSharedPtr<TAsmBasicReturnInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicReturnInstruct>();
	Instruct->MyReturnValue = InBuilder.ReferStack(ReturnElementOffset);
	Instruct->MyPosition = MyPosition;
}

void TAstIfNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	Assert(MyEvalExpr);
	MyEvalExpr->BuildIR(InBuilder);

	TSharedPtr<TAsmBasicJumpLabelInstruct> InstructJump = InBuilder.CreateInstruct<TAsmBasicJumpLabelInstruct>();
	InstructJump->MyMode = ConditionCode::Equal;
	InstructJump->MyValue = InBuilder.PopStack();
	InstructJump->MyPosition = MyIfPosition;

	MyTrueExpr->BuildIR(InBuilder);
	if (MyFalseExpr)
	{
		InBuilder.CreateInstruct<TAsmBasicLabelInstruct>(InBuilder.MakeNewLabel());
		MyFalseExpr->BuildIR(InBuilder);
		InBuilder.CreateInstruct<TAsmBasicLabelInstruct>(InBuilder.MakeNewLabel());
	}
	else
	{
		TSharedPtr<TAsmBasicLabelInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicLabelInstruct>();
		Instruct->MyLabelName = InBuilder.MakeNewLabel();
		Instruct->MyPosition = MyElsePosition;
	}
	InstructJump->MyLabelName = InBuilder.GetLastLabelName();
}

void TAstWhileNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicJumpLabelInstruct> JumpCondInstruct = InBuilder.CreateInstruct<TAsmBasicJumpLabelInstruct>(InBuilder.MakeNewLabel(), ConditionCode::Unknown, nullptr);
	JumpCondInstruct->MyPosition = MyPosition;

	TSharedPtr<TAsmBasicLabelInstruct> StartLabelInstruct = InBuilder.CreateInstruct<TAsmBasicLabelInstruct>(InBuilder.MakeNewLabel());
	StartLabelInstruct->MyPosition = MyPosition;

	Assert(MyBlockExpr);
	MyBlockExpr->BuildIR(InBuilder);

	TSharedPtr<TAsmBasicLabelInstruct> CondLabelInstruct = InBuilder.CreateInstruct<TAsmBasicLabelInstruct>(InBuilder.MakeNewLabel());
	CondLabelInstruct->MyPosition = MyPosition;
	JumpCondInstruct->MyLabelName = CondLabelInstruct->MyLabelName;

	Assert(MyEvalExpr);
	MyEvalExpr->BuildIR(InBuilder);

	TSharedPtr<TObject> Value = MakeShared<TObject>(TTypeTable::GetGlobalTable()->GetInfo(U"bool"));
	Value->MyValue = 1;
	TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Value);
	TSharedPtr<TAsmBasicJumpLabelInstruct> JumpInstruct = InBuilder.CreateInstruct<TAsmBasicJumpLabelInstruct>(StartLabelInstruct->MyLabelName, ConditionCode::Equal, Operand);
}

void TAstForNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstFunctionDefinitionNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicLabelInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicLabelInstruct>(MyFunctionName->MyLetter);
	Instruct->MyPosition = MyFunctionName;

	InBuilder.ClearStack();
	InBuilder.SetStackOffset(0);

	for (const Lime::TPair<TSharedPtr<TTypeInfo>, TSharedPtr<TVarInfo>>& Argument : MyArguments)
	{
		TSharedPtr<TObject> Value = MakeShared<TObject>();
		Value->MyType = Argument.first;
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Local, Value, Argument.second->MyName);
		Argument.second->MyStackIndex = InBuilder.GetStackPos();
		InBuilder.PushStack(Operand);
		InBuilder.CreateInstruct<TAsmBasicPushInstruct>(Operand);
	}

	Assert(MyBlockExpr);
	MyBlockExpr->BuildIR(InBuilder);

	InBuilder.ClearStack();
	InBuilder.SetStackOffset(0);
}

void TAstVariableDefinitionNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TVarInfo> Var;
	if (TSharedPtr<TBlockEntry> Block = MyBlock.Lock())
	{
		Var = Block->GetInfo(MyName->MyLetter);
	}

	/* --- push the element as local variable to stack (this memory is never pop until reaching the end of block) --- */
	TSharedPtr<TAsmBasicOperand> LocalOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Local, Var->MyType, MyName->MyLetter);
	if (Var->MyIsArray)
	{
		LocalOperand->MyName = LocalOperand->MyName + U"[0]";
	}
	Var->MyStackIndex = InBuilder.GetStackPos();

	InBuilder.PushStack(LocalOperand);
	InBuilder.CreateInstruct<TAsmBasicPushInstruct>(LocalOperand);

	/* push rest element too if this variable is array */
	for (Lime::size_t Index = 1; Index < MyArrayCount; ++Index)
	{
		/* AsmBasicOperandType::Local means local variable. (never poped until the end of scope) */
		LocalOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Local, Var->MyType, MyName->MyLetter + U'[' + ToUtf32String(Index) + U']');
		InBuilder.PushStack(LocalOperand);
		InBuilder.CreateInstruct<TAsmBasicPushInstruct>(LocalOperand);
	}
	/* --- end push the element as local variable to stack --- */

	/* --- set initial value --- */
	if (MyInitializeExpr)
	{
		if (!MyIsArray)
		{
			MyInitializeExpr->BuildIR(InBuilder);
			TSharedPtr<TAsmBasicMovInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicMovInstruct>();
			Instruct->MyRhs = InBuilder.PopStack();
			Instruct->MyLhs = InBuilder.ReferStack(Var->MyStackIndex);
			Instruct->MyPosition = MyName;
			return;
		}
		/* additional push local memory (set local memory offset at this memory).
		 * this element is poped by the other BuildIR().
		 * 
		 * reason why does not the push to stack at once:
		 * stack order the below, case of array.
		 *       |-----|-----|---------------|---------------|
		 * front | [0] | [1] | offset of [1] | offset of [0] | back
		 *       |-----|-----|---------------|---------------|
		 * the others BuildIR() use from back. in addition, we should follow the below rules.
		 * - local memory (for example: [0] and [1]) should not pop until the end of scope
		 * - need set initial value to each memory in order from [0]
		 * - initial value is need to set to local memory(for example: [0] and [1])
		 */
		int64_t StackOffset = Var->MyStackIndex;
		for (int64_t Index = 0; Index < MyArrayCount; ++Index)
		{
			/* AsmBasicOperandType::Address means `offset of [n]` */
			LocalOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, Var->MyType, MyName->MyLetter);
			LocalOperand->MyValue->MyValue = StackOffset;

			TSharedPtr<TAsmBasicMovInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicMovInstruct>();
			Instruct->MyLhs = InBuilder.ReferStack(StackOffset + Index);
			Instruct->MyPosition = MyName;
			Instruct->MyRhs = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Var->MyObject[Index]);
		}
		// MyInitializeExpr->BuildIR(InBuilder);
	}
	/* --- end set initial value --- */
}

void TAstInitializerListNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	for (Lime::TArray<TSharedPtr<TAstBaseNode>>::const_iterator Itr = MyLists.end() - 1; Itr > MyLists.begin(); --Itr)
	{
		Assert(*Itr);
		(*Itr)->BuildIR(InBuilder);
	}
	if (MyLists.size() > 0)
	{
		(*MyLists.begin())->BuildIR(InBuilder);
	}
}

void TAstFunctionCallNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	{
		/* return value */
		TSharedPtr<TAsmBasicPushInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicPushInstruct>();
		TSharedPtr<TObject> Value = MakeShared<TObject>(TTypeTable::GetGlobalTable()->GetInfo(U"int"));
		Instruct->MyPosition = MyPosition;
		Instruct->MyValue = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Local, Value, U"ret_value" + TUtf32String(U" ;return value register;"));
		InBuilder.PushStack(Instruct->MyValue);
	}

	Lime::size_t StackOffset = InBuilder.GetStackOffset();
	Lime::size_t LastStackOffset = InBuilder.GetStackPos();
	InBuilder.SetStackOffset(StackOffset);


	if (MyArguments.size())
	{
		for (auto Itr = MyArguments.end() - 1;; --Itr)
		{
			if (Itr->first->IsStaticEvaluatable())
			{
				TSharedPtr<TAsmBasicPushInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicPushInstruct>();
				Instruct->MyPosition = MyPosition;
				Instruct->MyValue = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Itr->first->Evaluate());
				InBuilder.PushStack(Instruct->MyValue);
			}
			else
			{
				Itr->first->BuildIR(InBuilder);
			}
			if (Itr == MyArguments.begin())
			{
				break;
			}
		}
	}

	InBuilder.CreateInstruct<TAsmBasicCallInstruct>(MyFunction->MyName);

	while (InBuilder.GetStackPos() > LastStackOffset)
	{
		InBuilder.RemoveStack();
	}

	InBuilder.SetStackOffset(StackOffset);
}

void TAstAsmNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}