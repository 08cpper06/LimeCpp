#pragma once


#include "../Std.hpp"


struct TVarTypeInfo {
	THashString MyName { U"" };
	size_t MyAlignemnt { 1 };

	Lime::TArray<THashString> MyMemberVariable {};
};

class TVarTypeTable {
public:
	bool IsDefined(THashString InName) const
	{
		Lime::TMap<size_t, TVarTypeInfo>::const_iterator Itr = Table.find(InName.MyHashValue);
		return Itr != Table.end();
	}

	TOption<TVarTypeInfo> GetInfo(THashString InName) const
	{
		Lime::TMap<size_t, TVarTypeInfo>::const_iterator Itr = Table.find(InName.MyHashValue);
		if (Itr == Table.end())
		{
			return DefaultErrorType::Error;
		}
		return Itr->second;
	}

	void AddDefine(const TVarTypeInfo& Info)
	{
		if (!IsDefined(Info.MyName))
		{
			Table.insert(std::pair<size_t, TVarTypeInfo>(Info.MyName.MyHashValue, Info));
		}
	}

private:
	std::map<size_t /* HashValue */, TVarTypeInfo> Table;

private:
public:
	TVarTypeTable()
	{
		TVarTypeInfo BuiltInType[] = {
			{ THashString(U"char"), 1 },
			{ THashString(U"short"), 2 },
			{ THashString(U"int"), 4 },
			{ THashString(U"long"), 4 },
			{ THashString(U"unsigned char"), 1 },
			{ THashString(U"unsigned short"), 2 },
			{ THashString(U"unsigned int"), 4 },
			{ THashString(U"unsigned long"), 4 },
			{ THashString(U"float"), 4 },
			{ THashString(U"double"), 8 }
		};

		for (const TVarTypeInfo& Info : BuiltInType)
		{
			Table.insert(std::pair<size_t, TVarTypeInfo>(Info.MyName.MyHashValue, Info));
		}
	}
};
