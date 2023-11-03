#include "CommandParser.hpp"
#include "SourceContext.hpp"
#include "Tokenize/Tokenizer.hpp"
#include "Parse/Parser.hpp"
#include "FileIO/FileIO.hpp"
#include "Asm/BasicAsmGenerator.hpp"


#define UCONSOLE_COLOR(R, G, B) U"\x1b[38;2;" #R U";" #G U";" #B U"m"
#define UDEFAULT_COLOR() UCONSOLE_COLOR(200, 200, 200)


static TSourceContext Context;

IMPLEMENT_COMMAND_FUNCTION(SourceLoader, "source")
TUtf32String SourceLoaderCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
	if (InArgs.size() < 2)
	{
		return U"source ...";
	}
	TUtf32String Output;
	TOption<TUtf8String, ConvertEncodingError> FilePath = String::ConvertToUtf8(InArgs[1].GetString());
	TOption<TUtf32String, FileIOError> Source = FileIO::ReadFile(reinterpret_cast<const char*>(FilePath->Bytes()));
	switch (Source.GetStatus()) {
	case FileIOError::NoExist:
		return U"not found file (" + InArgs[1] + U")\n";
	case FileIOError::EncodingError:
		return U"encoding error expect UTF-8";
	}
	Context = *Source;
	return *Source + U'\n';
}

IMPLEMENT_COMMAND_FUNCTION(Tokenize, "tokenize")
TUtf32String TokenizeCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
	if (Context.Source().IsEmpty())
	{
		return U"set source file (`source ...`)";
	}
	TUtf32String Output;
	Tokenizer::Analyze(Context);
	if (Context.Tokens().size() == 0 || Context.Tokens().back().MyLetter != U'\0')
	{
		return U"Tokenize is failed";
	}

	for (const TToken& Token : Context.Tokens())
	{
		Output += Token.GetInfoString() + U'\n';
	}
	return Output;
}

IMPLEMENT_COMMAND_FUNCTION(Parser, "parse")
TUtf32String ParserCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
	if (Context.Tokens().empty())
	{
		TokenizeCommand::Execute(Lime::TArray<THashString>());
	}
	Parser::Analyze(Context);
	TSharedPtr<TAstBaseNode> Root = Context.ASTRoot();
	if (!Root)
	{
		return U"fail to build AST";
	}

	return Root->GetInfoString(U"");
}

IMPLEMENT_COMMAND_FUNCTION(BuildIR, "ir")
TUtf32String BuildIRCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
	if (Context.Tokens().empty())
	{
		TokenizeCommand::Execute(Lime::TArray<THashString>());
	}

	if (!Context.ASTRoot())
	{
		ParserCommand::Execute(Lime::TArray<THashString>());
	}
	TSharedPtr<TAstBaseNode> Root = Context.ASTRoot();
	if (!Root)
	{
		return U"fail to build AST";
	}
	BasicAsmGenerator::Analyze(Context);
	return Context.AsmBuilder().GetInfoString();
}

IMPLEMENT_COMMAND_FUNCTION(PrintInfo, "print")
TUtf32String PrintInfoCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
	if (InArgs.size() == 1)
	{
		TUtf32String Str = UCONSOLE_COLOR(0, 164, 150) + TUtf32String(U"----- tokens -----\n");
		Str += UCONSOLE_COLOR(142, 197, 74) + TokenizeCommand::Execute(Lime::TArray<THashString>());
		Str += UCONSOLE_COLOR(0, 164, 150) + TUtf32String(U"----- ast -----\n");
		Str += UCONSOLE_COLOR(142, 197, 74) + ParserCommand::Execute(Lime::TArray<THashString>());
		Str += UCONSOLE_COLOR(0, 164, 150) + TUtf32String(U"----- ir -----\n");
		Str += UCONSOLE_COLOR(142, 197, 74) + BuildIRCommand::Execute(Lime::TArray<THashString>());
		return Str + UDEFAULT_COLOR();
	}
	else if (InArgs[1] == U"token")
	{
		return TokenizeCommand::Execute(Lime::TArray<THashString>());
	}
	else if (InArgs[1] == U"ast")
	{
		return ParserCommand::Execute(Lime::TArray<THashString>());
	}
	else if (InArgs[1] == U"ir")
	{
		return BuildIRCommand::Execute(Lime::TArray<THashString>());
	}
	return U"";
}

IMPLEMENT_COMMAND_FUNCTION(Clear, "clear")
TUtf32String ClearCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
	printf("\x1b[2J\x1b[0;0H");
	return U"";
}
