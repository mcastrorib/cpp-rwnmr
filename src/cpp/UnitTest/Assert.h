#ifndef ASSERT_H_
#define ASSERT_H_

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

using namespace std;

class Assert
{
public:
	Assert(){}
	virtual ~Assert(){}

	static void check(bool result, bool expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected." << endl;
			exit(1);
		}
	}

	static void check(uint result, uint expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected." << endl;
			exit(1);
		}
	}

	static void check(int result, int expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected." << endl;
			exit(1);
		}
	}

	static void check(uint64_t result, uint64_t expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected." << endl;
			exit(1);
		}
	}

	static void check(float result, float expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected."<< endl;
			exit(1);
		}
	}

	static void check(double result, double expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected." << endl;
			exit(1);
		}
	}

	static void check(string result, string expected, string warning)
	{
		if(result != expected)
		{
			cout << "Error: " << warning <<  " was unexpected." << endl;
			exit(1);
		}
	}

	static void check(vector<int> result, vector<int> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << "Error: (size) " << warning <<  " was unexpected." << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << "Error: value(" << idx << ") " << warning <<  " was unexpected." << endl;
				exit(1);
			}			
		}
	}

	static void check(vector<uint> result, vector<uint> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << "Error: (size) " << warning <<  " was unexpected." << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << "Error: value(" << idx << ") " << warning <<  "was unexpected." << endl;
				exit(1);
			}			
		}
	}

	static void check(vector<uint64_t> result, vector<uint64_t> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << "Error: (size) " << warning <<  " was unexpected." << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << "Error: value(" << idx << ") " << warning <<  " was unexpected." << endl;
				exit(1);
			}			
		}
	}

	static void check(vector<float> result, vector<float> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << "Error: (size) " << warning <<  " was unexpected."<< endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << "Error: value(" << idx << ") " << warning <<  " was unexpected." << endl;
				exit(1);
			}			
		}
	}

	static void check(vector<double> result, vector<double> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << "Error: (size) " << warning <<  " was unexpected." << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << "Error: value(" << idx << ") " << warning <<  " was  unexpected." << endl;
				exit(1);
			}			
		}
	}

	static void check(vector<string> result, vector<string> expected, string warning)
	{
		if(result.size() != expected.size())
		{
			cout << "Error: (size) " << warning <<  " was unexpected." << endl;
			exit(1);
		}

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				cout << "Error: value(" << idx << ") " << warning <<  " was unexpected." << endl;
				exit(1);
			}			
		}
	}

};

#endif