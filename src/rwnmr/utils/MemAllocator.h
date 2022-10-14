#ifndef MEM_ALLOCATOR_H_
#define MEM_ALLOCATOR_H_

// include C++ standard libraries
#include <iostream>
#include <string>

using namespace std;

class MemAllocator
{
public:    
    MemAllocator(){};
    MemAllocator(const MemAllocator &_otherAllocator);
    virtual ~MemAllocator(){}

    template <typename T>
    static bool checkAllocation(T *ptr, bool _exit=true)
    {
        if(ptr == NULL)
        {
            printf("Memory allocation failed!\n");
            if(_exit) exit(1); 
            return false;
        }

        return true;
    }

    static int* mallocIntArray(uint size)
    {
        int *array;
        array = (int *)malloc(size * sizeof(int));
        MemAllocator::checkAllocation(array);
        return array;
    }

    static uint* mallocUIntArray(uint size)
    {
        uint *array;
        array = (uint *)malloc(size * sizeof(uint));
        MemAllocator::checkAllocation(array);
        return array;
    }

    static double* mallocDoubleArray(uint size)
    {
        double *array;
        array = (double *)malloc(size * sizeof(double));
        MemAllocator::checkAllocation(array);
        return array;
    }

    static float* mallocFloatArray(uint size)
    {
        float *array;
        array = (float *)malloc(size * sizeof(float));
        MemAllocator::checkAllocation(array);
        return array;
    }


    static uint64_t* mallocUInt64Array(uint size)
    {
        uint64_t *array;
        array = (uint64_t *)malloc(size * sizeof(uint64_t));
        MemAllocator::checkAllocation(array);
        return array;
    }  
};

#endif