#pragma once


#include "../Std.hpp"
#include "BasicInstruct.hpp"


class TAsmBasicBuilder {
public:
	template <class Type>
	TSharedPtr<Type> CreateInstruct() noexcept
	{
		TSharedPtr<Type> Ptr = MakeShared<Type>();
		MyInstructs.push_back(Ptr);
		return Ptr;
	}

	void PushStack(TSharedPtr<TAsmBasicOperand> InElement) noexcept
	{
		InElement->MyName = ToUtf32String(MyStack.size());
		MyStack.push(InElement);
	}

	TSharedPtr<TAsmBasicOperand> PopStack() noexcept
	{
		TSharedPtr<TAsmBasicOperand> Ptr = MyStack.top();
		MyStack.pop();
		return Ptr;
	}

	TSharedPtr<TAsmBasicOperand> ReferStack() const noexcept
	{
		return MyStack.top();
	}

	TUtf32String GetInfoString() const noexcept
	{
		TUtf32String Str;
		for (TSharedPtr<TAsmBasicInstruct> Instruct : MyInstructs)
		{
			Str += Instruct->GetInfoString();
		}
		return Str;
	}

CLASS_PRIVATE:
	Lime::TStack<TSharedPtr<TAsmBasicOperand>> MyStack;
	Lime::TList<TSharedPtr<TAsmBasicInstruct>> MyInstructs;
};