#pragma once


#include "Std.hpp"
#include "String/Utf32String.hpp"
#include "String/HashString.hpp"


class TCommandParser {
public:
	static TCommandParser* Get();

	void AddCommand(THashString InCommand, std::function<TUtf32String(Lime::TArray<THashString>)> InCallback) noexcept;

	TUtf32String Parse(const Lime::TArray<THashString>& InArgs) const noexcept;

	/* perfect match if return array count is 1 */
	Lime::TArray<THashString> GetCandiateCommand(THashString InCommand) const noexcept;
private:
	TCommandParser() noexcept = default;
	~TCommandParser() noexcept = default;

	Lime::TMap<THashString, std::function<TUtf32String(Lime::TArray<THashString>)>> MyCallbackList;
};

#define IMPLEMENT_COMMAND_FUNCTION(CommandStr, CommandName) class CommandStr##Command { \
public: \
	~CommandStr##Command() noexcept = default; \
	CommandStr##Command(nullptr_t) noexcept { TCommandParser::Get()->AddCommand(*String::ConvertToUtf32(CommandName), CommandStr##Command::Execute); } \
	CommandStr##Command() {} \
	static TUtf32String Execute(const Lime::TArray<THashString>&); \
}; CommandStr##Command CommandStr##Command##Instance(nullptr);
