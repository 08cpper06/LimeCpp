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
	return 0;
}