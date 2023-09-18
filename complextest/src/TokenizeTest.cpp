#include "ComplexTestFramework.hpp"
#include "ComplexTestFileManager.hpp"
#include "Tokenize/Tokenize.hpp"


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
	OutParameters.push_back("Test_01.c");
	OutParameters.push_back("Test_02.c");
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
		Lime::TList<TToken> List = Tokenize::Analyze(Source->Bytes());
		TUtf8String Log;
		for (const TToken& Token : List)
		{
			Log += Token.GetInfoString();
			Log += TChar(u8'\n');
		}
		std::cout << Log << std::endl;
		std::cout << std::endl;

		FileIO::WriteFile(InPath.string().c_str(), Log.Bytes());
		return *String::ConvertToUtf32(Log.Bytes());
	});

	std::vector<TUtf32String> ResultLines = GetLines(Result->Bytes());
	Lime::size_t NumLine = 0;

	Lime::TList<TToken> List = Tokenize::Analyze(Source->Bytes());
	TUtf32String Line;
	for (const TToken& Token : List)
	{
		Line = *String::ConvertToUtf32(Token.GetInfoString().Bytes());
		if (NumLine > ResultLines.size())
		{
			AssertTrue(false, "line is too short");
			break;
		}
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