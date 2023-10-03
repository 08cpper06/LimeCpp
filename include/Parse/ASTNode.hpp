#pragma once


#include "../Std.hpp"

#include "Tokenize/Token.hpp"
#include "../String/Utf8String.hpp"
#include "../String/Utf32String.hpp"
#include "VariableTypeTable.hpp"
#include "VariableTable.hpp"


namespace Lime {
	
	typedef decltype(Lime::TList<TToken>().cbegin()) TTokenIterator;

}


class TAstBaseNode {
public:
	virtual ~TAstBaseNode() = default;

	virtual TAstBaseNode* StaticClass() const = 0;
	virtual TUtf32String GetInfoString(TUtf32String InPrefix) const = 0;	
};

#define AST_BODY_CLASS(ClassName) ~ClassName() = default; \
virtual TAstBaseNode* StaticClass() const override \
{ \
	static ClassName StaticInstance; \
	return &StaticInstance; \
} \
TUtf32String GetInfoString(TUtf32String InPrefix) const override; \
friend class Parser


class TAstErrorNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstErrorNode);

CLASS_PRIVATE:
	Lime::TTokenIterator MyPosition;
	TUtf32String MyMessage;
};

class TAstBlockNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstBlockNode);

CLASS_PRIVATE:
	Lime::TList<TSharedPtr<TAstBaseNode>> MyNodes;
	TUtf32String MyBlockName;
};

enum class ValueType {
	Unknown,
	Int32,
	Float,
};

inline TUtf32String ToUtf32String(ValueType InType) noexcept
{
	switch (InType) {
	case ValueType::Int32:
		return U"int";
	case ValueType::Float:
		return U"float";
	}
	return U"Unknown";
}

inline TUtf8String ToUtf8String(ValueType InType) noexcept
{
	switch (InType) {
	case ValueType::Int32:
		return u8"int";
	case ValueType::Float:
		return u8"float";
	}
	return u8"Unknown";
}

class TAstValNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstValNode);

CLASS_PRIVATE:
	Lime::TTokenIterator MyStartItr;
	Lime::TTokenIterator MyEndItr;
	ValueType MyType;
};

class TAstAddSubNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstAddSubNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstMulDivNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstMulDivNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstExprNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstExprNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyNode;
};

class TAstParenthessNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstParenthessNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	TSharedPtr<TAstErrorNode> MyError;
};

class TAstUnaryNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstUnaryNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	Lime::TTokenIterator MyOperator;
};

class TAstEqualityNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstEqualityNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstAssignNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstAssignNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
};

class TAstRelationalNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstRelationalNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstVarNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstVarNode);

CLASS_PRIVATE:
	Lime::TTokenIterator MyName;
	TWeakPtr<TBlockEntry> MyBlock;
	TVarTypeInfo MyType;
};

class TAstReturnNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstReturnNode);

CLASS_PRIVATE:
	Lime::TTokenIterator MyValue;
	TSharedPtr<TAstBaseNode> MyExpr;
};

class TAstIfNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstIfNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyEvalExpr;
	TSharedPtr<TAstBaseNode> MyTrueExpr;
	TSharedPtr<TAstBaseNode> MyFalseExpr;
};

class TAstWhileNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstWhileNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyEvalExpr;
	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstForNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstForNode);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyInitExpr;
	TSharedPtr<TAstBaseNode> MyCondExpr;
	TSharedPtr<TAstBaseNode> MyUpdateExpr;
	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstFunctionDefinitionNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstFunctionDefinitionNode);

CLASS_PRIVATE:
	Lime::TTokenIterator MyFunctionName;
	TVarTypeInfo MyReturnType;

	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstVariableDefinitionNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstVariableDefinitionNode);

CLASS_PRIVATE:
	Lime::TTokenIterator MyName;
	TWeakPtr<TBlockEntry> MyBlock;
	TVarTypeInfo MyType;
	bool MyIsArray;
	size_t MyArrayCount;

	TSharedPtr<TAstBaseNode> MyInitializeExpr;
};

class TAstInitializerListNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstInitializerListNode);

CLASS_PRIVATE:
	TVarTypeInfo MyType;
	Lime::TArray<TSharedPtr<TAstBaseNode>> MyLists;
	TSharedPtr<TAstErrorNode> MyError;
};