#ifndef MRNG_H_
#define MRNG_H_

#include <iostream>
#include <random>
#include <chrono>   
#include <stdint.h>

using namespace std;

class mRNG
{
public:
    mRNG(){}
    virtual ~mRNG(){}

    static uint64_t RNG_uint64()
    {
        std::random_device rd;
        unsigned seed;

        // check if the implementation provides a usable random_device
        cout << "RNG entropy is " << rd.entropy(); 
        if (0 != rd.entropy())
        {
           cout << ", using random_device to generate SEED." << endl;
           seed = rd();
        }
        else
        {
           cout << ", using systime to generate SEED." << endl;
           // no random_device available, seed using the system clock
           seed = static_cast<unsigned> (std::chrono::system_clock::now().time_since_epoch().count());
        }

        std::mt19937 rng(seed);
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 4294967296); 

        // RNG warm up
        for(int i = 0; i < 1000; i++) dist(rng);
        return (dist(rng));
    } 
};



#endif