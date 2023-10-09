#include "CommandParser.hpp"
#include "SourceContext.hpp"
#include "Tokenize/Tokenizer.hpp"
#include "Parse/Parser.hpp"
#include "FileIO/FileIO.hpp"


static TSourceContext Context;

IMPLEMENT_COMMAND_FUNCTION(SourceLoader, "source")
TUtf32String SourceLoaderCommand::Execute(const Lime::TArray<THashString>& InArgs)
{
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
	return *Source;
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