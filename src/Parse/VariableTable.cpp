#include "Parse/VariableTable.hpp"


TVarInfo::TVarInfo() :
	MyName(U""),
	MyType(),
	MyArrayCount(1)
{}

TVarInfo::TVarInfo(THashString InName, const TVarTypeInfo& InType, Lime::size_t InArrayCount, THashString InScopeName) :
	MyName(InName),
	MyType(InType),
	MyArrayCount(InArrayCount),
	MyScope(InScopeName)
{}

bool TVarInfo::operator==(const TVarInfo& InRhs) const noexcept
{
	return MyName == InRhs.MyName;
}
bool TVarInfo::operator!=(const TVarInfo& InRhs) const noexcept
{
	return MyName != InRhs.MyName;
}
bool TVarInfo::operator>(const TVarInfo& InRhs) const noexcept
{
	return MyName > InRhs.MyName;
}
bool TVarInfo::operator<(const TVarInfo& InRhs) const noexcept
{
	return MyName < InRhs.MyName;
}
bool TVarInfo::operator>=(const TVarInfo& InRhs) const noexcept
{
	return MyName >= InRhs.MyName;
}
bool TVarInfo::operator<=(const TVarInfo& InRhs) const noexcept
{
	return MyName <= InRhs.MyName;
}


TBlockEntry::TBlockEntry(TSharedPtr<TBlockEntry> InParent, THashString InStr) noexcept :
	MyBlockName(InStr),
	MyParent(InParent)
{}

TBlockEntry::~TBlockEntry() noexcept {}

bool TBlockEntry::IsDefined(THashString InVarName) const noexcept
{
	const auto Itr = MyVariableTable.find(InVarName);
	if (MyParent)
	{
		return MyParent->IsDefined(InVarName) && Itr != MyVariableTable.end();
	}
	return Itr != MyVariableTable.end();
}

void TBlockEntry::Define(THashString InVarName, const TVarTypeInfo& InInfo, Lime::size_t InArrayCount) noexcept
{
	MyVariableTable.insert({
		InVarName,
		TVarInfo(InVarName, InInfo, InArrayCount, MyBlockName)
	});
}
TOption<TVarInfo> TBlockEntry::GetInfo(THashString InVarName) const noexcept
{
	const auto Itr = MyVariableTable.find(InVarName);
	if (Itr != MyVariableTable.end())
	{
		return Itr->second;
	}
	if (MyParent)
	{
		return MyParent->GetInfo(InVarName);
	}
	return DefaultErrorType::Error;
}

THashString TBlockEntry::BlockName() const noexcept
{
	return MyBlockName;
}
TSharedPtr<TBlockEntry> TBlockEntry::Parent() const noexcept
{
	return MyParent;
}


TVariableTable::TVariableTable() noexcept :
	MyRootBlock(MakeShared<TBlockEntry>()),
	MyReferenceTable()
{}

TSharedPtr<TBlockEntry> TVariableTable::AddBlock(TSharedPtr<TBlockEntry> InParentBlock, THashString InAddBlockName) noexcept
{
	TSharedPtr<TBlockEntry> Ret = MakeShared<TBlockEntry>();
	Ret->MyBlockName = InAddBlockName;
	Ret->MyParent = InParentBlock;
	return Ret;
}

TSharedPtr<TBlockEntry> TVariableTable::GetBlock(THashString InBlockName) const noexcept
{
	const auto Itr = MyReferenceTable.find(InBlockName);

	return Itr != MyReferenceTable.end() ? Itr->second.Lock() : nullptr;
}

bool TVariableTable::IsContain(THashString InBlockName) const noexcept
{
	const auto Itr = MyReferenceTable.find(InBlockName);
	return Itr != MyReferenceTable.end();
}