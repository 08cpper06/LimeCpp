#pragma once


#include "../String/HashString.hpp"
#include "../Parse/TypeTable.hpp"
#include "../Parse/Object.hpp"
#include "../Parse/ASTNode.hpp"


enum AsmBasicOperandType {
	Unknown,
	Immidiate,
	Address,
	Stack,
	Local,
	Label,
};

inline TUtf32String ToUtf32String(AsmBasicOperandType InType) noexcept
{
	switch (InType) {
	case AsmBasicOperandType::Immidiate:
		return U"immidiate";
	case AsmBasicOperandType::Address:
		return U"address";
	case AsmBasicOperandType::Stack:
		return U"stack";
	case AsmBasicOperandType::Local:
		return U"local";
	case AsmBasicOperandType::Label:
		return U"label";
	}
	return U"unknown";
}

struct TAsmBasicOperand {
public:
	TAsmBasicOperand() noexcept = default;

	TAsmBasicOperand(AsmBasicOperandType InType, TSharedPtr<TObject> InValue, THashString InName = THashString()) noexcept :
		MyOperandType(InType),
		MyValue(InValue),
		MyName(InName)
	{}
	TAsmBasicOperand(AsmBasicOperandType InType, TSharedPtr<TTypeInfo> InValue, THashString InName = THashString()) noexcept :
		MyOperandType(InType),
		MyName(InName)
	{
		MyValue = MakeShared<TObject>();
		MyValue->MyType = InValue;
	}

public:
	AsmBasicOperandType MyOperandType { AsmBasicOperandType::Unknown };
	TSharedPtr<TObject> MyValue;
	THashString MyName;
};

class TAsmBasicInstruct {
public:
	virtual ~TAsmBasicInstruct() = default;
	virtual TAsmBasicInstruct* StaticClass() const noexcept = 0;
	virtual TUtf32String GetInfoString(TUtf32String InPrefix = U"") const noexcept = 0;

	TUtf32String GetOperandName(TSharedPtr<TAsmBasicOperand> InTarget) const noexcept
	{
		TUtf32String Str;
		if (InTarget->MyOperandType == AsmBasicOperandType::Stack ||
			InTarget->MyOperandType == AsmBasicOperandType::Local)
		{
			Str += U'#' + InTarget->MyName + U'\t';
		}
		else if (InTarget->MyOperandType == AsmBasicOperandType::Immidiate)
		{
			Str += ToUtf32String(*InTarget->MyValue) + U'\t';
		}
		else
		{
			Str += U"unknown\t";
		}
		return Str;
	}

public:
	Lime::TTokenIterator MyPosition;
};

#define BASIC_INSTRUCT_BODY_CLASS(ClassName) ClassName() = default; ~ClassName() = default; \
virtual TAsmBasicInstruct* StaticClass() const noexcept override \
{ \
	static ClassName StaticInstance; \
	return &StaticInstance; \
} \
TUtf32String GetInfoString(TUtf32String InPrefix = U"") const noexcept override


class TAsmBasicMovInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicMovInstruct);

public:
	TSharedPtr<TAsmBasicOperand> MyLhs;
	TSharedPtr<TAsmBasicOperand> MyRhs;
};

class TAsmBasicBinInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicBinInstruct);

	TAsmBasicBinInstruct(THashString InOperaotr) :
		MyOperator(InOperaotr),
		MyLhs(nullptr),
		MyRhs(nullptr)
	{}

public:
	THashString MyOperator;
	TSharedPtr<TAsmBasicOperand> MyLhs;
	TSharedPtr<TAsmBasicOperand> MyRhs;
};

class TAsmBasicLabelInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicLabelInstruct);

	TAsmBasicLabelInstruct(THashString InLabel) :
		MyLabelName(InLabel)
	{}
	
public:
	THashString MyLabelName;
};

enum class ConditionCode {
	Unknown,
	Equal,
	NotEqual,
	Negative,
	NonNegative,
	Greater,
	GreaterEuqal,
	Less,
	LessEqual,
	Above,
	AboveEqual,
	Below,
	BelowEqual,
};

inline TUtf32String ToUtf32String(ConditionCode InMode) noexcept
{
	TUtf32String Str;
	switch (InMode) {
	case ConditionCode::Unknown:		Str += U""; break;
	case ConditionCode::Equal:			Str += U"e"; break;
	case ConditionCode::NotEqual:		Str += U"ne"; break;
	case ConditionCode::Negative:		Str += U"s"; break;
	case ConditionCode::NonNegative:	Str += U"ns"; break;
	case ConditionCode::Greater:		Str += U"g"; break;
	case ConditionCode::GreaterEuqal:	Str += U"ge"; break;
	case ConditionCode::Less:			Str += U"l"; break;
	case ConditionCode::LessEqual:		Str += U"le"; break;
	case ConditionCode::Above:			Str += U"a"; break;
	case ConditionCode::AboveEqual:		Str += U"ae"; break;
	case ConditionCode::Below:			Str += U"b"; break;
	case ConditionCode::BelowEqual:		Str += U"be"; break;
	}
	return Str;
}

class TAsmBasicJumpLabelInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicJumpLabelInstruct);

	TAsmBasicJumpLabelInstruct(THashString InLabel, ConditionCode InMode, TSharedPtr<TAsmBasicOperand> InCmpValue) :
		MyLabelName(InLabel),
		MyMode(InMode),
		MyValue(InCmpValue)
	{}
	
public:
	THashString MyLabelName;
	ConditionCode MyMode;
	TSharedPtr<TAsmBasicOperand> MyValue;
};

class TAsmBasicReturnInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicReturnInstruct);

	TAsmBasicReturnInstruct(TSharedPtr<TAsmBasicOperand> InValue) :
		MyReturnValue(InValue)
	{}

public:
	TSharedPtr<TAsmBasicOperand> MyReturnValue;
};
