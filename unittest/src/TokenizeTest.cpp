#pragma once


#include "Tokenize/Tokenizer.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(Tokenize)
bool TokenizeTest::RunTest() const
{
	TSourceContext Context = TUtf32String(U"int main() { return 0; }");
	Tokenizer::Analyze(Context);
	for (const TToken& Token : Context.Tokens())
	{
		std::printf(CONSOLE_COLOR(103, 190, 141));
		std::printf("%s\n", reinterpret_cast<const char*>(Token.GetInfoString().Bytes()));
		std::printf(DEFAULT_COLOR);
	}
	return true;
}