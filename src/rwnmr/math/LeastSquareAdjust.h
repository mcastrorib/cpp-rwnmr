#ifndef LSADJUST_H
#define LSADJUST_H

#include <iostream>
#include <vector>

using namespace std;

class LeastSquareAdjust
{
public:    
    LeastSquareAdjust(vector<double> &_x, vector<double> &_y, bool _intercept = true);
    virtual ~LeastSquareAdjust(){}
    void solve();

    void setX(vector<double> &_x);
    void setY(vector<double> &_y);
    void setIntercept(bool _intercept){ this->intercept = _intercept; }
    void setVerbose(bool _verbose) { this->verbose = _verbose; }
    void setPoints(int _points);
    void setThreshold(double _threshold);
    void setLimits();

    bool isVerbose() { return this->verbose; }

    vector<double> &getX(){ return this->X; }
    vector<double> &getY(){ return this->Y; }
    bool getIntercept(){ return this->intercept; }
    bool getVerbose(){ return this->verbose; }
    double getMeanX(){ return this->meanX; }
    double getMeanY(){ return this->meanY; }
    double getA();
    double getB();
    double getResidual(){ return this->residual; }
    bool getSolved(){ return this->solved; }
    int getBegin(){ return this->begin; }
    int getEnd(){ return this->end; }
    int getPoints(){ return this->points; }
    double getThreshold(){ return this->threshold; }

    bool isSolved() { return this->solved; }
    bool hasIntercept() { return this->intercept; }
    double getMSE();
    double getSMSE();

private:    
    vector<double> &X;
    vector<double> &Y;
    bool intercept;
    bool verbose;
    double meanX;
    double meanY;
    double A;
    double B;
    double residual;
    bool solved;

    int begin;
    int end;
    int points;
    double threshold;

    double computeMean(vector<double> &_vector);
    void computeB();
    void computeBWithIntercept();
    void computeBWithoutIntercept();
    void computeA();
    void computeAWithIntercept();
    void computeAWithoutIntercept();
    void setA(double _a){ this->A = _a; }
    void setB(double _b){ this->B = _b; }
    void setResidual(double _res){ this->residual = _res; }
    void setSolved(bool isSolved);
    void computeMeanSquaredResiduals(); 
    double evaluate(double point);
};

#endif
