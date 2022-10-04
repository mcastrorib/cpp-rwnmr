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

    static void showblock(uint64_t x)
    {
        int lineCounter = 0;

        for (int i = 0; i < (sizeof(uint64_t) * 8); i++)
        {
            (x & (1ull << i) ? cout << "1" : cout << "0");
            cout << "  ";

            if (lineCounter == 7)
            {
                cout << endl;
                lineCounter = -1;
            }

            lineCounter++;
        }
        cout << endl;
    }

    // small function to print int64_t in bits
    static void showbits(uint64_t x)
    {
        for (int i = (sizeof(uint64_t) * 8) - 1; i >= 0; i--)
        {
            (x & (1ull << i) ? cout << "1" : cout << "0");
        }
        cout << endl;
    }

    // small function to print int64_t in an 8 by 8 bitblock
    static void showblock8x8(uint64_t x)
    {
        int lineCounter = 0;

        for (int i = 0; i < (sizeof(uint64_t) * 8); i++)
        {
            (x & (1ull << i) ? cout << "1" : cout << "0");
            cout << "  ";

            if (lineCounter == 7)
            {
                cout << endl;
                lineCounter = -1;
            }

            lineCounter++;
        }
        cout << endl;
    }

    // small function to print int64_t in an 4 by 4 by 4 cubic bitblock
    static void showblock4x4x4(uint64_t x)
    {
        int lineCounter = 0;
        int baseCounter = 0;

        for (int i = 0; i < (sizeof(uint64_t) * 8); i++)
        {
            (x & (1ull << i) ? cout << "1" : cout << "0");
            cout << "  ";

            if (lineCounter == 3)
            {
                cout << endl;
                lineCounter = -1;
                if (baseCounter == 15)
                {
                    cout << endl;
                    baseCounter = -1;
                }
            }

            lineCounter++;
            baseCounter++;
        }
        cout << endl;
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
};
#endif