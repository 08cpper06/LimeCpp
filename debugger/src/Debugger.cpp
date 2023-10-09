#include "Debugger.hpp"
#include "Tokenize/SyntaxFunction.hpp"
#include "CommandParser.hpp"
#include <iostream>
#include <string>


TDebugger* TDebugger::Get() noexcept
{
	static TDebugger StaticInstance;
	return &StaticInstance;
}

Lime::TArray<THashString> TDebugger::GetArguments() noexcept
{
	Lime::TArray<THashString> Out;
	std::string Str;
	if (std::getline(std::cin, Str))
	{
		TUtf32String TmpStr;
		for (char Char : Str)
		{
			if (Syntax::IsSpace(Char))
			{
				Out.push_back(TmpStr);
				TmpStr.Reset();
				continue;
			}
			TmpStr += Char;
		}
		if (!TmpStr.IsEmpty())
		{
			Out.push_back(TmpStr);
		}
	}

	return Out;

}

void TDebugger::MainLoop() noexcept
{
	Lime::TArray<THashString> Args;
	for (;;)
	{
		std::cout << ">>";
		Args = GetArguments();
		if (!Args.empty())
		{
			Lime::TArray<THashString> Candiate = TCommandParser::Get()->GetCandiateCommand(Args[0]);
			if (Candiate.size() != 1 || Candiate[0].GetString() != Args[0])
			{
				std::cout << "not found command (candiate command list :";
				bool IsFirst = true;
				for (THashString Item : Candiate)
				{
					if (!std::exchange(IsFirst, false))
					{
						std::cout << " ";
					}
					std::cout << "`" << Item.GetString() << "`";
				}
				std::cout << ")" << std::endl;
				continue;
			}
		}
		else
		{
			break;
		}

		std::cout << CONSOLE_COLOR(0, 88, 45);
		for (const THashString Str : Args)
		{
			std::cout << Str;
		}
		std::cout << DEFAULT_COLOR << std::endl;
		std::cout << CONSOLE_COLOR(200, 217, 33) << TCommandParser::Get()->Parse(Args) << DEFAULT_COLOR << std::endl;
	}
}