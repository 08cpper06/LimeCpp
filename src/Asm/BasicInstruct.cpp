#include "Asm/BasicInstruct.hpp"


TUtf32String TAsmBasicMovInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix + U"mov\t";
	Str += ToUtf32String(MyLhs->MyOperandType) + U'\t';
	Str += MyLhs->MyValue->MyType->MyName + U'\t';
	Str += MyLhs->MyName;

	Str += ToUtf32String(MyRhs->MyOperandType) + U'\t';
	Str += MyRhs->MyValue->MyType->MyName;
	if (MyRhs->MyOperandType == AsmBasicOperandType::Stack)
	{
		Str += MyRhs->MyName + U'\t';
	}
	else if (MyRhs->MyOperandType == AsmBasicOperandType::Immidiate)
	{
		Str += ToUtf32String(*MyRhs->MyValue) + U'\t';
	}
	else
	{
		Str += U"unknown\t";
	}

	return Str + U'\n';
}

TUtf32String TAsmBasicAddInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix + U"add\t";
	Str += ToUtf32String(MyLhs->MyOperandType) + U'\t';
	Str += MyLhs->MyValue->MyType->MyName + U'\t';
	if (MyLhs->MyOperandType == AsmBasicOperandType::Stack)
	{
		Str += MyLhs->MyName + U'\t';
	}
	else if (MyLhs->MyOperandType == AsmBasicOperandType::Immidiate)
	{
		Str += ToUtf32String(*MyLhs->MyValue) + U'\t';
	}
	else
	{
		Str += U"unknown\t";
	}

	Str += ToUtf32String(MyRhs->MyOperandType) + U'\t';
	Str += MyRhs->MyValue->MyType->MyName;
	if (MyRhs->MyOperandType == AsmBasicOperandType::Stack)
	{
		Str += MyRhs->MyName + U'\t';
	}
	else if (MyRhs->MyOperandType == AsmBasicOperandType::Immidiate)
	{
		Str += ToUtf32String(*MyRhs->MyValue) + U'\t';
	}
	else
	{
		Str += U"unknown\t";
	}

	return Str + U'\n';
}

TUtf32String TAsmBasicLabelInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	return MyLabelName + U":\n";
}

TUtf32String TAsmBasicReturnInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix + U"ret\t";
	Str += ToUtf32String(MyReturnValue->MyOperandType) + U'\t';
	Str += MyReturnValue->MyValue->MyType->MyName + U'\t';
	if (MyReturnValue->MyOperandType == AsmBasicOperandType::Stack)
	{
		Str += MyReturnValue->MyName + U'\t';
	}
	else if (MyReturnValue->MyOperandType == AsmBasicOperandType::Immidiate)
	{
		Str += ToUtf32String(*MyReturnValue->MyValue) + U'\t';
	}
	else
	{
		Str += U"unknown\t";
	}

	return Str + U'\n';
}