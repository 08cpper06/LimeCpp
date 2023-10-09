#pragma once


#include "../String/HashString.hpp"
#include "../Std.hpp"

#include "TypeTable.hpp"
#include "Object.hpp"


class TBlocKEntry;

struct TVarInfo {
public:
	TVarInfo();
	TVarInfo(THashString InName, TSharedPtr<TTypeInfo> InType, bool InIsArray, Lime::size_t InArrayCount, THashString InScopeName);
	~TVarInfo() = default;

	bool operator==(const TVarInfo& InRhs) const noexcept;
	bool operator!=(const TVarInfo& InRhs) const noexcept;
	bool operator>(const TVarInfo& InRhs) const noexcept;
	bool operator<(const TVarInfo& InRhs) const noexcept;
	bool operator>=(const TVarInfo& InRhs) const noexcept;
	bool operator<=(const TVarInfo& InRhs) const noexcept;

public:
	THashString MyName;
	TSharedPtr<TTypeInfo> MyType;
	THashString MyScope;
	bool MyIsArray;
	Lime::size_t MyArrayCount;
	Lime::TArray<TSharedPtr<TObject>> MyObject;
};


class TBlockEntry {
public:
	TBlockEntry() = default;
	TBlockEntry(TSharedPtr<TBlockEntry> InParent, THashString InStr) noexcept;
	~TBlockEntry() noexcept;
	
	bool IsDefined(THashString InVarName) const noexcept;
	void Define(THashString InVarName, TSharedPtr<TTypeInfo> InInfo, bool InIsArray, Lime::size_t InArrayCount = 1) noexcept;
	void UnDefine(THashString InVarName) noexcept;
	TSharedPtr<TVarInfo> GetInfo(THashString InVarName) const noexcept;

	THashString BlockName() const noexcept;
	TSharedPtr<TBlockEntry> Parent() const noexcept;

	auto begin() noexcept
	{
		return MyVariableTable.begin();
	}
	auto end() noexcept
	{
		return MyVariableTable.end();
	}
	auto begin() const noexcept
	{
		return MyVariableTable.begin();
	}
	auto end() const noexcept
	{
		return MyVariableTable.end();
	}
	auto cbegin() const noexcept
	{
		return MyVariableTable.cbegin();
	}
	auto cend() const noexcept
	{
		return MyVariableTable.cend();
	}

CLASS_PRIVATE:
	THashString MyBlockName;
	Lime::TMap<THashString, TSharedPtr<TVarInfo>> MyVariableTable;
	TWeakPtr<TBlockEntry> MyParent;

	friend class TVariableTable;
};

class TVariableTable {
public:
	TVariableTable() noexcept;
	~TVariableTable() noexcept = default;

	TSharedPtr<TBlockEntry> AddBlock(TSharedPtr<TBlockEntry> InParentBlock, THashString InAddBlockName) noexcept;
	TSharedPtr<TBlockEntry> GetBlock(THashString InBlockName) const noexcept;
	void RemoveBlock(THashString InBlockName) noexcept;
	bool IsContain(THashString InBlockName) const noexcept;

CLASS_PRIVATE:
	TSharedPtr<TBlockEntry> MyRootBlock;

	Lime::TMap<THashString, TSharedPtr<TBlockEntry>> MyReferenceTable;
};
