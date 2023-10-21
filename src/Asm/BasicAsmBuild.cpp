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
	int64_t Pos = InBuilder.GetStackPos();
	for (TSharedPtr<TAstBaseNode> Node : MyNodes)
	{
		Node->BuildIR(InBuilder);
	}
	while (Pos < InBuilder.GetStackPos())
	{
		InBuilder.PopStack();
	}
}

void TAstValNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicOperand> Ptr = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, MyValue);
	InBuilder.PushStack(Ptr);
}

void TAstStringValNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstAddSubNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (IsStaticEvaluatable())
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Evaluate());
		InBuilder.PushStack(Operand);
		return;
	}

	MyRhs->BuildIR(InBuilder);
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
		return;
	}

	MyRhs->BuildIR(InBuilder);
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
}

void TAstPostfixUnaryNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstArrayReference::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstEqualityNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstAssignNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstRelationalNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (IsStaticEvaluatable())
	{
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Immidiate, Evaluate());
		InBuilder.PushStack(Operand);
		return;
	}

	const char32_t* Operator = MyOperator->MyLetter.GetString().Bytes();
	if (*Operator == U'>' || MyOperator->MyLetter == U">=")
	{
		MyRhs->BuildIR(InBuilder);
		MyLhs->BuildIR(InBuilder);
	}
	else if (*Operator == U'<' || MyOperator->MyLetter == U"<=")
	{
		MyLhs->BuildIR(InBuilder);
		MyRhs->BuildIR(InBuilder);
	}

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
	TSharedPtr<TAsmBasicReturnInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicReturnInstruct>(InBuilder.PopStack());
	Instruct->MyPosition = MyPosition;
}

void TAstIfNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	if (MyEvalExpr)
	{
		MyEvalExpr->BuildIR(InBuilder);
	}

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
}

void TAstForNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstFunctionDefinitionNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicLabelInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicLabelInstruct>(MyFunctionName->MyLetter);
	Instruct->MyPosition = MyFunctionName;

	if (MyBlockExpr)
	{
		MyBlockExpr->BuildIR(InBuilder);
	}
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
	Var->MyStackIndex = InBuilder.GetStackPos();
	InBuilder.PushStack(LocalOperand);

	/* push rest element too if this variable is array */
	for (Lime::size_t Index = 1; Index <= MyArrayCount; ++Index)
	{
		/* AsmBasicOperandType::Local means local variable. (never poped until the end of scope) */
		LocalOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Local, Var->MyType, MyName->MyLetter);
		InBuilder.PushStack(LocalOperand);
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
		for (int64_t Index = MyArrayCount; Index >= 0; --Index)
		{
			/* AsmBasicOperandType::Address means `offset of [n]` */
			LocalOperand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, Var->MyType, MyName->MyLetter);
			LocalOperand->MyValue->MyValue = StackOffset;
			StackOffset += Var->MyType->MyByteSize;
			InBuilder.PushStack(LocalOperand);
		}
		MyInitializeExpr->BuildIR(InBuilder);
	}
	/* --- end set initial value --- */
}

void TAstInitializerListNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	for (Lime::TArray<TSharedPtr<TAstBaseNode>>::const_iterator Itr = MyLists.end() - 1; Itr >= MyLists.begin(); --Itr)
	{
		(*Itr)->BuildIR(InBuilder);
	}
}

void TAstFunctionCallNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	for (Lime::TPair<TSharedPtr<TAstBaseNode>, TSharedPtr<TAstBaseNode>> Node : MyArguments)
	{
		Node.first->BuildIR(InBuilder);
	}
	TSharedPtr<TAsmBasicJumpLabelInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicJumpLabelInstruct>(MyFunction->MyName, ConditionCode::Unknown, nullptr);
}

void TAstAsmNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}