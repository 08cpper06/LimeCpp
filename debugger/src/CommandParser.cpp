#include "CommandParser.hpp"
#include "Tokenize/SyntaxFunction.hpp"


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
	if (InArgs.empty())
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

Lime::TArray<THashString> TCommandParser::GetCandiateCommand(THashString InCommand) const noexcept
{
	Lime::TArray<THashString> Out;
	for (const Lime::TPair<THashString, std::function<TUtf32String(Lime::TArray<THashString>)>> Item : MyCallbackList)
	{
		if (InCommand == Item.first.GetString())
		{
			return { InCommand };
		}
		if (Item.first.GetString().StartWith(InCommand.GetString()))
		{
			Out.push_back(Item.first);
		}
	}
	return Out;
}
