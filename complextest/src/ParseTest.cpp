#include "ComplexTestFramework.hpp"
#include "ComplexTestFileManager.hpp"
#include "Tokenize/Tokenizer.hpp"
#include "Parse/Parser.hpp"
#include "SourceContext.hpp"


namespace {

	std::vector<TUtf32String> GetLines(TUtf32StringView InSource)
	{
		TUtf32StringView::TConstIterator LineStartItr = InSource.cbegin();
		TUtf32StringView::TConstIterator Itr = InSource.cbegin();
		std::vector<TUtf32String> Lines;
		while (Itr)
		{
			if (*Itr == U'\n')
			{
				Lines.emplace_back(LineStartItr, Itr);
				LineStartItr = ++Itr;
				continue;
			}
			++Itr;
		}
		return Lines;
	}

	std::filesystem::path ResultRootPath = u8"../../complextest/test/parse";

}

IMPLEMENT_COMPLEX_TEST_CLASS(Parser)
void ParserTest::GetTests(std::vector<std::string>& OutParameters) const
{
	TUtf8String Path = u8"../../complextest/test";
	for (const std::filesystem::directory_entry& Entry : std::filesystem::directory_iterator(Path.Bytes()))
	{
		if (Entry.is_regular_file())
		{
			OutParameters.push_back(Entry.path().filename().string());
		}
	}
}

bool ParserTest::RunTest(const std::string& InParameter) const
{
	TOption<TUtf32String> Source = ComplexTestFileManager::Get().GetSourceString(InParameter);
	AssertTrue(Source, "not found file");
	if (!Source)
	{
		return false;
	}

	TOption<TUtf32String> Result = ComplexTestFileManager::Get().GetResultString((ResultRootPath / (InParameter + ".par")).c_str(), [&Source](std::filesystem::path InPath)
		-> TOption<TUtf32String>
	{
		TSourceContext Context = *Source;
		Tokenizer::Analyze(Context);
		Parser::Analyze(Context);
		if (!Context.ASTRoot())
		{
			return DefaultErrorType::Error;
		}
		TUtf32String ResultLog = Context.ASTRoot()->GetInfoString(U"").Bytes();
		TOption<TUtf8String, ConvertEncodingError> Log = String::ConvertToUtf8(TUtf32StringView(ResultLog.Bytes(), ResultLog.CharCount()));

		std::cout << *Source << std::endl;
		std::cout << std::endl;
		std::cout << *Log << std::endl;
		std::cout << std::endl;

		FileIO::WriteFile(InPath.string().c_str(), Log->Bytes());
		return ResultLog;
	});

	std::vector<TUtf32String> ResultLines = GetLines(Result->Bytes());
	Lime::size_t NumLine = 0;


	TSourceContext Context = *Source;
	Tokenizer::Analyze(Context);
	Parser::Analyze(Context);
	TUtf32String Line;

	if (!Context.ASTRoot())
	{
		std::cout << CONSOLE_COLOR(255, 0, 0)
			<< "AST build is failed"
			<< DEFAULT_COLOR << std::endl;
		return false;
	}

	std::vector<TUtf32String> ActualLines = GetLines(Context.ASTRoot()->GetInfoString(U"").Bytes());

	AssertEqual(ActualLines.size(), ResultLines.size(), "AST Log is not same num of lines");
	if (!IsSuccess)
	{
		return false;
	}

	for (const TUtf32String& Line : ActualLines)
	{
		if (Line != ResultLines[NumLine])
		{
			std::cout << CONSOLE_COLOR(234, 96, 158)
				<< "- [Line : " << NumLine << "] "
				<< Line
				<< DEFAULT_COLOR << std::endl;
			std::cout << CONSOLE_COLOR(77, 67, 152)
				<< "+ [Line : " << NumLine << "] "
				<< ResultLines[NumLine]
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
		++NumLine;
	}
	return IsSuccess;
}