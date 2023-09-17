#pragma once


#include "String/HashString.hpp"
#include "String/String.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(HashString)
bool HashStringTest::RunTest() const
{
	TUtf32String Str = U"HelloWorld";
	TUtf32StringView Str2(U"HelloWorld2", 10);
	TUtf8String Str3 = u8"Hello";
	TUtf8StringView Str4 = u8"Hello";

	ASSERT_EQUAL(THashString(Str), THashString(Str2), "InValid Hash Value");

	ASSERT_EQUAL(THashString(Str3), THashString(Str4), "InValid Hash Value");

	ASSERT_EQUAL(String::ConvertCharToUtf8(*THashString(U';').GetString().Bytes()), TChar(u8";"), "Invalid Hash Value");

	return true;
}