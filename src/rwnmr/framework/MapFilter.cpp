#include "MapFilter.h"

MapFilter::MapFilter(Model &_model, double _mapTime, double _filter, double _tol) : model(_model),
                                                                                    mapTime(_mapTime),
                                                                                    mapSteps(0),
                                                                                    tol(_tol)              
{
    (*this).initThreshold(_filter);
}

MapFilter::MapFilter(Model &_model, uint _mapSteps, double _filter, double _tol) : model(_model),
                                                                                    mapTime(0.0),
                                                                                    mapSteps(_mapSteps),
                                                                                    tol(_tol)              
{
    (*this).initThreshold(_filter);
}

void MapFilter::initThreshold(double _filter)
{
    if(_filter > 0.0) 
        this->threshold = (1.0+(*this).getTol()) * (3.0 * (1.0/(double) this->model.getVoxelDivision())) / (4.0 * _filter);
    else 
        this->threshold = _filter;
}

void MapFilter::run()
{
    if((*this).getMapTime() > 0.0)
    { 
        (*this).runMapSimulation((*this).getMapTime());
        return;
    }

    if((*this).getMapSteps() > 0)
    {
        (*this).runMapSimulation((*this).getMapSteps());
        return;
    }

    return;
}

void MapFilter::runMapSimulation(double time)
{
    this->model.buildTimeFramework((*this).getMapTime()); 
    cout << "- Initial map time: " << this->model.getTimeInterval()*this->model.getSimulationSteps() << " ms ";
    cout << "[" << this->model.getSimulationSteps() << " RW-steps]" << endl;
    this->model.mapSimulation();
    (*this).sortWalkersInModel();
}

void MapFilter::runMapSimulation(uint steps)
{
    this->model.buildTimeFramework((*this).getMapSteps()); 
    cout << "- Initial map time: " << this->model.getTimeInterval()*this->model.getSimulationSteps() << " ms ";
    cout << "[" << this->model.getSimulationSteps() << " RW-steps]" << endl;
    this->model.mapSimulation();
    this->model.updateWalkersRelaxativity(this->model.getRwnmrConfig().getRho()[0]);
    (*this).sortWalkersInModel();
}

void MapFilter::sortWalkersInModel()
{
    vector<Walker> *walkers = this->model.getWalkers();
    sort(walkers->begin(), walkers->end(), [](Walker &a, Walker &b) { return a.getXiRate() > b.getXiRate(); });
    walkers = NULL;
}

void MapFilter::filter()
{
    vector<Walker> *walkers = this->model.getWalkers();
    uint thidx = (*this).findThresholdIndex(walkers, (*this).getThreshold());
    if(thidx >= walkers->size() - 1) 
    {
        cout << "Warning: no walkers below filtering xirate." << endl;
        return;
    }

    uint randomIdx;
    std::uniform_int_distribution<std::mt19937::result_type> dist(thidx, walkers->size()-1);
    for(uint i = 0; i < thidx; i++)
    {
        randomIdx = dist(Model::_rng);
        (*walkers)[i].setInitialPosition((*walkers)[randomIdx].getInitialPosition());
    }
    
    walkers = NULL;
}

uint MapFilter::findThresholdIndex(vector<Walker> *walkers, double threshold)
{
    uint thidx = 0;
    while(thidx < walkers->size() and (*walkers)[thidx].getXiRate() > threshold)
    {
        thidx++;
    }
    return thidx;
}




