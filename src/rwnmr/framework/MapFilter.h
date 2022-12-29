#ifndef MAP_FILTER_H_
#define MAP_FILTER_H_

#include "Defs.h"
#include "Model.h"
#include "Walker.h"

class MapFilter
{
private:
    Model &model;
    double mapTime;
    uint mapSteps;
    double threshold;
    double tol;
    uint iterations;

public:	
	MapFilter(Model &_model, double _mapTime, double _mfilter, double _mtol, uint _its = 1);
    MapFilter(Model &_model, uint mapSteps, double _mfilter, double _mtol, uint _its = 1);
	virtual ~MapFilter(){}
    void initThreshold(double _filter);
    void run();
    void filter();
    void runMapSimulation(double time);
    void runMapSimulation(uint steps);
    void sortWalkersInModel();
    uint findThresholdIndex(vector<Walker> *w, double th);

    Model &getModel(){ return this->model; }
    double getMapTime(){ return this->mapTime;}
    uint getMapSteps(){ return this->mapSteps;}
    double getThreshold(){ return this->threshold;}
    double getTol(){ return this->tol;}
    uint getIterations(){ return this->iterations; }

    void setModel(Model &_model){ this->model = _model; }
    void setMapTime(double t){ this->mapTime = t; }
    void setMapSteps(double t){ this->mapSteps = t; }
    void setThreshold(double t){ this->threshold = t; }
    void setTol(double t){ this->tol = t; }
    void setIterations(uint t){ this->iterations = t; }
};

#endif