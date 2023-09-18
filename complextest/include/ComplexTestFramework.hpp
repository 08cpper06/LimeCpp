#pragma once


#include <string>
#include <list>
#include <memory>
#include <map>
#include <iostream>
#include <vector>
#include <source_location>


#if !defined(_DEBUG)
#define _DEBUG
#endif

#define CONSOLE_COLOR(R, G, B) "\x1b[38;2;" #R ";" #G ";" #B "m"
#define DEFAULT_COLOR CONSOLE_COLOR(200, 200, 200)


class ComplexTestBaseClass {
public:
	ComplexTestBaseClass() : IsSuccess(true) {}
	virtual ~ComplexTestBaseClass() = default;

	virtual std::string GetTestName() const noexcept = 0;
	virtual void GetTests(std::vector<std::string>& OutParameters) const = 0;
	virtual bool RunTest(const std::string& InParameter) const = 0;

	void AssertTrue(bool InTrigger, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (!InTrigger)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< " [File : " << InCodeLocation.file_name() << "]"
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}
	void AssertFalse(bool InTrigger, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InTrigger)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< " [File : " << InCodeLocation.file_name() << "]"
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type>
	void AssertEqual(Type InLhs, Type InRhs, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InLhs != InRhs)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< "(Lhs : " << InLhs << ", Rhs : " << InRhs << ") "
				<< "[File : " << InCodeLocation.file_name() << "]"
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertEqual(Type InLhs, UType InRhs, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InLhs != InRhs)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< "(Lhs : " << InLhs << ", Rhs : " << InRhs << ") "
				<< "[File : " << InCodeLocation.file_name() << "]"
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertEqual(const Type* InLhs, const UType* InRhs, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InLhs != InRhs)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< " (Lhs : " << std::hex << InLhs << ", Rhs : " << InRhs << ") "
				<< "[File : " << InCodeLocation.file_name() << "]"
				<< std::dec << DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type>
	void AssertNotEqual(Type InLhs, Type InRhs, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InLhs == InRhs)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< "(Lhs : " << InLhs << ", Rhs : " << InRhs << ") "
				<< "[File : " << InCodeLocation.file_name() << "]"
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertNotEqual(Type InLhs, UType InRhs, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InLhs == InRhs)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< "(Lhs : " << InLhs << ", Rhs : " << InRhs << ") "
				<< "[File : " << InCodeLocation.file_name() << "]"
				<< DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertNotEqual(const Type* InLhs, const UType* InRhs, std::string_view InMessage, std::source_location InCodeLocation = std::source_location::current()) const
	{
		if (InLhs == InRhs)
		{
			std::cout << CONSOLE_COLOR(255, 0, 0)
				<< "[Line : " << InCodeLocation.line() << "] "
				<< InMessage
				<< " (Lhs : " << std::hex << InLhs << ", Rhs : " << InRhs << ") "
				<< "[File : " << InCodeLocation.file_name() << "]"
				<< std::dec << DEFAULT_COLOR << std::endl;
			IsSuccess = false;
		}
	}

	mutable bool IsSuccess;
};


class ComplexTestFramework {
public:
	static ComplexTestFramework& Get() noexcept
	{
		return StaticInstance;
	}

	void RunTest(const char* InTestName) const
	{
		auto Itr = TestList.find(InTestName);
		if (Itr == TestList.end())
		{
			std::cout << DEFAULT_COLOR << InTestName << " is not found" << std::endl;
			return;
		}
		std::vector<std::string> Parameters;
		Itr->second->GetTests(Parameters);
		if (!Parameters.empty())
		{
			std::cout << CONSOLE_COLOR(29, 80, 162)
				<< "=== " << Itr->second->GetTestName() << "(Count : " << Parameters.size() << ") === "
				<< DEFAULT_COLOR << std::endl;
			for (const std::string& Parameter : Parameters)
			{
				std::cout << CONSOLE_COLOR(49, 144, 112)
					<< "--- Parameter : \"" << Parameter.c_str() << ("\" ---")
					<< DEFAULT_COLOR << std::endl;
				if (!Itr->second->RunTest(Parameter) || !Itr->second->IsSuccess)
				{
					std::cout << CONSOLE_COLOR(204, 125, 177) << "Test is failed" << DEFAULT_COLOR << std::endl;
				}
				else
				{
					std::cout << CONSOLE_COLOR(160, 216, 239) << "Test is success" << DEFAULT_COLOR << std::endl;
				}
				Itr->second->IsSuccess = true;
			}
		}
		
	}

	std::list<std::string> GatherTest()
	{
		std::list<std::string> TestNameList;
		for (auto& Test : TestList)
		{
			TestNameList.push_back(Test.first);
		}
		return TestNameList;
	}

	friend class ComplexTestBaseClass;

	void AddTestClass(ComplexTestBaseClass* InClassPtr)
	{
		if (InClassPtr)
		{
			std::string TestName = InClassPtr->GetTestName();
			TestList.insert({TestName, InClassPtr});
		}
	}

private:
	static ComplexTestFramework StaticInstance;
	std::map<std::string, ComplexTestBaseClass*> TestList;
};


#define IMPLEMENT_COMPLEX_TEST_CLASS(TestClassName) class TestClassName##Test : public ComplexTestBaseClass { \
public: \
	~TestClassName##Test() = default; \
	TestClassName##Test(nullptr_t) { ComplexTestFramework::Get().AddTestClass(new TestClassName##Test()); } \
	TestClassName##Test() {} \
	std::string GetTestName() const noexcept override { return #TestClassName; } \
	void GetTests(std::vector<std::string>& OutParameters) const override; \
	bool RunTest(const std::string& InParameter) const override; \
}; TestClassName##Test TestClassName##Test##Instance(nullptr);