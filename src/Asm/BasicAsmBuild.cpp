#include "Parse/ASTNode.hpp"
#include "Asm/BasicInstructBuilder.hpp"


void TAstErrorNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstWarningNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstBlockNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	for (TSharedPtr<TAstBaseNode> Node : MyNodes)
	{
		Node->BuildIR(InBuilder);
	}
}

void TAstValNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicOperand> Ptr = MakeShared<TAsmBasicOperand>();
	Ptr->MyOperandType = AsmBasicOperandType::Immidiate;
	Ptr->MyValue = MyValue;
	InBuilder.PushStack(Ptr);
}

void TAstStringValNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstAddSubNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstMulDivNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstExprNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstParenthessNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
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
}

void TAstVarNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstReturnNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicReturnInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicReturnInstruct>();
	Instruct->MyPosition = MyPosition;
	if (MyExpr)
	{
		MyExpr->BuildIR(InBuilder);
	}
	Instruct->MyReturnValue = InBuilder.PopStack();
}

void TAstIfNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstWhileNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstForNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstFunctionDefinitionNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
	TSharedPtr<TAsmBasicLabelInstruct> Instruct = InBuilder.CreateInstruct<TAsmBasicLabelInstruct>();
	Instruct->MyPosition = MyFunctionName;
	Instruct->MyLabelName = MyFunctionName->MyLetter;

	if (MyBlockExpr)
	{
		MyBlockExpr->BuildIR(InBuilder);
	}
}

void TAstVariableDefinitionNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstInitializerListNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstFunctionCallNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}

void TAstAsmNode::BuildIR(TAsmBasicBuilder& InBuilder) const noexcept
{
}