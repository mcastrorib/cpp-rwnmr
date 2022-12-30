#include "MapFilter.h"

MapFilter::MapFilter(Model &_model, double _mapTime, double _filter, double _tol, uint _it) : model(_model),
                                                                                              mapTime(_mapTime), 
                                                                                              mapSteps(0), 
                                                                                              tol(_tol), 
                                                                                              iterations(_it),
                                                                                              thresholdIndex(0),
                                                                                              filteredPercentual(0.0)              
{
    (*this).initThreshold(_filter);
}

MapFilter::MapFilter(Model &_model, uint _mapSteps, double _filter, double _tol, uint _it) : model(_model),
                                                                                              mapTime(0), 
                                                                                              mapSteps(_mapSteps), 
                                                                                              tol(_tol), 
                                                                                              iterations(_it),
                                                                                              thresholdIndex(0),
                                                                                              filteredPercentual(0.0)              
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
    if((*this).getMapTime() == 0.0 and (*this).getMapSteps() == 0)
        return;

    if((*this).getMapTime() > 0.0)
    { 
        this->model.buildTimeFramework((*this).getMapTime()); 
    } else 
    {
        this->model.buildTimeFramework((*this).getMapSteps());
    }

    (*this).runMapSimulation();     
    return;
}

void MapFilter::runMapSimulation()
{
    cout << "- Initial map time: " << this->model.getTimeInterval()*this->model.getSimulationSteps() << " ms ";
    cout << "[" << this->model.getSimulationSteps() << " RW-steps]" << endl;
    this->model.mapSimulation();
    this->model.updateWalkersRelaxativity(this->model.getRwnmrConfig().getRho()[0]);
    (*this).sortWalkersInModel();
    (*this).findThresholdIndex(this->model.getWalkers(), (*this).getThreshold());
            
    if((*this).getThreshold() > 0.0)
    {
        for(uint it = 0; it < (*this).getIterations(); it++)
        {   
            (*this).filter();
            this->model.mapSimulation();
            this->model.updateWalkersRelaxativity(this->model.getRwnmrConfig().getRho()[0]);
            (*this).sortWalkersInModel();
            (*this).findThresholdIndex(this->model.getWalkers(), (*this).getThreshold());
            (*this).computeFilteredPercentual(this->model.getWalkers(), (*this).getThresholdIndex());         
            cout << "it: " << it+1 << "/" << (*this).getIterations() << ": " << 100.0*(*this).getFilteredPercentual() << "\% filtered." << endl; 
        }    
    }    
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
    if((*this).getThresholdIndex() >= walkers->size() - 1) 
    {
        cout << "Warning: no walkers below filtering xirate." << endl;
        return;
    }

    uint randomIdx;
    std::uniform_int_distribution<std::mt19937::result_type> dist((*this).getThresholdIndex(), walkers->size()-1);
    for(uint i = 0; i < (*this).getThresholdIndex(); i++)
    {
        randomIdx = dist(Model::_rng);
        (*walkers)[i].setInitialPosition((*walkers)[randomIdx].getInitialPosition());
    }
    
    walkers = NULL;
}

void MapFilter::findThresholdIndex(vector<Walker> *walkers, double threshold)
{
    uint thidx = 0;
    while(thidx+1 < walkers->size() and (*walkers)[thidx].getXiRate() > threshold)
    {
        thidx++;
    }
    (*this).setThresholdIndex(thidx);
}

void MapFilter::computeFilteredPercentual(vector<Walker> *walkers, uint thidx)
{
    double size = (double) walkers->size();
    (*this).setFilteredPercentual((double) (thidx + 1) / size);
}



