#pragma once


#include "Tokenize/Tokenize.hpp"
#include "Parse/Parser.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(Parse)
bool ParseTest::RunTest() const
{
	TUtf32StringView Source = U"int main() { return 0; }";

	std::printf(CONSOLE_COLOR(0, 164,141));
	std::printf("\n%s\n\n", "int main() { return 0; }");
	std::printf(DEFAULT_COLOR);

	Lime::TList<TToken> List = Tokenize::Analyze(Source);
	TParseResult Result = Parser::Analyze(List);
	if (Result.MyASTRoot)
	{
		TUtf8String Log = *String::ConvertToUtf8(Result.MyASTRoot->GetInfoString(U"").Bytes());
		std::printf(CONSOLE_COLOR(200, 217, 33));
		std::printf("%s\n", reinterpret_cast<const char*>(Log.Bytes()));
		std::printf(DEFAULT_COLOR);
	}
	return true;
}