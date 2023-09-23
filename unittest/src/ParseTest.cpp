#pragma once


#include "Tokenize/Tokenizer.hpp"
#include "Parse/Parser.hpp"

#include "TestFramework.hpp"


IMPLEMENT_TEST_CLASS(Parse)
bool ParseTest::RunTest() const
{
	TSourceContext Context = TUtf32String(U"int main() { return 0; }");

	std::printf(CONSOLE_COLOR(0, 164,141));
	std::printf("\n%s\n\n", "int main() { return 0; }");
	std::printf(DEFAULT_COLOR);

	Tokenizer::Analyze(Context);
	Parser::Analyze(Context);
	if (TSharedPtr<TAstBaseNode> Root = Context.ASTRoot())
	{
		TUtf8String Log = *String::ConvertToUtf8(Root->GetInfoString(U"").Bytes());
		std::printf(CONSOLE_COLOR(200, 217, 33));
		std::printf("%s\n", reinterpret_cast<const char*>(Log.Bytes()));
		std::printf(DEFAULT_COLOR);
	}
	return true;
}