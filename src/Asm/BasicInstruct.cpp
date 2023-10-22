#include "Asm/BasicInstruct.hpp"


TUtf32String TAsmBasicMovInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix + U"mov\t";
	Str += ToUtf32String(MyLhs->MyOperandType) + U'\t';
	Str += MyLhs->MyValue->MyType->MyName + U'\t';
	Str += GetOperandName(MyLhs);

	Str += ToUtf32String(MyRhs->MyOperandType) + U'\t';
	Str += MyRhs->MyValue->MyType->MyName + U'\t';
	Str += GetOperandName(MyRhs);

	return Str + U'\n';
}

TUtf32String TAsmBasicBinInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix;
	bool IsSwap = false;
	switch (MyOperator.MyHashValue) {
		case U'+': Str += U"add\t"; break;
		case U'-': Str += U"sub\t"; break;
		case U'*': Str += U"mul\t"; break;
		case U'/': Str += U"div\t"; break;
		case U'>': IsSwap = true; [[fallthrough]];
		case U'<': Str += U"setl\t"; break;
		default: {
			if (MyOperator == U"<=" ||
				MyOperator == U">=")
			{
				Str += U"setle\t";
				IsSwap = MyOperator == U">=";
			}
			else if (MyOperator == U"==")
			{
				Str += U"sete\t";
			}
			else if (MyOperator == U"!=")
			{
				Str += U"setne\t";
			}
			break;
		}
	}

	if (IsSwap)
	{
		Str += ToUtf32String(MyRhs->MyOperandType) + U'\t';
		Str += MyRhs->MyValue->MyType->MyName + U'\t';
		Str += GetOperandName(MyRhs);

		Str += ToUtf32String(MyLhs->MyOperandType) + U'\t';
		Str += MyLhs->MyValue->MyType->MyName + U'\t';
		Str += GetOperandName(MyLhs);
	}
	else
	{
		Str += ToUtf32String(MyLhs->MyOperandType) + U'\t';
		Str += MyLhs->MyValue->MyType->MyName + U'\t';
		Str += GetOperandName(MyLhs);

		Str += ToUtf32String(MyRhs->MyOperandType) + U'\t';
		Str += MyRhs->MyValue->MyType->MyName + U'\t';
		Str += GetOperandName(MyRhs);
	}

	return Str + U'\n';
}

TUtf32String TAsmBasicLabelInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	return MyLabelName + U":\n";
}

TUtf32String TAsmBasicJumpLabelInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix;
	TUtf32String Postfix = ToUtf32String(MyMode);
	if (Postfix.CharCount() > 0)
	{
		Str += U'j' + Postfix;
	}
	else
	{
		Str = U"jump";
	}
	Str += U'\t' + GetOperandName(MyValue);
	Str += U'\t' + MyLabelName;
	return Str + U'\n';
}

TUtf32String TAsmBasicReturnInstruct::GetInfoString(TUtf32String InPrefix) const noexcept
{
	TUtf32String Str = InPrefix + U"ret\t";
	Str += ToUtf32String(MyReturnValue->MyOperandType) + U'\t';
	Str += MyReturnValue->MyValue->MyType->MyName + U'\t';
	Str += GetOperandName(MyReturnValue);

	return Str + U'\n';
}