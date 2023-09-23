#include "ComplexTestFramework.hpp"
#include "ComplexTestFileManager.hpp"
#include "Tokenize/Tokenizer.hpp"
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

	std::filesystem::path ResultRootPath = u8"../../complextest/test/tokenize";

}

IMPLEMENT_COMPLEX_TEST_CLASS(Tokenize)
void TokenizeTest::GetTests(std::vector<std::string>& OutParameters) const
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

bool TokenizeTest::RunTest(const std::string& InParameter) const
{
	TOption<TUtf32String> Source = ComplexTestFileManager::Get().GetSourceString(InParameter);
	AssertTrue(Source, "not found file");
	if (!Source)
	{
		return false;
	}

	TOption<TUtf32String> Result = ComplexTestFileManager::Get().GetResultString((ResultRootPath / (InParameter + ".tok")).c_str(), [&Source](std::filesystem::path InPath)
	{
		TSourceContext Context = *Source;
		Tokenizer::Analyze(Context);
		TUtf8String Log;
		for (const TToken& Token : Context.Tokens())
		{
			Log += Token.GetInfoString();
			Log += TChar(u8'\n');
		}

		std::cout << *Source << std::endl;
		std::cout << std::endl;
		std::cout << Log << std::endl;
		std::cout << std::endl;

		FileIO::WriteFile(InPath.string().c_str(), Log.Bytes());
		return *String::ConvertToUtf32(Log.Bytes());
	});

	std::vector<TUtf32String> ResultLines = GetLines(Result->Bytes());
	Lime::size_t NumLine = 0;


	TSourceContext Context = *Source;
	Tokenizer::Analyze(Context);
	TUtf32String Line;

	AssertEqual(Context.Tokens().size(), ResultLines.size(), "num of token is not matched");
	if (!IsSuccess)
	{
		return false;
	}

	for (const TToken& Token : Context.Tokens())
	{
		Line = *String::ConvertToUtf32(Token.GetInfoString().Bytes());
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