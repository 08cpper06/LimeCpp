#include "Parse/VariableTypeTable.hpp"


TVarTypeInfo::TVarTypeInfo() :
	MyName(U""),
	MyAlignemnt(1),
	MyMemberVariable()
{}
TVarTypeInfo::TVarTypeInfo(THashString InName, Lime::size_t InAlignment) :
	MyName(InName),
	MyAlignemnt(InAlignment),
	MyMemberVariable()
{}


bool TVarTypeTable::IsDefined(THashString InName) const
{
	Lime::TMap<size_t, TVarTypeInfo>::const_iterator Itr = Table.find(InName.MyHashValue);
	return Itr != Table.end();
}


TOption<TVarTypeInfo> TVarTypeTable::GetInfo(THashString InName) const
{
	Lime::TMap<size_t, TVarTypeInfo>::const_iterator Itr = Table.find(InName.MyHashValue);
	if (Itr == Table.end())
	{
		return DefaultErrorType::Error;
	}
	return Itr->second;
}

void TVarTypeTable::AddDefine(const TVarTypeInfo& Info)
{
	if (!IsDefined(Info.MyName))
	{
		Table.insert(std::pair<size_t, TVarTypeInfo>(Info.MyName.MyHashValue, Info));
	}
}

TVarTypeTable::TVarTypeTable()
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

