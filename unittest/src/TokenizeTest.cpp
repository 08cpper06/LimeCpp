#pragma once


#include "Tokenize/Tokenize.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(Tokenize)
bool TokenizeTest::RunTest() const
{
	TUtf32StringView Source = U"int main() { return 0; }";
	Lime::TList<TToken> List = Tokenize::Analyze(Source);
	for (TToken& Token : List)
	{
		std::printf(CONSOLE_COLOR(103, 190, 141));
		std::printf("%s\n", reinterpret_cast<const char*>(Token.GetInfoString().Bytes()));
		std::printf(DEFAULT_COLOR);
	}
	return true;
}