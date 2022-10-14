#ifndef THREADS_BALANCER_H_
#define THREADS_BALANCER_H_

// include C++ standard libraries
#include <iostream>
#include <string>

using namespace std;

class ThreadsBalancer
{
private:
    double start;
    double finish;

public:
    ThreadsBalancer(const int thread_id, const int num_threads, const int num_loops)
    {
        const int loops_per_thread = num_loops / num_threads;
        (*this).setStart(thread_id * loops_per_thread);
        if (thread_id == (num_threads - 1)) { (*this).setFinish(num_loops); }
        else { (*this).setFinish((*this).getStart() + loops_per_thread); }
    }
    ThreadsBalancer(const ThreadsBalancer &other)
    {
        this->start = other.start;
        this->finish = other.finish;
    }
    virtual ~ThreadsBalancer(){}

    void setStart(int _start) { this->start = _start; }
    void setFinish(int _finish) { this->finish = _finish; }
    int getStart(){ return this->start; }
    int getFinish(){ return this->finish;}    
};

#endif