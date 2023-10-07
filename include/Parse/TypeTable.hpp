#pragma once


#include "../Std.hpp"
#include "String/HashString.hpp"


enum class CastErrorCode {
	NotCastable,
	Castable,
	LossCast,
	ExplicitCastable,
};

class TTypeInfo {
public:
	TTypeInfo();
	TTypeInfo(THashString InName, Lime::size_t InAlignment, Lime::TMap<THashString, CastErrorCode> InCastArray); /* Variable Type */
	TTypeInfo(THashString InName, Lime::size_t InAlignment, const Lime::TArray<THashString>& InMembers); /* Struct Type */
	TTypeInfo(THashString InName, const Lime::TArray<THashString>& InArguments, THashString InReturnType); /* Function */
	~TTypeInfo() = default;

	bool IsFunction() const noexcept;

	CastErrorCode IsCastable(THashString InTo) const noexcept;
	TOption<THashString> IsEvaluatableExpr(const TTypeInfo& InRhs) const noexcept;

public:
	THashString MyName;
	Lime::size_t MyAlignemnt;
	TOption<THashString> MyReturnType;

	Lime::TArray<THashString> MyMemberVariable;
	/* need explicit cast if value invalid value */
	/* cannot castable if not exist key */
	Lime::TMap<THashString /* CastTarget */, CastErrorCode /* CastTo */> MyCastArray;
};

class TTypeTable {
public:
	bool IsDefined(THashString InName) const;

	TOption<TTypeInfo> GetInfo(THashString InName) const;
	
	void AddDefine(const TTypeInfo& Info);

private:
	std::map<Lime::size_t /* HashValue */, TTypeInfo> Table;

private:
public:
	TTypeTable();
	~TTypeTable() = default;
};
