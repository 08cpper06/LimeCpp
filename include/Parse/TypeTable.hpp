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
	TTypeInfo(THashString InName, Lime::size_t InAlignment, Lime::size_t InByteSize, Lime::TMap<THashString, CastErrorCode> InCastArray); /* Variable Type */
	TTypeInfo(THashString InName, Lime::size_t InAlignment, Lime::size_t InByteSize, const Lime::TArray<THashString>& InMembers); /* Struct Type */
	TTypeInfo(THashString InName, const Lime::TArray<THashString>& InArguments, THashString InReturnType); /* Function */
	~TTypeInfo() = default;

	bool IsFunction() const noexcept;

	CastErrorCode IsCastable(THashString InTo) const noexcept;
	TOption<THashString> IsEvaluatableExpr(TSharedPtr<TTypeInfo> InRhs) const noexcept;

public:
	THashString MyName;
	Lime::size_t MyAlignemnt;
	Lime::size_t MyByteSize;
	TOption<THashString> MyReturnType;

	Lime::TArray<THashString> MyMemberVariable;
	/* need explicit cast if value invalid value */
	/* cannot castable if not exist key */
	Lime::TMap<THashString /* CastTarget */, CastErrorCode /* CastTo */> MyCastArray;
};

class TTypeTable {
public:
	bool IsDefined(THashString InName) const;

	TSharedPtr<TTypeInfo> GetInfo(THashString InName) const;
	
	void Define(const TTypeInfo& Info);

	static TTypeTable* GetGlobalTable() noexcept;
private:
	Lime::TMap<Lime::size_t /* HashValue */, TSharedPtr<TTypeInfo>> MyTable;

public:
	TTypeTable();
	TTypeTable(nullptr_t); /* dummy constructor for GlobalTable */
	~TTypeTable() = default;
};
