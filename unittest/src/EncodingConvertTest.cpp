#pragma once


#include "String/EncodingConverter.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(EncodingConvert)
bool EncodingConvertTest::RunTest() const
{
	TUtf8String SrcStr = u8"HelloWorld";
	TOption<TUtf32String, ConvertEncodingError> Utf32Str = String::ConvertToUtf32(SrcStr.Bytes());
	ASSERT_TRUE(Utf32Str, "Fail to convert to utf32 from utf8");

	TUtf32StringView Utf32StrView = Utf32Str->Bytes();
	ASSERT_EQUAL(*Utf32Str, TUtf32StringView(U"HelloWorld"), "buffer is not matched");

	ASSERT_EQUAL(Utf32Str->CharCount(), TUtf32StringView(U"HelloWorld").CharCount(), "CharCount is not matched");

	TOption<TUtf8String, ConvertEncodingError> Utf8Str = String::ConvertToUtf8(Utf32Str->Bytes());

	ASSERT_TRUE(Utf8Str, "Fail to convert to utf8 from utf32");

	ASSERT_EQUAL(*Utf8Str, TUtf8StringView(u8"HelloWorld"), "buffer is not matched");
	ASSERT_EQUAL(Utf8Str->CharCount(), TUtf8StringView(u8"HelloWorld").CharCount(), "CharCount is not matched");

	return true;
}