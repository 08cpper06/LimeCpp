#include "Parse/VariableTable.hpp"


TVarInfo::TVarInfo() :
	MyName(U""),
	MyType(nullptr),
	MyArrayCount(1),
	MyIsArray(false),
	MyScope()
{}

TVarInfo::TVarInfo(THashString InName, TSharedPtr<TTypeInfo> InType, bool InIsArray, Lime::size_t InArrayCount, THashString InScopeName) :
	MyName(InName),
	MyType(InType),
	MyArrayCount(InArrayCount),
	MyIsArray(InIsArray),
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
	if (TSharedPtr<TBlockEntry> Parent = MyParent.Lock())
	{
		return Parent->IsDefined(InVarName) && Itr != MyVariableTable.end();
	}
	return Itr != MyVariableTable.end();
}

void TBlockEntry::Define(THashString InVarName, TSharedPtr<TTypeInfo> InInfo, bool InIsArray, Lime::size_t InArrayCount) noexcept
{
	MyVariableTable.insert({
		InVarName,
		MakeShared<TVarInfo>(InVarName, InInfo, InIsArray, InArrayCount, MyBlockName)
	});
}

void TBlockEntry::UnDefine(THashString InVarName) noexcept
{
	MyVariableTable.erase(InVarName);
}

TSharedPtr<TVarInfo> TBlockEntry::GetInfo(THashString InVarName) const noexcept
{
	const auto Itr = MyVariableTable.find(InVarName);
	if (Itr != MyVariableTable.end())
	{
		return Itr->second;
	}
	if (TSharedPtr<TBlockEntry> Parent = MyParent.Lock())
	{
		return Parent->GetInfo(InVarName);
	}
	return nullptr;
}

THashString TBlockEntry::BlockName() const noexcept
{
	return MyBlockName;
}
TSharedPtr<TBlockEntry> TBlockEntry::Parent() const noexcept
{
	return MyParent.Lock();
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
	MyReferenceTable.insert({ InAddBlockName, Ret });
	return Ret;
}

TSharedPtr<TBlockEntry> TVariableTable::GetBlock(THashString InBlockName) const noexcept
{
	const auto Itr = MyReferenceTable.find(InBlockName);

	return Itr != MyReferenceTable.end() ? Itr->second : nullptr;
}

void TVariableTable::RemoveBlock(THashString InBlockName) noexcept
{
	MyReferenceTable.erase(InBlockName);
}

bool TVariableTable::IsContain(THashString InBlockName) const noexcept
{
	const auto Itr = MyReferenceTable.find(InBlockName);
	return Itr != MyReferenceTable.end();
}