#ifndef MATH_FUNCTIONS_H_
#define MATH_FUNCTIONS_H_

// include C++ standard libraries
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class MathFunctions 
{
    public:
    MathFunctions(){}
	virtual ~MathFunctions (){}

    static vector<double> zerosVector(uint size)
    {
        vector<double> zvec;       
        zvec.reserve(size);
        for(uint i = 0; i < size; i++)
        {
            zvec.push_back(0);
        }
        return zvec;
    }

    static vector<double> onesVector(uint size)
    {
        vector<double> ovec;       
        ovec.reserve(size);
        for(uint i = 0; i < size; i++)
        {
            ovec.push_back(1);
        }
        return ovec;
    }

    template <typename T>
    static vector<T> addVectors(vector<T> a, vector<T> b)
    {
        vector<T> ab;
        if(a.size() != b.size()) 
            throw "incompatible sizes";
        
        ab.reserve(a.size());
        for(uint i = 0; i < a.size(); i++)
        {
            ab.push_back(a[i]+b[i]);
        }
        return ab;
    }

    template <typename T>
    static vector<T> multiplyVector(T a, vector<T> b)
    {
        vector<T> ab;       
        ab.reserve(b.size());
        for(uint i = 0; i < b.size(); i++)
        {
            ab.push_back(a*b[i]);
        }
        return ab;
    }

    template <typename T>
    static vector<T> multiplyVectors(vector<T> a, vector<T> b)
    {
        vector<T> ab;       
        ab.reserve(a.size());
        for(uint i = 0; i < a.size(); i++)
        {
            ab.push_back(a[i]*b[i]);
        }
        return ab;
    }

    // Intern/'Dot' Product
    template <typename T>
    static T dotProduct(vector<T> a, vector<T> b)
    {
        return MathFunctions::sum(MathFunctions::multiplyVectors(a,b));
    }

    // Some Basic Linear Functions
    // Euclidean Norm
    static double norm(vector<double> vec)
    {
        return sqrt(MathFunctions::dotProduct(vec,vec));
    }

    // Euclidean Distance
    static double euclideanDistance(vector<double> vec1, vector<double> vec2)
    {
        vector<double> diff = MathFunctions::addVectors(vec1, MathFunctions::multiplyVector(-1.0, vec2));
        return MathFunctions::norm(diff);
    }

    template <typename T>
    static T sum(vector<T> _vec)
    {
        T sum = 0;
        for(uint idx = 0; idx < _vec.size(); idx++) sum += _vec[idx];
        return sum;
    }    

    template <typename T>
    static T sum(T *_vec, uint _size)
    {
        T sum = 0;
        for(uint idx = 0; idx < _size; idx++) sum += _vec[idx];
        return sum;
    }

    template <typename T>
    static double mean(vector<T> _vec)
    {
        return (MathFunctions::sum(_vec) /  (double) _vec.size() );
    }

    template <typename T>
	static double mean(T *_vec, uint _size)
    {
        return (MathFunctions::sum(_vec, _size) /  (double) _size );
    }

    template <typename T>
	static double stdDev(vector<T> _vec)
    {
        double mean = MathFunctions::mean(_vec);
        vector<double> disp = MathFunctions::addVectors(_vec, MathFunctions::multiplyVector(mean, MathFunctions::onesVector(_vec.size())));
        double sum = MathFunctions::sum(MathFunctions::multiplyVectors(disp,disp));
        double dsize = _vec.size();
        return sqrt(sum/dsize);
    }

    template <typename T>
    static double stdDev(T *_vec, uint _size)
    {
        double mean = MathFunctions::mean(_vec, _size);
        double sum = 0.0;
        double dsize = (double) _size;

        for(uint idx = 0; idx < _size; idx++)
        {
            sum += ((_vec[idx] - mean) * (_vec[idx] - mean)); 
        }

        return sqrt(sum/dsize);
    }
	
    // Returns a vector<double> linearly space from @start to @end with @points
    static vector<double> linspace(double start, double end, uint points)
    {
        vector<double> vec(points);
        double step = (end - start) / ((double) points - 1.0);
        
        for(int idx = 0; idx < points; idx++)
        {
            double x_i = start + step * idx;
            vec[idx] = x_i;
        }

        return vec;
    }

    // Returns a vector<double> logarithmly space from 10^@exp_start to 10^@end with @points
    static vector<double> logspace(double exp_start, double exp_end, uint points, double base=10.0)
    {
        vector<double> vec(points);
        double step = (exp_end - exp_start) / ((double) points - 1.0);
        
        for(int idx = 0; idx < points; idx++)
        {
            double x_i = exp_start + step * idx;
            vec[idx] = pow(base, x_i);
        }

        return vec;
    }

};
#endif