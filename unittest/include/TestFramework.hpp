#pragma once


#include <string>
#include <list>
#include <memory>
#include <map>
#include <iostream>
#include <source_location>


#if !defined(_DEBUG)
#define _DEBUG
#endif

#define CONSOLE_COLOR(R, G, B) "\x1b[38;2;" #R ";" #G ";" #B "m"
#define DEFAULT_COLOR CONSOLE_COLOR(200, 200, 200)


class TestBaseClass {
public:
	TestBaseClass() : IsSuccess(true) {}
	virtual ~TestBaseClass() = default;

	virtual std::string GetTestName() const noexcept = 0;
	virtual bool RunTest() const = 0;

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


class TestFramework {
public:
	static TestFramework& Get() noexcept
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
		if (!Itr->second->RunTest() || !Itr->second->IsSuccess)
		{
			std::cout << CONSOLE_COLOR(255, 128, 255) << "Test " << InTestName << " is failed" << DEFAULT_COLOR << std::endl;
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

	friend class TestBaseClass;

	void AddTestClass(TestBaseClass* InClassPtr)
	{
		if (InClassPtr)
		{
			std::string TestName = InClassPtr->GetTestName();
			TestList.insert({TestName, InClassPtr});
		}
	}

private:
	static TestFramework StaticInstance;
	std::map<std::string, TestBaseClass*> TestList;
};


#define IMPLEMENT_TEST_CLASS(TestClassName) class TestClassName##Test : public TestBaseClass { \
public: \
	~TestClassName##Test() = default; \
	TestClassName##Test(nullptr_t) { TestFramework::Get().AddTestClass(new TestClassName##Test()); } \
	TestClassName##Test() {} \
	std::string GetTestName() const noexcept override { return #TestClassName; } \
	bool RunTest() const override; \
}; TestClassName##Test TestClassName##Test##Instance(nullptr);