#ifndef ASSERT_H_
#define ASSERT_H_

#include "Includes.h"

class Assert
{
public:
	Assert(){}
	virtual ~Assert(){}

	static void assertTrue(bool condition, string warning)
	{
		if(!condition)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertFalse(bool condition, string warning)
	{
		if(condition)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(bool result, bool expected, string warning)
	{
		if(result != expected)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(uint result, uint expected, string warning)
	{
		if(result != expected)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(int result, int expected, string warning)
	{
		if(result != expected)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(uint64_t result, uint64_t expected, string warning)
	{
		if(result != expected)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(float result, float expected, string warning, double tol=1.0e-6)
	{
		if(std::abs(result - expected) > tol)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(double result, double expected, string warning, double tol=1.0e-6)
	{
		if(std::abs(result - expected) > tol)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(string result, string expected, string warning)
	{
		if(result != expected)
		{
			cout << warning << endl;
			exit(1);
		}
	}

	static void assertEquals(vector<int> result, vector<int> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << warning << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << warning << endl;
				exit(1);
			}			
		}
	}

	static void assertEquals(vector<uint> result, vector<uint> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << warning << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << warning << endl;
				exit(1);
			}			
		}
	}

	static void assertEquals(vector<uint64_t> result, vector<uint64_t> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << warning << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << warning << endl;
				exit(1);
			}			
		}
	}

	static void assertEquals(vector<float> result, vector<float> expected, string warning, double tol=1.0e-6)
	{
		if(result.size() != expected.size())
		{
			cout << warning << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(std::abs(result[idx] - expected[idx]) > tol)
			{
				cout << warning << endl;
				exit(1);
			}			
		}
	}

	static void assertEquals(vector<double> result, vector<double> expected, string warning, double tol=1.0e-6)
	{
		if(result.size() != expected.size())
		{
			cout << warning << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(std::abs(result[idx] - expected[idx]) > tol)
			{
				cout << warning << endl;
				exit(1);
			}			
		}
	}

	static void assertEquals(vector<string> result, vector<string> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << warning << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << warning << endl;
				exit(1);
			}			
		}
	}

};

#endif