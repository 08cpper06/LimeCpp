#pragma once


#include "String/Utf32String.hpp"

#include "TestFramework.hpp"


auto ConvertChar = [](char32_t InChar) -> TChar {
	TChar Char;
	if (InChar <= 0x7F)
	{
		Char.MyData[0] = static_cast<char8_t>(InChar);
		return Char;
	}

	if (InChar <= 0x7FF)
	{
		Char.MyData[0] = 0b11000000 | (InChar >> 6);
		Char.MyData[1] = 0b10000000 | (InChar & 0b00111111);
		return Char;
	}

	if (InChar <= 0xFFFF) {
		Char.MyData[0] = 0b11100000 | (InChar >> 12);
		Char.MyData[1] = 0b10000000 | ((InChar >> 6) & 0b00111111);
		Char.MyData[2] = 0b10000000 | (InChar & 0b00111111);
		return Char;
	}

	Char.MyData[0] = 0b11110000 | (InChar >> 18);
	Char.MyData[1] = 0b10000000 | ((InChar >> 12) & 0b00111111);
	Char.MyData[2] = 0b10000000 | ((InChar >> 6) & 0b00111111);
	Char.MyData[3] = 0b10000000 | (InChar & 0b00111111);
	return Char;
};


IMPLEMENT_TEST_CLASS(Utf32String)
bool Utf32StringTest::RunTest() const
{
	TUtf32String Str = U"HelloWorld";
	AssertEqual(Str.CharCount(), 10, "CharCount() is not matched");

	auto Itr = Str.begin();
	const char32_t* TrueStr = U"HelloWorld";
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>(ConvertChar(*Itr++).MyData[0]), static_cast<char>(TrueStr[Index]), "Buffer is not match");
	}

	AssertFalse(std::strcmp(reinterpret_cast<const char*>(Str.MyData.data()), reinterpret_cast<const char*>(TrueStr)), "Buffer is invalid state");

	AssertFalse(Str.IsEmpty(), "Expect IsEmpty() return false, but not");

	AssertTrue(TUtf32String().IsEmpty(), "Expect IsEmpty() return true, but not");

	AssertTrue(Str.StartWith(U'H'), "StartWith(char32_t) return true, but not");
	AssertTrue(Str.StartWith(U"Hello"), "StartWith(const char32_t*) return true, but not");

	Str += U"Hello";
	Itr = Str.begin();
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>(ConvertChar(*Itr++).MyData[0]), "HelloWorldHello"[Index], "Buffer is not match");
	}

	TUtf32String Str2 = Str + TUtf32StringView(U"World");
	Itr = Str2.begin();
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>(ConvertChar(*Itr++).MyData[0]), "HelloWorldHelloWorld"[Index], "Buffer is not match");
	}

	Str.Reset();
	AssertTrue(Str.IsEmpty(), "Expect IsEmpty() return true, but not");
	return true;
}

IMPLEMENT_TEST_CLASS(Utf32StringView)
bool Utf32StringViewTest::RunTest() const
{
	TUtf32StringView Str = U"HelloWorld";
	AssertEqual(Str.CharCount(), 10, "CharCount() is not matched");

	auto Itr = Str.begin();
	const char32_t* TrueStr = U"HelloWorld";
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>(ConvertChar(*Itr++).MyData[0]), static_cast<char>(TrueStr[Index]), "Buffer is not match");
	}

	AssertFalse(Str.IsEmpty(), "Expect IsEmpty() return false, but not");

	AssertTrue(TUtf32StringView().IsEmpty(), "Expect IsEmpty() return true, but not");

	AssertTrue(Str.StartWith(U'H'), "StartWith(char32_t) return true, but not");
	AssertTrue(Str.StartWith(U"Hello"), "StartWith(const char32_t*) return true, but not");

	Str.Reset();
	AssertTrue(Str.IsEmpty(), "Expect IsEmpty() return true, but not");
	return true;
}