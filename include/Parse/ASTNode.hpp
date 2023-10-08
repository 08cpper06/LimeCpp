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
	virtual TSharedPtr<TTypeInfo> EvaluateType() const noexcept = 0;
	virtual bool IsStaticEvaluatable() const noexcept = 0;
	virtual TSharedPtr<TObject> Evaluate() const noexcept = 0;
};

#define AST_BODY_CLASS(ClassName) ~ClassName() = default; \
virtual TAstBaseNode* StaticClass() const override \
{ \
	static ClassName StaticInstance; \
	return &StaticInstance; \
} \
TUtf32String GetInfoString(TUtf32String InPrefix) const override; \
TSharedPtr<TObject> Evaluate() const noexcept override; \
friend class Parser

#define DEFINE_EVALUATE_TYPE(Value) TSharedPtr<TTypeInfo> EvaluateType() const noexcept override { return Value; }
#define IMPLEMENT_EVALUATE_TYPE() TSharedPtr<TTypeInfo> EvaluateType() const noexcept override

#define DEFINE_STATIC_EVALUATABLE(Value) bool IsStaticEvaluatable() const noexcept override { return Value; }
#define IMPLEMENT_STATIC_EVALUATABLE() bool IsStaticEvaluatable() const noexcept override

class TAstErrorNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstErrorNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(true);

CLASS_PRIVATE:
	Lime::TTokenIterator MyPosition;
	TUtf32String MyMessage;

	friend class TParseResult;
};

class TAstWarningNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstWarningNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(true);

CLASS_PRIVATE:
	Lime::TTokenIterator MyPosition;
	TUtf32String MyMessage;

	friend class TParseResult;

	friend class TAstReturnNode;
	friend class TAstFunctionCallNode;
};

class TAstReturnNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstReturnNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyExpr)
		{
			return MyExpr->EvaluateType();
		}
		return TTypeTable::GetGlobalTable()->GetInfo(THashString(U"void"));
	}

	DEFINE_STATIC_EVALUATABLE(MyExpr->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	TSharedPtr<TAstWarningNode> MyWarning;
	Lime::TTokenIterator MyPosition;

	friend class TAstReturnNode;
};

class TAstBlockNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstBlockNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	IMPLEMENT_STATIC_EVALUATABLE()
	{
		bool Ret = true;
		for (TSharedPtr<TAstBaseNode> Node : MyNodes)
		{
			Ret &= Node->IsStaticEvaluatable();
		}
		return Ret;
	}

CLASS_PRIVATE:
	Lime::TList<TSharedPtr<TAstBaseNode>> MyNodes;
	Lime::TArray<TSharedPtr<TAstReturnNode>> ReturnList;
	TUtf32String MyBlockName;
};

class TAstValNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstValNode);

	DEFINE_EVALUATE_TYPE(MyType);

	DEFINE_STATIC_EVALUATABLE(true);

CLASS_PRIVATE:
	Lime::TTokenIterator MyStartItr;
	Lime::TTokenIterator MyEndItr;
	TSharedPtr<TTypeInfo> MyType;

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
			return nullptr;
		}
		if (TOption<THashString> TypeName = MyLhsType->IsEvaluatableExpr(MyRhs->EvaluateType()))
		{
			return TTypeTable::GetGlobalTable()->GetInfo(*TypeName);
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyLhs->IsStaticEvaluatable() && MyRhs->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TTypeInfo> MyLhsType;
	TSharedPtr<TAstBaseNode> MyRhs;
	TSharedPtr<TTypeInfo> MyRhsType;
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
			return nullptr;
		}
		if (TOption<THashString> TypeName = MyLhsType->IsEvaluatableExpr(MyRhs->EvaluateType()))
		{
			return TTypeTable::GetGlobalTable()->GetInfo(*TypeName);
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyLhs->IsStaticEvaluatable() && MyRhs->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TTypeInfo> MyLhsType;
	TSharedPtr<TAstBaseNode> MyRhs;
	TSharedPtr<TTypeInfo> MyRhsType;
	Lime::TTokenIterator MyOperator;
};

class TAstExprNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstExprNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyNode)
		{
			return MyNode->EvaluateType();
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyNode->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyNode;
};

class TAstParenthessNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstParenthessNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyExpr && !MyError)
		{
			return MyExpr->EvaluateType();
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyExpr ? MyExpr->IsStaticEvaluatable() : true);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	TSharedPtr<TAstErrorNode> MyError;
};

class TAstPrefixUnaryNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstPrefixUnaryNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyExpr)
		{
			return MyExpr->EvaluateType();
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyExpr->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyExpr;
	Lime::TTokenIterator MyOperator;
};

class TAstPostfixUnaryNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstPostfixUnaryNode );

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyExpr)
		{
			return MyExpr->EvaluateType();
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyExpr->IsStaticEvaluatable());

CLASS_PRIVATE:
	Lime::TTokenIterator MyOperator;
	TSharedPtr<TAstBaseNode> MyExpr;
};

class TAstArrayReference : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstArrayReference);

	DEFINE_EVALUATE_TYPE(MyArrayInfo->MyType);

	DEFINE_STATIC_EVALUATABLE(MyIndex->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TVarInfo> MyArrayInfo;
	TSharedPtr<TAstBaseNode> MyIndex;
	TSharedPtr<TAstBaseNode> MyError;
};

class TAstEqualityNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstEqualityNode);

	DEFINE_EVALUATE_TYPE(TTypeTable::GetGlobalTable()->GetInfo(U"bool"));

	DEFINE_STATIC_EVALUATABLE(MyLhs->IsStaticEvaluatable() && MyRhs->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstAssignNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstAssignNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyLhs)
		{
			return MyLhs->EvaluateType();
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(MyRhs->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
};

class TAstRelationalNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstRelationalNode);

	DEFINE_EVALUATE_TYPE(TTypeTable::GetGlobalTable()->GetInfo(U"bool"));

	DEFINE_STATIC_EVALUATABLE(MyLhs->IsStaticEvaluatable() && MyRhs->IsStaticEvaluatable());

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyLhs;
	TSharedPtr<TAstBaseNode> MyRhs;
	Lime::TTokenIterator MyOperator;
};

class TAstVarNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstVarNode);

	DEFINE_EVALUATE_TYPE(MyType);

	DEFINE_STATIC_EVALUATABLE(false); /* TODO : Check whether variable is constant? */

CLASS_PRIVATE:
	Lime::TTokenIterator MyName;
	TWeakPtr<TBlockEntry> MyBlock;
	TSharedPtr<TTypeInfo> MyType;

	friend class TAstFunctionCallNode;
};

class TAstIfNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstIfNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(false);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyEvalExpr;
	TSharedPtr<TAstBaseNode> MyTrueExpr;
	TSharedPtr<TAstBaseNode> MyFalseExpr;
};

class TAstWhileNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstWhileNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(false);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyEvalExpr;
	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstForNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstForNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(false);

CLASS_PRIVATE:
	TSharedPtr<TAstBaseNode> MyInitExpr;
	TSharedPtr<TAstBaseNode> MyCondExpr;
	TSharedPtr<TAstBaseNode> MyUpdateExpr;
	TSharedPtr<TAstBaseNode> MyBlockExpr;
};

class TAstFunctionDefinitionNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstFunctionDefinitionNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(true);

CLASS_PRIVATE:
	Lime::TTokenIterator MyFunctionName;
	TSharedPtr<TTypeInfo> MyReturnType;

	TSharedPtr<TAstBaseNode> MyBlockExpr;
	Lime::TArray<Lime::TPair<TSharedPtr<TTypeInfo>, TSharedPtr<TVarInfo>>> MyArguments;
	Lime::TArray<TSharedPtr<TAstErrorNode>> MyErrors;
};

class TAstVariableDefinitionNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstVariableDefinitionNode);

	DEFINE_EVALUATE_TYPE(nullptr);

	DEFINE_STATIC_EVALUATABLE(true);

CLASS_PRIVATE:
	Lime::TTokenIterator MyName;
	TWeakPtr<TBlockEntry> MyBlock;
	TSharedPtr<TTypeInfo> MyType;
	bool MyIsArray { false };
	size_t MyArrayCount { 1 };

	TSharedPtr<TAstBaseNode> MyInitializeExpr;
};

class TAstInitializerListNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstInitializerListNode);

	DEFINE_EVALUATE_TYPE(MyType);

	IMPLEMENT_STATIC_EVALUATABLE()
	{
		bool Ret = true;
		for (TSharedPtr<TAstBaseNode> Node : MyLists)
		{
			Ret &= Node->IsStaticEvaluatable();
		}
		return Ret;
	}

CLASS_PRIVATE:
	TSharedPtr<TTypeInfo> MyType;
	Lime::TArray<TSharedPtr<TAstBaseNode>> MyLists;
	TSharedPtr<TAstErrorNode> MyError;
};

class TAstFunctionCallNode : public TAstBaseNode {
public:
	AST_BODY_CLASS(TAstFunctionCallNode);

	IMPLEMENT_EVALUATE_TYPE()
	{
		if (MyFunction->MyReturnType)
		{
			return TTypeTable::GetGlobalTable()->GetInfo(*MyFunction->MyReturnType);
		}
		return nullptr;
	}

	DEFINE_STATIC_EVALUATABLE(false);

CLASS_PRIVATE:
	Lime::TArray<Lime::TPair<TSharedPtr<TAstBaseNode> /* Node */, TSharedPtr<TAstBaseNode> /* Error or Warning */>> MyArguments;
	TSharedPtr<TTypeInfo> MyFunction;
	TSharedPtr<TAstErrorNode> MyError;
};