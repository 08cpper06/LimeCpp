#pragma once


#include "../Std.hpp"

#include "../String/String.hpp"

#include <string>
#include <fstream>


enum class FileIOError {
	NoError,
	NoExist,
	EncodingError,
};


class FileIO {
public:
	static void WriteFile(const char* InPath, TUtf8StringView Buffer)
	{
		std::ofstream FileStream(InPath);
		if (!FileStream.fail())
		{
			FileStream << Buffer;
		}
	}

	static TOption<TUtf32String, FileIOError> ReadFile(const char* InPath)
	{
		TOption<std::string, FileIOError> Data = _ReadFile(InPath);
		if (!Data)
		{
			return Data.GetStatus();
		}
		TUtf8String Utf8Str = reinterpret_cast<const char8_t*>(Data->c_str());

		TOption<TUtf32String, ConvertEncodingError> Utf32Str = String::ConvertToUtf32(Utf8Str.Bytes());
		if (!Utf32Str)
		{
			return FileIOError::EncodingError;
		}
		return *Utf32Str;
	}

private:
	static TOption<std::string, FileIOError> _ReadFile(const char* InPath)
	{
		std::ifstream FileStream(InPath);
		if (FileStream.fail())
		{
			return FileIOError::NoExist;
		}
		std::string RawData(
			(std::istreambuf_iterator<char>(FileStream)),
			(std::istreambuf_iterator<char>())
		);
		return RawData;
	}

};