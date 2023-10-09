#include "CommandParser.hpp"


TCommandParser* TCommandParser::Get()
{
	static TCommandParser StaticInstance;
	return &StaticInstance;
}

void TCommandParser::AddCommand(THashString InCommand, std::function<TUtf32String(Lime::TArray<THashString>)> InCallback) noexcept
{
	MyCallbackList.insert({ InCommand, InCallback });
}

TUtf32String TCommandParser::Parse(const Lime::TArray<THashString>& InArgs) const noexcept
{
	if (InArgs.size() < 2)
	{
		return U"argument too short";
	}
	auto Itr = MyCallbackList.find(InArgs[0]);
	if (Itr == MyCallbackList.end())
	{
		return U"not found command";
	}
	return Itr->second(InArgs);
}

