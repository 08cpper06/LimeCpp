#pragma once


#include "Std.hpp"
#include "String/HashString.hpp"


class TDebugger {
public:
	static TDebugger* Get() noexcept;

	void MainLoop() noexcept;

private:
	static int GetChar() noexcept;
	static bool Kbhit() noexcept;

	static Lime::TArray<THashString> GetArguments() noexcept;

private:
	TDebugger() noexcept = default;
	~TDebugger() noexcept = default;
};