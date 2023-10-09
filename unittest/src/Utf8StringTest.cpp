#pragma once


#include "TestFramework.hpp"

#include "String/Utf8String.hpp"


IMPLEMENT_TEST_CLASS(Utf8String)
bool Utf8StringTest::RunTest() const
{
	TUtf8String Str = u8"HelloWorld";
	AssertEqual(Str.CharCount(), 10, "CharCount() is not matched");

	auto Itr = Str.begin();
	const char8_t* TrueStr = u8"HelloWorld";
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>((*Itr).MyData[0]), static_cast<char>(TrueStr[Index]), "Buffer is not match");
		++Itr;
	}

	AssertFalse(std::strcmp(reinterpret_cast<const char*>(Str.MyData.data()), reinterpret_cast<const char*>(TrueStr)), "Buffer is invalid state");

	AssertFalse(Str.IsEmpty(), "Expect IsEmpty() return false, but not");

	AssertTrue(TUtf8String().IsEmpty(), "Expect IsEmpty() return true, but not");

	AssertTrue(Str.StartWith(u8'H'), "StartWith(char8_t) return true, but not");
	AssertTrue(Str.StartWith(u8"Hello"), "StartWith(const char8_t*) return true, but not");

	Str += TUtf8StringView(u8"Hello");
	Itr = Str.begin();
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>((*Itr).MyData[0]), "HelloWorldHello"[Index], "Buffer is not match");
		++Itr;
	}

	TUtf8String Str2 = Str + TUtf8StringView(u8"World");
	Itr = Str2.begin();
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>((*Itr).MyData[0]), "HelloWorldHelloWorld"[Index], "Buffer is not match");
		++Itr;
	}

	Str.Reset();
	AssertTrue(Str.IsEmpty(), "Expect IsEmpty() return true, but not");
	return true;
}

IMPLEMENT_TEST_CLASS(Utf8StringView)
bool Utf8StringViewTest::RunTest() const
{
	TUtf8StringView Str = u8"HelloWorld";
	AssertEqual(Str.CharCount(), 10, "CharCount() is not matched");

	auto Itr = Str.begin();
	const char8_t* TrueStr = u8"HelloWorld";
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		AssertEqual(static_cast<char>((*Itr).MyData[0]), static_cast<char>(TrueStr[Index]), "Buffer is not match");
		++Itr;
	}

	AssertFalse(Str.IsEmpty(), "Expect IsEmpty() return false, but not");

	AssertTrue(TUtf8StringView().IsEmpty(), "Expect IsEmpty() return true, but not");

	AssertTrue(Str.StartWith(u8'H'), "StartWith(char8_t) return true, but not");
	AssertTrue(Str.StartWith(u8"Hello"), "StartWith(const char8_t*) return true, but not");

	Str.Reset();
	AssertTrue(Str.IsEmpty(), "Expect IsEmpty() return true, but not");
	return true;
}