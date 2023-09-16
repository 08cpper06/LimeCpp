#include "TestFramework.hpp"


int main(int Argc, const char** Argv)
{
	if (Argc == 1)
	{
		for (const std::string& TestName : TestFramework::Get().GatherTest())
		{
			TestFramework::Get().RunTest(TestName.c_str());
		}
		return 0;
	}

	TestFramework::Get().RunTest(Argv[1]);
	return 0;
}