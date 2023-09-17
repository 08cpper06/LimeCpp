#pragma once


#include "String/Utf8String.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(Utf8String)
bool Utf8StringTest::RunTest() const
{
	TUtf8String Str = u8"HelloWorld";
	ASSERT_EQUAL(Str.CharCount(), 10, "CharCount() is not matched");

	auto Itr = Str.begin();
	const char8_t* TrueStr = u8"HelloWorld";
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		ASSERT_EQUAL(static_cast<char>((*Itr).MyData[0]), static_cast<char>(TrueStr[Index]), "Buffer is not match");
		++Itr;
	}

	ASSERT_FALSE(std::strcmp(reinterpret_cast<const char*>(Str.MyData.data()), reinterpret_cast<const char*>(TrueStr)), "Buffer is invalid state");

	ASSERT_FALSE(Str.IsEmpty(), "Expect IsEmpty() return false, but not");

	ASSERT_TRUE(TUtf8String().IsEmpty(), "Expect IsEmpty() return true, but not");

	ASSERT_TRUE(Str.StartWith(u8'H'), "StartWith(char8_t) return true, but not");
	ASSERT_TRUE(Str.StartWith(u8"Hello"), "StartWith(const char8_t*) return true, but not");

	Str += TUtf8StringView(u8"Hello");
	Itr = Str.begin();
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		ASSERT_EQUAL(static_cast<char>((*Itr).MyData[0]), "HelloWorldHello"[Index], "Buffer is not match");
		++Itr;
	}

	TUtf8String Str2 = Str + TUtf8StringView(u8"World");
	Itr = Str2.begin();
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		ASSERT_EQUAL(static_cast<char>((*Itr).MyData[0]), "HelloWorldHelloWorld"[Index], "Buffer is not match");
		++Itr;
	}

	Str.Reset();
	ASSERT_TRUE(Str.IsEmpty(), "Expect IsEmpty() return true, but not");
	return true;
}

IMPLEMENT_TEST_CLASS(Utf8StringView)
bool Utf8StringViewTest::RunTest() const
{
	TUtf8StringView Str = u8"HelloWorld";
	ASSERT_EQUAL(Str.CharCount(), 10, "CharCount() is not matched");

	auto Itr = Str.begin();
	const char8_t* TrueStr = u8"HelloWorld";
	for (size_t Index = 0; Index < Str.CharCount(); ++Index)
	{
		ASSERT_EQUAL(static_cast<char>((*Itr).MyData[0]), static_cast<char>(TrueStr[Index]), "Buffer is not match");
		++Itr;
	}

	ASSERT_FALSE(Str.IsEmpty(), "Expect IsEmpty() return false, but not");

	ASSERT_TRUE(TUtf8StringView().IsEmpty(), "Expect IsEmpty() return true, but not");

	ASSERT_TRUE(Str.StartWith(u8'H'), "StartWith(char8_t) return true, but not");
	ASSERT_TRUE(Str.StartWith(u8"Hello"), "StartWith(const char8_t*) return true, but not");

	Str.Reset();
	ASSERT_TRUE(Str.IsEmpty(), "Expect IsEmpty() return true, but not");
	return true;
}