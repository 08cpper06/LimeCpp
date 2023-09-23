#include "FileIO/FileIO.hpp"
#include "Tokenize/Tokenizer.hpp"
#include "Parse/Parser.hpp"


#define CONSOLE_COLOR(R, G, B) "\x1b[38;2;" #R ";" #G ";" #B "m"
#define DEFAULT_COLOR CONSOLE_COLOR(200, 200, 200)

int main(int Argc, const char** Argv)
{
	if (Argc == 1)
	{
		std::cout << "not input file" << std::endl;
		return 1;
	}

	TOption<TUtf32String, FileIOError> Source = FileIO::ReadFile(Argv[1]);
	switch (Source.GetStatus()) {
	case FileIOError::NoExist:
		std::cout << "not found file (" << Argv[1] << ")" << std::endl;
		return int(Source.GetStatus()) + 1;
	case FileIOError::EncodingError:
		std::cout << "encoding error expect UTF-8" << std::endl;
		return int(Source.GetStatus()) + 1;
	}

	TSourceContext Context = *Source;
	
	Tokenizer::Analyze(Context);
	Parser::Analyze(Context);

	std::cout << CONSOLE_COLOR(0, 164, 141)
		<< *Source
		<< (DEFAULT_COLOR) << std::endl;

	if (Context.Tokens().size() == 0 || Context.Tokens().back().MyLetter != U'\0')
	{
		std::cout << "Tokenize is failed" << std::endl;
		return 3;
	}

	std::cout << "\n" << CONSOLE_COLOR(103, 190, 141);
	for (const TToken& Token : Context.Tokens())
	{
		std::cout << Token.GetInfoString() << std::endl;
	}
	std::cout << DEFAULT_COLOR << std::endl;

	TSharedPtr<TAstBaseNode> Root = Context.ASTRoot();
	if (!Root)
	{
		std::cout << "fail to build AST" << std::endl;
		return 4;
	}

	TUtf8String Log = *String::ConvertToUtf8(Root->GetInfoString(U"").Bytes());
	std::cout << CONSOLE_COLOR(200, 217, 33)
		<< Log
		<< DEFAULT_COLOR << std::endl;

	return 0;
}
