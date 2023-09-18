#include "ComplexTestFramework.hpp"


int main(int Argc, const char** Argv)
{
	if (Argc == 1)
	{
		for (const std::string& TestName : ComplexTestFramework::Get().GatherTest())
		{
			ComplexTestFramework::Get().RunTest(TestName.c_str());
		}
		return 0;
	}

	ComplexTestFramework::Get().RunTest(Argv[1]);
	return 0;
}