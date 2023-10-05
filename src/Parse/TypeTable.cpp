#include "Parse/TypeTable.hpp"


TTypeInfo::TTypeInfo() :
	MyName(U""),
	MyAlignemnt(1),
	MyMemberVariable()
{}
TTypeInfo::TTypeInfo(THashString InName, Lime::size_t InAlignment) :
	MyName(InName),
	MyAlignemnt(InAlignment),
	MyMemberVariable()
{}


bool TTypeTable::IsDefined(THashString InName) const
{
	Lime::TMap<size_t, TTypeInfo>::const_iterator Itr = Table.find(InName.MyHashValue);
	return Itr != Table.end();
}


TOption<TTypeInfo> TTypeTable::GetInfo(THashString InName) const
{
	Lime::TMap<size_t, TTypeInfo>::const_iterator Itr = Table.find(InName.MyHashValue);
	if (Itr == Table.end())
	{
		return DefaultErrorType::Error;
	}
	return Itr->second;
}

void TTypeTable::AddDefine(const TTypeInfo& Info)
{
	if (!IsDefined(Info.MyName))
	{
		Table.insert(std::pair<size_t, TTypeInfo>(Info.MyName.MyHashValue, Info));
	}
}

TTypeTable::TTypeTable()
{
	TTypeInfo BuiltInType[] = {
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

	for (const TTypeInfo& Info : BuiltInType)
	{
		Table.insert(std::pair<size_t, TTypeInfo>(Info.MyName.MyHashValue, Info));
	}
}

