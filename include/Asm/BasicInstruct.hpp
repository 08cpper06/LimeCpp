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
	case AsmBasicOperandType::Label:
		return U"label";
	}
	return U"unknown";
}

struct TAsmBasicOperand {
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

class TAsmBasicAddInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicAddInstruct);

public:
	TSharedPtr<TAsmBasicOperand> MyLhs;
	TSharedPtr<TAsmBasicOperand> MyRhs;
};

class TAsmBasicLabelInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicLabelInstruct);
	
public:
	THashString MyLabelName;
};

class TAsmBasicReturnInstruct : public TAsmBasicInstruct {
public:
	BASIC_INSTRUCT_BODY_CLASS(TAsmBasicReturnInstruct);

public:
	TSharedPtr<TAsmBasicOperand> MyReturnValue;
};
