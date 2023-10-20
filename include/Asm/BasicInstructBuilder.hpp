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

	int64_t GetStackPos() noexcept
	{
		/* TODO : change stack offset [byte] */
		return MyStack.size();
	}

	void PushStack(TSharedPtr<TAsmBasicOperand> InElement) noexcept
	{
		int64_t Ret = MyStack.size();
		if (!InElement->MyName.MyHashValue)
		{
			InElement->MyName = ToUtf32String(Ret);
		}
		MyStack.push_back(InElement);
	}

	TSharedPtr<TAsmBasicOperand> ReferStack(int64_t InIndex) noexcept
	{
		if (InIndex >= static_cast<int64_t>(MyStack.size()))
		{
			return nullptr;
		}
		if (InIndex < 0)
		{
			if (MyStack.size() + InIndex < 0)
			{
				return nullptr;
			}
			return MyStack[MyStack.size() + InIndex];
		}
		return MyStack[InIndex];
	}

	TSharedPtr<TAsmBasicOperand> PopStack() noexcept
	{
		TSharedPtr<TAsmBasicOperand> Ptr = MyStack.back();
		MyStack.pop_back();
		if (Ptr->MyOperandType == AsmBasicOperandType::Address)
		{
			return ReferStack(*(Ptr->MyValue->GetInteger()));
		}
		return Ptr;
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
	Lime::TArray<TSharedPtr<TAsmBasicOperand>> MyStack;
	Lime::TList<TSharedPtr<TAsmBasicInstruct>> MyInstructs;
};