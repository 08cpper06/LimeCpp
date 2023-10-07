#include "Parse/TypeTable.hpp"


TTypeInfo::TTypeInfo() :
	MyName(U""),
	MyAlignemnt(1),
	MyReturnType(DefaultErrorType::Error),
	MyMemberVariable()
{}
TTypeInfo::TTypeInfo(THashString InName, Lime::size_t InAlignment, Lime::TMap<THashString, CastErrorCode> InCastArray) :
	MyName(InName),
	MyAlignemnt(InAlignment),
	MyReturnType(DefaultErrorType::Error),
	MyMemberVariable(),
	MyCastArray(InCastArray)
{}
TTypeInfo::TTypeInfo(THashString InName, Lime::size_t InAlignment, const Lime::TArray<THashString>& InMembers) :
	MyName(InName),
	MyAlignemnt(InAlignment),
	MyReturnType(DefaultErrorType::Error),
	MyMemberVariable(InMembers)
{}
TTypeInfo::TTypeInfo(THashString InName, const Lime::TArray<THashString>& InArguments, THashString InReturnType) :
	MyName(InName),
	MyAlignemnt(1),
	MyReturnType(InReturnType),
	MyMemberVariable(InArguments)
{}
bool TTypeInfo::IsFunction() const noexcept
{
	return MyReturnType.GetStatus() != DefaultErrorType::Error;
}

CastErrorCode TTypeInfo::IsCastable(THashString InTo) const noexcept
{
	auto Itr = MyCastArray.find(InTo);
	if (Itr != MyCastArray.end())
	{
		return Itr->second;
	}
	return CastErrorCode::NotCastable;
}

TOption<THashString> TTypeInfo::IsEvaluatableExpr(const TTypeInfo& InRhs) const noexcept
{
	switch (IsCastable(InRhs.MyName)){
	case CastErrorCode::NotCastable:
		return DefaultErrorType::Error;
	case CastErrorCode::LossCast:
		return MyName;
	}
	return InRhs.MyName;
}


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
		{ THashString(U"void"),				0, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"char"),				1, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"short"),			2, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"int"),				4, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"long"),				4, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"unsigned char"),	1, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"unsigned short"),	2, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"unsigned int"),		4, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"unsigned long"),	4, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"float"),			4, Lime::TMap<THashString, CastErrorCode>() },
		{ THashString(U"double"),			8, Lime::TMap<THashString, CastErrorCode>() }
	};

	/* char */
	BuiltInType[1].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::Castable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::Castable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"unsigned short"), CastErrorCode::ExplicitCastable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::ExplicitCastable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::ExplicitCastable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[1].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* short */
	BuiltInType[2].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::ExplicitCastable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::Castable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::ExplicitCastable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::ExplicitCastable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::ExplicitCastable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[2].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* int */
	BuiltInType[3].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::ExplicitCastable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::ExplicitCastable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::ExplicitCastable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::ExplicitCastable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::ExplicitCastable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[3].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* long */
	BuiltInType[4].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::ExplicitCastable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::ExplicitCastable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::ExplicitCastable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::ExplicitCastable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::ExplicitCastable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[4].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* unsigned char */
	BuiltInType[5].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::ExplicitCastable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::ExplicitCastable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::ExplicitCastable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[5].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* unsigned short */
	BuiltInType[6].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::ExplicitCastable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::ExplicitCastable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::ExplicitCastable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::ExplicitCastable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::Castable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::Castable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::Castable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[6].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* unsigned int */
	BuiltInType[7].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::ExplicitCastable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::ExplicitCastable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::ExplicitCastable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::Castable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::Castable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[7].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* unsigned long */
	BuiltInType[8].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::ExplicitCastable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::ExplicitCastable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::Castable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::Castable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::ExplicitCastable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::ExplicitCastable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::Castable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::Castable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[8].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* float */
	BuiltInType[9].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[9].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"unsigned short"),	CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::LossCast });
	BuiltInType[9].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::Castable });
	BuiltInType[9].MyCastArray.insert({ THashString(U"double"),			CastErrorCode::Castable });
	/* double */
	BuiltInType[10].MyCastArray.insert({ THashString(U"void"),			CastErrorCode::Castable });
	BuiltInType[10].MyCastArray.insert({ THashString(U"char"),			CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"short"),			CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"int"),			CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"long"),			CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"unsigned char"),	CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"unsigned short"),CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"unsigned int"),	CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"unsigned long"),	CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"float"),			CastErrorCode::LossCast });
	BuiltInType[10].MyCastArray.insert({ THashString(U"double"),		CastErrorCode::Castable });

	for (const TTypeInfo& Info : BuiltInType)
	{
		Table.insert(std::pair<size_t, TTypeInfo>(Info.MyName.MyHashValue, Info));
	}
}

