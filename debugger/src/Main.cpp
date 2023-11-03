#include "CommandParser.hpp"
#include "Debugger.hpp"
#include "Std.hpp"
#include "Tokenize/Tokenizer.hpp"


int main(int Argc, const char** Argv)
{
	Lime::TArray<THashString> Args;
	if (Argc >= 2)
	{
		std::cout << CONSOLE_COLOR(0, 88, 45);
		for (int Index = 1; Index < Argc; ++Index)
		{
			Args.push_back(*String::ConvertToUtf32(Argv[Index]));
			std::cout << Argv[Index] << " ";
		}
		std::cout << std::endl;
		std::cout << CONSOLE_COLOR(142, 197, 74) << TCommandParser::Get()->Parse(Args) << DEFAULT_COLOR << std::endl;
	}

	TDebugger::Get()->MainLoop();
	return 0;
}

