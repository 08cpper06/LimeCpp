#pragma once


#include "../Std.hpp"

#include "Tokenize/Token.hpp"
#include "../String/Utf8String.hpp"
#include "../String/Utf32String.hpp"
#include "TypeTable.hpp"
#include "VariableTable.hpp"


namespace Lime {
	
	typedef decltype(Lime::TList<TToken>().cbegin()) TTokenIterator;

}


class TAstBaseNode {
public:
	virtual ~TAstBaseNode() = default;

	virtual TAstBaseNode* StaticClass() const = 0;
	virtual TUtf32String GetInfoString(TUtf32String InPrefix) const = 0;	
	virtual TOption<THashString> EvaluateType() const noexcept = 0;
};

#define AST_BODY_CLASS(ClassName) ~ClassName() = default; \
virtual TAstBaseNode* StaticClass() const override \
{ \
	static ClassName StaticInstance; \
	return &StaticInstance; \
} \
TUtf32String GetInfoString(TUtf32String InPrefix) const override; \
friend class Parser

#define DEFINE_EVALUATE_TYPE(Value) TOption<THashString> EvaluateType() const noexcept override { return Value; }
#define IMPLEMENT_EVALUATE_TYPE() TOption<THashString> EvaluateType() const noexcept override


class TAstErrorNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstErrorNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	Lime::TTokenIterator MyPosition;
	TUtf32String MyMessage;

	friend class TParseResult;
};

class TAstWarningNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstWarningNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	Lime::TTokenIterator MyPosition;
	TUtf32String MyMessage;

	friend class TParseResult;
	friend class TAstFunctionCallNode;
};

class TAstBlockNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstBlockNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	Lime::TList<TSharedPtr<TAstBaseNode>> MyNodes;
	TUtf32String MyBlockName;
};

class TAstValNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstValNode);

	DEFINE_EVALUATE_TYPE(MyType.MyName);

CLASS_PRIVATE:
	Lime::TTokenIterator MyStartItr;
	Lime::TTokenIterator MyEndItr;
	TTypeInfo MyType;

	friend class TAstFunctionCallNode;
};

class TAstAddSubNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstAddSubNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if ((!MyLhs || !MyLhs->EvaluateType()) ||
			(!MyRhs || !MyRhs->EvaluateType()))
		{
			return DefaultErrorType::Error;
		}
		return MyLhsType.IsEvaluatableExpr(MyRhsType);
	}

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TTypeInfo MyLhsType;
	TSharedPtr<TAstBaseNode> MyRhs;
	TTypeInfo MyRhsType;
	Lime::TTokenIterator MyOperator;
};

class TAstMulDivNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstMulDivNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if ((!MyLhs || !MyLhs->EvaluateType()) ||
			(!MyRhs || !MyRhs->EvaluateType()))
		{
			return DefaultErrorType::Error;
		}
		return MyLhsType.IsEvaluatableExpr(MyRhsType);
	}

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TTypeInfo MyLhsType;
	TSharedPtr<TAstBaseNode> MyRhs;
	TTypeInfo MyRhsType;
	Lime::TTokenIterator MyOperator;
};

class TAstExprNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstExprNode);

	DEFINE_EVALUATE_TYPE(MyNode ? MyNode->EvaluateType() : DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyNode;
};

class TAstParenthessNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstParenthessNode);

	DEFINE_EVALUATE_TYPE(MyExpr && !MyError ? MyExpr->EvaluateType() : DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	TSharedPtr<TAstErrorNode> MyError;
};

class TAstUnaryNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstUnaryNode);

	DEFINE_EVALUATE_TYPE(MyExpr ? MyExpr->EvaluateType() : DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	Lime::TTokenIterator MyOperator;
};

class TAstEqualityNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstEqualityNode);

	DEFINE_EVALUATE_TYPE(U"bool");

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstAssignNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstAssignNode);

	DEFINE_EVALUATE_TYPE(MyLhs ? MyLhs->EvaluateType() : DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
};

class TAstRelationalNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstRelationalNode);

	DEFINE_EVALUATE_TYPE(U"bool");

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstVarNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstVarNode);

	DEFINE_EVALUATE_TYPE(MyType.MyName);

CLASS_PRIVATE:
	Lime::TTokenIterator MyName;
	TWeakPtr<TBlockEntry> MyBlock;
	TTypeInfo MyType;

	friend class TAstFunctionCallNode;
};

class TAstReturnNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstReturnNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
};

class TAstIfNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstIfNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyEvalExpr;
	TSharedPtr<TAstBaseNode> MyTrueExpr;
	TSharedPtr<TAstBaseNode> MyFalseExpr;
};

class TAstWhileNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstWhileNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyEvalExpr;
	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstForNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstForNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyInitExpr;
	TSharedPtr<TAstBaseNode> MyCondExpr;
	TSharedPtr<TAstBaseNode> MyUpdateExpr;
	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstFunctionDefinitionNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstFunctionDefinitionNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	Lime::TTokenIterator MyFunctionName;
	TTypeInfo MyReturnType;

	TSharedPtr<TAstBaseNode> MyBlockExpr;
	Lime::TArray<Lime::TPair<TTypeInfo, TVarInfo>> MyArguments;
};

class TAstVariableDefinitionNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstVariableDefinitionNode);

	DEFINE_EVALUATE_TYPE(DefaultErrorType::Error);

CLASS_PRIVATE:
	Lime::TTokenIterator MyName;
	TWeakPtr<TBlockEntry> MyBlock;
	TTypeInfo MyType;
	bool MyIsArray;
	size_t MyArrayCount;

	TSharedPtr<TAstBaseNode> MyInitializeExpr;
};

class TAstInitializerListNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstInitializerListNode);

	DEFINE_EVALUATE_TYPE(MyType.MyName);

CLASS_PRIVATE:
	TTypeInfo MyType;
	Lime::TArray<TSharedPtr<TAstBaseNode>> MyLists;
	TSharedPtr<TAstErrorNode> MyError;
};

class TAstFunctionCallNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstFunctionCallNode);

	DEFINE_EVALUATE_TYPE(MyFunction.MyReturnType);

CLASS_PRIVATE:
	Lime::TArray<Lime::TPair<TSharedPtr<TAstBaseNode>, TSharedPtr<TAstBaseNode>>> MyArguments;
	TTypeInfo MyFunction;
	TSharedPtr<TAstErrorNode> MyError;
};