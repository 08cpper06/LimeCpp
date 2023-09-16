#pragma once


#include <string>
#include <list>
#include <memory>
#include <map>
#include <iostream>


#if !defined(_DEBUG)
#define _DEBUG
#endif

class TestBaseClass {
public:
	TestBaseClass() : IsSuccess(true) {}
	virtual ~TestBaseClass() = default;

	virtual std::string GetTestName() const noexcept = 0;
	virtual bool RunTest() const = 0;

	void AssertTrue(bool InTrigger, std::string_view InMessage) const
	{
		if (!InTrigger)
		{
			std::cout << "Fail(AssertTrue) : " << InMessage << std::endl;
			IsSuccess = false;
		}
	}
	void AssertFalse(bool InTrigger, std::string_view InMessage) const
	{
		if (InTrigger)
		{
			std::cout << "Fail(AssertFalse) : " << InMessage << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type>
	void AssertEqual(Type InLhs, Type InRhs, std::string_view InMessage) const
	{
		if (InLhs != InRhs)
		{
			std::cout << "Fail(AssertEqual) : " << InMessage << "(Lhs : " << InLhs << ", Rhs : " << InRhs << ")" << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertEqual(Type InLhs, UType InRhs, std::string_view InMessage) const
	{
		if (InLhs != InRhs)
		{
			std::cout << "Fail(AssertEqual) : " << InMessage << "(Lhs : " << InLhs << ", Rhs : " << InRhs << ")" << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertEqual(const Type* InLhs, const UType* InRhs, std::string_view InMessage) const
	{
		if (InLhs != InRhs)
		{
			std::cout << "Fail(AssertEqual) : " << InMessage << " (Lhs : " << std::hex << InLhs << ", Rhs : " << InRhs << ")" << std::dec << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type>
	void AssertNotEqual(Type InLhs, Type InRhs, std::string_view InMessage) const
	{
		if (InLhs == InRhs)
		{
			std::cout << "Fail(AssertNotEqual) : " << InMessage << "(Lhs : " << InLhs << ", Rhs : " << InRhs << ")" << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertNotEqual(Type InLhs, UType InRhs, std::string_view InMessage) const
	{
		if (InLhs == InRhs)
		{
			std::cout << "Fail(AssertNotEqual) : " << InMessage << "(Lhs : " << InLhs << ", Rhs : " << InRhs << ")" << std::endl;
			IsSuccess = false;
		}
	}

	template <class Type, class UType>
	void AssertNotEqual(const Type* InLhs, const UType* InRhs, std::string_view InMessage) const
	{
		if (InLhs == InRhs)
		{
			std::cout << "Fail(AssertNotEqual) : " << InMessage << " (Lhs : " << std::hex << InLhs << ", Rhs : " << InRhs << ")" << std::dec << std::endl;
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
			::printf("%s is not found\n", InTestName);
			return;
		}
		::printf("=== Test %s ===\n", InTestName);
		if (!Itr->second->RunTest())
		{
			::printf("Test %s is failed\n", InTestName);
		}
		::printf("=== Test %s End ===\n", InTestName);
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