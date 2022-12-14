#ifndef ASSERT_H_
#define ASSERT_H_

#include "Includes.h"
#define TOL 1.0e-6

class Assert
{
public:
	Assert(){}
	virtual ~Assert(){}

	static bool assertTrue(bool condition)
	{
		if(!condition) return false;			
		else return true;
	}

	static bool assertFalse(bool condition)
	{
		if(condition) return false;			
		else return true;	
	}

	template<typename T>
	static bool assertEquals(T result, T expected)
	{
		if(result != expected) return false;			
		else return true;
	}

	static bool assertEquals(float result, float expected, double tol=TOL)
	{
		if(std::abs(result - expected) > tol) return false;			
		else return true;
	}

	static bool assertEquals(double result, double expected, double tol=TOL)
	{
		if(std::abs(result - expected) > tol) return false;			
		else return true;
	}

	template<typename T>
	static bool assertVectorEquals(vector<T> result, vector<T> expected)
	{
		if(result.size() != expected.size()) return false;			

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(result[idx] != expected[idx])
			{
				return false;				
			}			
		}

		return true;
	}

	template<typename T>
	static bool assertVectorEquals(T *result, T *expected, int size)
	{
		for(int idx = 0; idx < size; idx++)
		{
			if(result[idx] != expected[idx])
			{
				return false;				
			}			
		}	

		return true;
	}

	static bool assertVectorEquals(float *result, float *expected, int size, double tol=TOL)
	{
		for(int idx = 0; idx < size; idx++)
		{
			if(std::abs(result[idx] - expected[idx]) > tol)
			{
				return false;				
			}			
		}

		return true;
	}

	static bool assertVectorEquals(double *result, double *expected, int size, double tol=TOL)
	{
		for(int idx = 0; idx < size; idx++)
		{
			if(std::abs(result[idx] - expected[idx]) > tol)
			{
				return false;				
			}			
		}

		return true;
	}

	static bool assertVectorEquals(vector<float> result, vector<float> expected, double tol=TOL)
	{
		if(result.size() != expected.size()) return false;			

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(std::abs(result[idx] - expected[idx]) > tol)
			{
				return false;				
			}			
		}

		return true;
	}

	static bool assertVectorEquals(vector<double> result, vector<double> expected, double tol=TOL)
	{
		if(result.size() != expected.size()) return false;			

		for(int idx = 0; idx < result.size(); idx++)
		{
			if(std::abs(result[idx] - expected[idx]) > tol)
			{
				return false;				
			}			
		}

		return true;
	}

};

#endif