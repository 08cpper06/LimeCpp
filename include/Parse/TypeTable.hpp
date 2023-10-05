#pragma once


#include "../Std.hpp"
#include "String/HashString.hpp"


class TTypeInfo {
public:
	TTypeInfo();
	TTypeInfo(THashString InName, Lime::size_t InAlignment);
	~TTypeInfo() = default;
public:
	THashString MyName;
	Lime::size_t MyAlignemnt;

	Lime::TArray<THashString> MyMemberVariable;
};

class TTypeTable {
public:
	bool IsDefined(THashString InName) const;

	TOption<TTypeInfo> GetInfo(THashString InName) const;
	
	void AddDefine(const TTypeInfo& Info);

private:
	std::map<size_t /* HashValue */, TTypeInfo> Table;

private:
public:
	TTypeTable();
	~TTypeTable() = default;
};
