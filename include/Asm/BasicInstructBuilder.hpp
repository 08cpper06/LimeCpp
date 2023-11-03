#pragma once


#include "../Std.hpp"
#include "BasicInstruct.hpp"


class TAsmBasicBuilder {
public:
	template <class Type, class... TArgs>
	TSharedPtr<Type> CreateInstruct(TArgs&&... InArgs) noexcept
	{
		TSharedPtr<Type> Ptr = MakeShared<Type>(InArgs...);
		MyInstructs.push_back(Ptr);
		return Ptr;
	}

	TUtf32String MakeNewLabel() noexcept
	{
		return U'L' + ToUtf32String(++MyLabelNamePostfix);
	}

	TUtf32String GetLastLabelName() const noexcept
	{
		return U'L' + ToUtf32String(MyLabelNamePostfix);
	}

	void PushStack(TSharedPtr<TAsmBasicOperand> InElement) noexcept
	{
		int64_t Ret = MyStack.size();
		if (!InElement->MyName.MyHashValue)
		{
			InElement->MyName = ToUtf32String(Ret - MyStackOffset);
		}
		MyStack.push_back(InElement);
	}

	Lime::size_t GetStackPos() const noexcept
	{
		Lime::size_t Size = MyStack.size() - MyStackOffset;
		return Size ? Size - 1 : 0;
	}

	void SetStackOffset(Lime::size_t InOffset) noexcept
	{
		MyStackOffset = InOffset;
	}

	Lime::size_t GetStackOffset() const noexcept
	{
		return MyStackOffset;
	}

	TSharedPtr<TAsmBasicOperand> ReferStack(int64_t InIndex) noexcept
	{
		assert(InIndex >= 0);
		assert(MyStackOffset + InIndex < MyStack.size());

		Lime::size_t Address = MyStackOffset + InIndex;

		TSharedPtr<TObject> Value = MakeShared<TObject>(static_cast<int>(Address));
		TSharedPtr<TAsmBasicOperand> Operand = MakeShared<TAsmBasicOperand>(AsmBasicOperandType::Address, Value, MyStack[Address]->MyName);
		return Operand;
	}

	TSharedPtr<TAsmBasicOperand> PopStack() noexcept
	{
		TSharedPtr<TAsmBasicOperand> Operand = MyStack.back();
		RemoveStack();
		return Operand;
	}

	void RemoveStack() noexcept
	{
		MyStack.pop_back();
	}
	void ClearStack() noexcept
	{
		MyStack.clear();
	}

	TUtf32String GetInfoString() const noexcept
	{
		TUtf32String Str;

		/* print ir */
		for (TSharedPtr<TAsmBasicInstruct> Instruct : MyInstructs)
		{
			if (Instruct->StaticClass() == TAsmBasicLabelInstruct().StaticClass())
			{
				Str += Instruct->GetInfoString();
			}
			else
			{
				Str += Instruct->GetInfoString(U'\t');
			}
		}
		return Str;
	}

CLASS_PRIVATE:
	Lime::size_t MyLabelNamePostfix { 0 };
	Lime::TArray<TSharedPtr<TAsmBasicOperand>> MyStack;
	Lime::size_t MyStackOffset { 0 };
	Lime::TList<TSharedPtr<TAsmBasicInstruct>> MyInstructs;
};