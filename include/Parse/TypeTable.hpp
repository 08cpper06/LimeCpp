#pragma once


#include "../Std.hpp"
#include "String/HashString.hpp"


class TTypeInfo {
public:
	TTypeInfo();
	TTypeInfo(THashString InName, Lime::size_t InAlignment); /* Variable Type */
	TTypeInfo(THashString InName, Lime::size_t InAlignment, const Lime::TArray<THashString>& InMembers); /* Struct Type */
	TTypeInfo(THashString InName, const Lime::TArray<THashString>& InArguments, THashString InReturnType); /* Function */
	~TTypeInfo() = default;

	bool IsFunction() const noexcept;
public:
	THashString MyName;
	Lime::size_t MyAlignemnt;
	TOption<THashString> MyReturnType;

	Lime::TArray<THashString> MyMemberVariable;
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
