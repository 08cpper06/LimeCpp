#include "Parse/Object.hpp"


TObject::TObject() noexcept :
	MyType(nullptr),
	MyValue(0)
{}

TObject::TObject(TSharedPtr<TTypeInfo> InType, TSharedPtr<TObject> InValue) noexcept :
	MyType(InType)
{
	if (InValue)
	{
		MyValue = InValue->MyValue;
	}
	else if (IsInteger())
	{
		MyValue = 0;
	}
	else
	{
		MyValue = 0.0;
	}
}
TObject::TObject(TSharedPtr<TTypeInfo> InType) noexcept :
	MyType(InType),
	MyValue()
{}

TObject::TObject(int InValue) noexcept :
	MyValue(InValue)
{
	MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
}
TObject::TObject(int64_t InValue) noexcept :
	MyValue(InValue)
{
	MyType = TTypeTable::GetGlobalTable()->GetInfo(U"int");
}
TObject::TObject(double InValue) noexcept :
	MyValue(InValue)
{
	MyType = TTypeTable::GetGlobalTable()->GetInfo(U"double");
}
TObject::TObject(bool InValue) noexcept :
	MyValue(InValue)
{
	MyType =TTypeTable::GetGlobalTable()->GetInfo(U"bool");
}

bool TObject::IsValid() const noexcept
{
	return MyType;
}

bool TObject::IsNumeric() const noexcept
{
	if (!MyType) /* Check whether is valid */
	{
		return false;
	}
	if (IsBoolean()) /* Check whether bool */
	{
		return false;
	}
	if (MyType->IsFunction()) /* Check whether function */
	{
		return false;
	}
	if (!MyType->MyMemberVariable.empty()) /* Check whether struct */
	{
		return false;
	}
	return true;
}
bool TObject::IsDouble() const noexcept
{
	if (!MyType && !MyType->IsFunction())
	{
		return false;
	}
	return !IsInteger() && MyType->IsCastable(U"double") != CastErrorCode::NotCastable;
}
bool TObject::IsInteger() const noexcept
{
	if (!MyType && !MyType->IsFunction())
	{
		return false;
	}
	return MyType->IsCastable(U"int") != CastErrorCode::NotCastable;
}
bool TObject::IsBoolean() const noexcept
{
	return MyType->MyName == U"bool";
}

bool TObject::GetBool() const noexcept
{
	return std::get<int64_t>(MyValue) == 1;
}
void TObject::SetBool(bool InFlag) noexcept
{
	std::get<int64_t>(MyValue) = InFlag ? 1 : 0;
}

double* TObject::GetDouble() noexcept
{
	if (!IsDouble())
	{
		return nullptr;
	}
	return std::get_if<double>(&MyValue);
}

const double* TObject::GetDouble() const noexcept
{
	if (!IsDouble())
	{
		return nullptr;
	}
	return std::get_if<double>(&MyValue);
}

int64_t* TObject::GetInteger() noexcept
{
	if (!IsInteger())
	{
		return nullptr;
	}
	return std::get_if<int64_t>(&MyValue);
}
const int64_t* TObject::GetInteger() const noexcept
{
	if (!IsInteger())
	{
		return nullptr;
	}
	return std::get_if<int64_t>(&MyValue);
}

TOption<double> TObject::GetAsDouble() const noexcept
{
	if (!IsNumeric())
	{
		return DefaultErrorType::Error;
	}
	if (IsDouble())
	{
		return *GetDouble();
	}
	return static_cast<double>(*GetInteger());
}
TOption<int64_t> TObject::GetAsInteger() const noexcept
{
	if (!IsNumeric())
	{
		return DefaultErrorType::Error;
	}
	if (IsInteger())
	{
		return *GetInteger();
	}
	return static_cast<int64_t>(*GetDouble());
}
bool TObject::Plus(const TObject& InRhs) noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	if (IsDouble() && InRhs.IsDouble())
	{
		*GetDouble() += *InRhs.GetDouble();
	}
	else if (IsInteger() && InRhs.IsInteger())
	{
		*GetInteger() += *InRhs.GetInteger();
	}
	else if (IsInteger())
	{
		*GetInteger() += InRhs.GetAsInteger();
	}
	else
	{
		*GetDouble() += InRhs.GetAsDouble();
	}
	return true;
}
bool TObject::Minus(const TObject& InRhs) noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	if (IsDouble() && InRhs.IsDouble())
	{
		*GetDouble() -= *InRhs.GetDouble();
	}
	else if (IsInteger() && InRhs.IsInteger())
	{
		*GetInteger() -= *InRhs.GetInteger();
	}
	else if (IsInteger())
	{
		*GetInteger() -= InRhs.GetAsInteger();
	}
	else
	{
		*GetDouble() -= InRhs.GetAsDouble();
	}
	return true;
}
bool TObject::Div(const TObject& InRhs) noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	if (IsDouble() && InRhs.IsDouble())
	{
		*GetDouble() /= *InRhs.GetDouble();
	}
	else if (IsInteger() && InRhs.IsInteger())
	{
		*GetInteger() /= *InRhs.GetInteger();
	}
	else if (IsInteger())
	{
		*GetInteger() /= InRhs.GetAsInteger();
	}
	else
	{
		*GetDouble() /= InRhs.GetAsDouble();
	}
	return true;
}
bool TObject::Time(const TObject& InRhs) noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	if (IsDouble() && InRhs.IsDouble())
	{
		*GetDouble() *= *InRhs.GetDouble();
	}
	else if (IsInteger() && InRhs.IsInteger())
	{
		*GetInteger() *= *InRhs.GetInteger();
	}
	else if (IsInteger())
	{
		*GetInteger() *= InRhs.GetAsInteger();
	}
	else
	{
		*GetDouble() *= InRhs.GetAsDouble();
	}
	return true;
}

void TObject::Set(const Lime::TVariant<int64_t, double>& InValue)
{
	if (MyValue.index() == InValue.index())
	{
		MyValue = InValue;
	}
}

bool TObject::operator==(const TObject& InRhs) const noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	if (IsBoolean() && InRhs.IsBoolean())
	{
		return GetBool() == InRhs.GetBool();
	}
	else if (IsDouble() && InRhs.IsDouble())
	{
		return *GetDouble() == *InRhs.GetDouble();
	}
	else if (IsInteger() && InRhs.IsInteger())
	{
		return *GetInteger() == *InRhs.GetInteger();
	}
	else if (IsInteger())
	{
		return *GetInteger() == InRhs.GetAsInteger();
	}
	else
	{
		return *GetDouble() == InRhs.GetAsDouble();
	}
	return false;
}

bool TObject::operator<=(const TObject& InRhs) const noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	TObject Obj = *this;
	if (!Obj.Minus(InRhs))
	{
		return false;
	}
	return GetAsDouble() <= 0.;
}
bool TObject::operator<(const TObject& InRhs) const noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	TObject Obj = *this;
	if (!Obj.Minus(InRhs))
	{
		return false;
	}
	return GetAsDouble() < 0.;
}
bool TObject::operator>=(const TObject& InRhs) const noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	TObject Obj = *this;
	if (!Obj.Minus(InRhs))
	{
		return false;
	}
	return GetAsDouble() >= 0.;
}
bool TObject::operator>(const TObject& InRhs) const noexcept
{
	if (!IsNumeric() || !InRhs.IsNumeric())
	{
		return false;
	}
	TObject Obj = *this;
	if (!Obj.Minus(InRhs))
	{
		return false;
	}
	return GetAsDouble() > 0.;
}
