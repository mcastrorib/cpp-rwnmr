#ifndef BASE_FUNCTIONS_H_
#define BASE_FUNCTIONS_H_

// include C libraries for filesystem access
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// include C++ standard libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <vector>
#include <cstring>
#include <string>
#include <stdint.h>

// standad C libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

using namespace std;

class BaseFunctions 
{
    public:
    BaseFunctions(){}
	virtual ~BaseFunctions (){}

    // small function to print int64_t in bits
    static string convertUint64ToBitString(uint64_t x)
    {
        string bstr = "";
        for (int i = (sizeof(uint64_t) * 8) - 1; i >= 0; i--)
        {
            (x & (1ull << i) ? bstr += "1" : bstr += "0");
        }
        return bstr;
    }

    // small function to print int64_t in an 8 by 8 bitblock
    static string convertBlock2dToString(uint64_t x)
    {
        string bstr = "";
        int lineCounter = 0;
        
        for (int i = 0; i < (sizeof(uint64_t) * 8); i++)
        {
            (x & (1ull << i) ? bstr += " 1" : bstr += " 0");
            
            if (lineCounter == 7)
            {
                bstr +="\n";
                lineCounter = -1;
            }

            lineCounter++;
        }

        return bstr;
    }

    // small function to print int64_t in an 4 by 4 by 4 cubic bitblock
    static string convertBlock3dToString(uint64_t x)
    {
        string bstr = "";
        int lineCounter = 0;
        int baseCounter = 0;

        for (int i = 0; i < (sizeof(uint64_t) * 8); i++)
        {
            (x & (1ull << i) ? bstr += " 1" : bstr += " 0");
            
            if (lineCounter == 3)
            {
                bstr += "\n";
                lineCounter = -1;
                if (baseCounter == 15)
                {
                    bstr += "\n";
                    baseCounter = -1;
                }
            }

            lineCounter++;
            baseCounter++;
        }
        return bstr;
    }

    template <typename T>
    static string convertToBits(T x)
    {
        string bstr = "";
        for (int i = (sizeof(T) * 8) - 1; i >= 0; i--)
        {
            (x & (1ull << i) ? bstr += "1" : bstr += "0");
        }
        return bstr;
    }


    template <typename T>
    static string convertBlockToBitString(T x, int dimX, int dimY=1, int dimZ=1)
    {
        string bstr = BaseFunctions::convertToBits(x);
        if(dimX*dimY*dimZ != bstr.length()) 
            return (string)"Error: bad dims";
        
        string finalStr = "";
        int lineCounter = 0;
        int baseCounter = 0;

        for (int i = bstr.length(); i > 0; i--)
        {
            finalStr += " ";
            finalStr += bstr.substr(i-1,1);

            if (lineCounter == (dimX-1))
            {
                finalStr += "\n";
                lineCounter = -1;
                
                if (baseCounter == (dimY - 1))
                {
                    finalStr += "\n";
                    baseCounter = -1;
                }

                baseCounter++;
            }

            lineCounter++;
        }
        return finalStr;
    }

    static void printElapsedTime(double start, double finish)
    {
        cout << "\t\tElapsed time: ";
        cout << (finish - start) << endl;
    }

    static void createDirectory(string path, string name)
    {
        string fullPath = path + name;

        char directory[fullPath.size() + 1];
        strcpy(directory, fullPath.c_str());

        struct stat st = {0};

        if (stat(directory, &st) == -1)
        {
            mkdir(directory, 0700);
        }
    }

    template <typename T>
	static vector<T> concatenateVectors(vector<T> v1, vector<T> v2)
	{
		vector<T> concat;
		for(int i = 0; i < v1.size(); i++) concat.push_back(v1[i]);
		for(int i = 0; i < v2.size(); i++) concat.push_back(v2[i]);
		return concat;
	}
};
#endif