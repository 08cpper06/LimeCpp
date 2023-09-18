#pragma once


#include "String/String.hpp"
#include "FileIO/FileIO.hpp"
#include <filesystem>


class ComplexTestFileManager {
public:
	inline static std::filesystem::path SourcePath = u8"../../complextest/test";
public:
	static ComplexTestFileManager& Get() noexcept
	{
		return StaticInstance;
	}

	TOption<TUtf32String> GetSourceString(const std::filesystem::path& InPath) const
	{
		if (!std::filesystem::exists(SourcePath / InPath))
		{
			return DefaultErrorType::Error;
		}
		TOption<TUtf32String, FileIOError> Buffer = FileIO::ReadFile((SourcePath / InPath).string().c_str());
		if (Buffer)
		{
			return *Buffer;
		}
		return DefaultErrorType::Error;
	}

	template <class Func>
	TOption<TUtf32String> GetResultString(const std::filesystem::path& InPath, Func&& IfNotExistFunc) const
	{
		if (!std::filesystem::exists(InPath))
		{
			return IfNotExistFunc(InPath);
		}
		else
		{
			std::filesystem::path Path = InPath;
			TOption<TUtf32String, FileIOError> Buffer = FileIO::ReadFile(Path.string().c_str());
			if (Buffer)
			{
				return *Buffer;
			}
			return DefaultErrorType::Error;
		}
	}
private:
	static ComplexTestFileManager StaticInstance;
};