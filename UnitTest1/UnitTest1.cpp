#include "pch.h"
#include "CppUnitTest.h"
#include "../LAB_12.13/LAB_12.13.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestDeleteEmployee)
		{
			Trie phoneBook;
			Employee emp("Doe", "12", "101");
			phoneBook.addEmployee(emp);

			phoneBook.deleteEmployee("12");
			Employee* foundEmp = phoneBook.searchByPhoneNumber("12");
			Assert::IsNull(foundEmp);
		}
	};
}
