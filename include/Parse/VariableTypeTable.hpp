#pragma once


#include "../Std.hpp"
#include "String/HashString.hpp"


class TVarTypeInfo {
public:
	TVarTypeInfo();
	TVarTypeInfo(THashString InName, Lime::size_t InAlignment);
	~TVarTypeInfo() = default;
public:
	THashString MyName;
	Lime::size_t MyAlignemnt;

	Lime::TArray<THashString> MyMemberVariable;
};

class TVarTypeTable {
public:
	bool IsDefined(THashString InName) const;

	TOption<TVarTypeInfo> GetInfo(THashString InName) const;
	
	void AddDefine(const TVarTypeInfo& Info);

private:
	std::map<size_t /* HashValue */, TVarTypeInfo> Table;

private:
public:
	TVarTypeTable();
	~TVarTypeTable() = default;
};
