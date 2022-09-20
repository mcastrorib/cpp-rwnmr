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

    // Some Basic Linear Functions
    // Euclidean Norm
    static double norm(vector<double> &vec)
    {

        double sum = 0;
        for (uint element = 0; element < vec.size(); element++)
        {
            sum += vec[element] * vec[element];
        }

        return sqrt(sum);
    }

    // Euclidean Distance
    static double euclideanDistance(vector<double> &vec1, vector<double> &vec2)
    {

        vector<double> difference(vec1.size());
        for(uint i = 0; i < difference.size(); i++)
        {
            difference[i] = vec1[1] - vec2[i];        
        }

        return norm(difference);
    }

    // Intern/'Dot' Product
    static double dotProduct(vector<double> &vec1, vector<double> &vec2)
    {
        double sum = 0;
        for (uint element = 0; element < vec1.size(); element++)
        {
            sum += vec1[element] * vec2[element];
        }

        return sum;
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
    static vector<double> logspace(double exp_start, double exp_end, uint points, double base)
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