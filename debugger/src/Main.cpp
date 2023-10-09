#include "CommandParser.hpp"
#include "Std.hpp"
#include "Tokenize/Tokenizer.hpp"

#include <string>


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
		std::cout << DEFAULT_COLOR << std::endl;
		std::cout << CONSOLE_COLOR(200, 217, 33) << TCommandParser::Get()->Parse(Args) << DEFAULT_COLOR << std::endl;
	}

	std::string Str;
	for (;;)
	{
		std::cout << ">>";
		if (std::getline(std::cin, Str))
		{
			if (Str.empty())
			{
				break;
			}
			TOption<TUtf32String, ConvertEncodingError> ConvStr = String::ConvertToUtf32(reinterpret_cast<const char8_t*>(Str.c_str()));
			TSourceContext Context = *ConvStr;
			Tokenizer::Analyze(Context);
			Args.clear();
			for (const TToken& Element : Context.Tokens())
			{
				Args.push_back(Element.MyLetter);
			}
			std::cout << CONSOLE_COLOR(0, 88, 45) << Str << DEFAULT_COLOR << std::endl;
			std::cout << CONSOLE_COLOR(200, 217, 33) << TCommandParser::Get()->Parse(Args) << DEFAULT_COLOR << std::endl;
		}
	}
	return 0;
}

