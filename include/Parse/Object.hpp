#pragma once

#include "TypeTable.hpp"
#include "../Core/Option.hpp"
#include "../Std.hpp"


class TObject {
public:
	TObject() noexcept;
	TObject(TSharedPtr<TTypeInfo> InType, TSharedPtr<TObject> InValue) noexcept;
	TObject(TSharedPtr<TTypeInfo> InType) noexcept;
	TObject(int InValue) noexcept;
	TObject(int64_t InValue) noexcept;
	TObject(double InValue) noexcept;
	TObject(bool InValue) noexcept;
	~TObject() noexcept = default;

	bool IsValid() const noexcept;
	bool IsNumeric() const noexcept;
	bool IsDouble() const noexcept;
	bool IsInteger() const noexcept;
	bool IsBoolean() const noexcept;

	bool GetBool() const noexcept;
	void SetBool(bool InFlag) noexcept;
	double* GetDouble() noexcept;
	const double* GetDouble() const noexcept;

	int64_t* GetInteger() noexcept;
	const int64_t* GetInteger() const noexcept;

	TOption<double> GetAsDouble() const noexcept;
	TOption<int64_t> GetAsInteger() const noexcept;

	bool Plus(const TObject& InRhs) noexcept;
	bool Minus(const TObject& InRhs) noexcept;
	bool Div(const TObject& InRhs) noexcept;
	bool Time(const TObject& InRhs) noexcept;

	bool operator==(const TObject& InRhs) const noexcept;
	bool operator<=(const TObject& InRhs) const noexcept;
	bool operator<(const TObject& InRhs) const noexcept;
	bool operator>=(const TObject& InRhs) const noexcept;
	bool operator>(const TObject& InRhs) const noexcept;

public:
	TSharedPtr<TTypeInfo> MyType;
	Lime::TVariant<int64_t, double, bool> MyValue;
};