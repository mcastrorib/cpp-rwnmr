#include "NMR_multitau.h"

using namespace std;

NMR_multitau::NMR_multitau( Model &_model,  
                            multitau_config _multitauConfig,  
                            cpmg_config _cpmgConfig,
                            int _mpi_rank,
                            int _mpi_processes) : model(_model),
                                                  cpmg(NULL), 
                                                  MultiTau_config(_multitauConfig), 
                                                  CPMG_config(_cpmgConfig), 
                                                  mpi_rank(_mpi_rank), 
                                                  mpi_processes(_mpi_processes)
{
    // Initialize cpmg object
    this->cpmg = new NMR_cpmg(this->model, this->CPMG_config);

	// vectors object init
    vector<uint> requiredSteps();
    vector<double> signalTimes();
    vector<double> signalAmps();

    (*this).setName();
    (*this).createDirectoryForData();
    (*this).setTauSequence();
}


void NMR_multitau::setName()
{
    int precisionVal = 2;
    string tauMin, tauMax, points;
    string scale = this->MultiTau_config.getTauScale();
    if(scale == "manual")
    {
        vector<double> tauValues = this->MultiTau_config.getTauValues();
        tauMin = std::to_string(tauValues[0]);
        tauMax = std::to_string(tauValues[tauValues.size()-1]);
        points = std::to_string(tauValues.size());
    } else
    {
        tauMin = std::to_string(this->MultiTau_config.getTauMin());
        tauMax = std::to_string(this->MultiTau_config.getTauMax());
        points = std::to_string(this->MultiTau_config.getTauPoints());
    }

    string trimmedTauMin = tauMin.substr(0, std::to_string(this->MultiTau_config.getTauMin()).find(".") + precisionVal + 1);
    string trimmedTauMax = tauMax.substr(0, std::to_string(this->MultiTau_config.getTauMax()).find(".") + precisionVal + 1);
	this->name = "/NMR_multitau_min=" + trimmedTauMin + "ms_max=" + trimmedTauMax + "ms_pts=" + points + "_scale=" + scale;
}

void NMR_multitau::createDirectoryForData()
{
	string path = this->model.getDBPath();
    BaseFunctions::createDirectory(path, this->model.simulationName + "/" + this->name);
    this->dir = (path + this->model.simulationName + "/" + this->name);
}

void NMR_multitau::setTauSequence()
{
    double tauMin = this->MultiTau_config.getTauMin();
    double tauMax = this->MultiTau_config.getTauMax();
    int tauPoints = this->MultiTau_config.getTauPoints();
    string scale = this->MultiTau_config.getTauScale();

    vector<double> times;
    if(scale == "manual") times = this->MultiTau_config.getTauValues();
    else if(scale == "log") times = (*this).logspace(log10(tauMin), log10(tauMax), tauPoints);
    else times = (*this).linspace(tauMin, tauMax, tauPoints);

    double timeInterval = this->model.getTimeInterval();
    if(this->requiredSteps.size() != 0) this->requiredSteps.clear();
    if(this->signalTimes.size() != 0) this->signalTimes.clear();
    uint minSteps = 0;
    for(uint idx = 0; idx < times.size(); idx++)
    {
        int steps = std::ceil(times[idx]/timeInterval);
        if(steps % 2 != 0) steps++;
        if(steps > minSteps)
        {
            requiredSteps.push_back(steps);
            minSteps = steps;
        } else
        {
            steps = minSteps + 2;
            requiredSteps.push_back(steps);
            minSteps = steps;
        }

        signalTimes.push_back(steps*timeInterval);
    }
}

void NMR_multitau::setExposureTime(uint index)
{
    this->model.setNumberOfStepsPerEcho(this->requiredSteps[index]);
    if(!this->MultiTau_config.getCompleteDecay()) 
        this->cpmg->setExposureTime(this->signalTimes[index]);
}


void NMR_multitau::setCPMG(uint index)
{
    (*this).setExposureTime(index);
    this->cpmg->setNMRTimeFramework(false);
    
    int precisionVal = 3;
    string te = std::to_string(this->requiredSteps[index] * this->model.getTimeInterval());
    string sufix = "_te=" + te.substr(0, std::to_string(this->requiredSteps[index] * this->model.getTimeInterval()).find(".") + precisionVal + 1);
    this->cpmg->setName(this->name, sufix);
    this->cpmg->createDirectoryForData();
}

void NMR_multitau::runCPMG()
{
    this->cpmg->run_simulation();
    int size = this->cpmg->signal_amps.size();
    if(size > 1) 
    {
        cout << "M[0] = " << this->cpmg->signal_amps[0] << endl;
        cout << "M[1] = " << this->cpmg->signal_amps[1] << endl;
    }

    if(size > 0)
    {
        this->signalAmps.push_back(this->cpmg->signal_amps[1]);
    }
    if(this->MultiTau_config.getCompleteDecay())
    {
        this->cpmg->applyLaplace();
    }
}

void NMR_multitau::saveCPMG()
{
    if(this->MultiTau_config.getCompleteDecay()) 
    {
        double time = omp_get_wtime();
        cout << "saving results...";

        if(this->MultiTau_config.getSaveWalkers())
        {
            this->cpmg->writeWalkers();
        }
    
        if(this->MultiTau_config.getSaveHistogram())
        {
            this->cpmg->writeHistogram();
        }    
    
        if(this->MultiTau_config.getSaveHistogramList())
        {
            this->cpmg->writeHistogramList();
        }
    
        if(this->MultiTau_config.getSaveDecay()) 
        {
            this->cpmg->writeT2decay();
            this->cpmg->writeT2dist();
        }

        time = omp_get_wtime() - time;
        cout << "Ok. (" << time << " seconds)." << endl; 
    }

}

void NMR_multitau::run()
{
    // first of all, reset conditions and map with highest time value
    double tick = omp_get_wtime();
    
    for(uint index = 0; index < this->requiredSteps.size(); index++)
    {
        (*this).setCPMG(index);
        (*this).runCPMG();
        (*this).saveCPMG();
        cout << endl;
    }

    (*this).save();

    double time = omp_get_wtime() - tick;
    cout << endl << "multitau_time: " << time << " seconds." << endl;
}

// -- Savings
void NMR_multitau::save()
{
	double time = omp_get_wtime();
    cout << "saving results...";
    
    // write multitau data
    if(this->MultiTau_config.getSaveDecay()) 
    {
        (*this).writeDecay();
    }

	time = omp_get_wtime() - time;
    cout << "Ok. (" << time << " seconds)." << endl; 
}

void NMR_multitau::writeDecay()
{
    string filename = this->dir + "/multitau_decay.csv";

    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const int num_points = this->signalAmps.size();
    const int precision = std::numeric_limits<double>::max_digits10;

    file << "echo_time,signal" << endl;
    if(this->signalTimes.size() == this->signalAmps.size())
    {
        for (int idx = 0; idx < num_points; idx++)
        {
            file << setprecision(precision) << this->signalTimes[idx] << ",";
            file << setprecision(precision) << this->signalAmps[idx] << endl;    
        }
    }    
    file.close();
}

void NMR_multitau::writeWalkers()
{
    string filename = this->dir + "/multitau_walkers.csv";
    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    file << "PositionXi";
    file << ",PositionYi";
    file << ",PositionZi";
    file << ",PositionXf";
    file << ",PositionYf";
    file << ",PositionZf";
    file << ",Collisions";
    file << ",XIRate";
    file << ",Energy"; 
    file << ",RNGSeed" << endl;

    const int precision = 6;
    for (uint index = 0; index < this->model.walkers.size(); index++)
    {
        file << setprecision(precision) << this->model.walkers[index].getInitialPositionX()
        << "," << this->model.walkers[index].getInitialPositionY()
        << "," << this->model.walkers[index].getInitialPositionZ()
        << "," << this->model.walkers[index].getPositionX() 
        << "," << this->model.walkers[index].getPositionY() 
        << "," << this->model.walkers[index].getPositionZ() 
        << "," << this->model.walkers[index].getCollisions() 
        << "," << this->model.walkers[index].getXIrate() 
        << "," << this->model.walkers[index].getEnergy() 
        << "," << this->model.walkers[index].getInitialSeed() << endl;
    }

    file.close();
}

void NMR_multitau::writeHistogram()
{
    string filename = this->dir + "/multitau_histogram.csv";
    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    file << "Bins"; 
    file << ",Amps" << endl;
    const int num_points = this->model.histogram.getSize();
    const int precision = std::numeric_limits<double>::max_digits10;
    for (int i = 0; i < num_points; i++)
    {
        file << setprecision(precision) 
        << this->model.histogram.bins[i] 
        << "," << this->model.histogram.amps[i] << endl;
    }

    file.close();
}

void NMR_multitau::writeHistogramList()
{
    string filename = this->dir + "/multitau_histList.csv";
    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const int histograms = this->model.histogramList.size();

    for(int hIdx = 0; hIdx < histograms; hIdx++)
    {
        file << "Bins" << hIdx << ",";
        file << "Amps" << hIdx << ",";
    }
    file << endl;

    const int num_points = this->model.histogram.getSize();
    const int precision = std::numeric_limits<double>::max_digits10;
    for (int i = 0; i < num_points; i++)
    {
        for(int hIdx = 0; hIdx < histograms; hIdx++)
        {
            file << setprecision(precision) << this->model.histogramList[hIdx].bins[i] << ",";
            file << setprecision(precision) << this->model.histogramList[hIdx].amps[i] << ",";
        }

        file << endl;
    }

    file.close();
}

// Returns a vector<double> linearly space from @start to @end with @points
vector<double> NMR_multitau::linspace(double start, double end, uint points)
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
vector<double> NMR_multitau::logspace(double exp_start, double exp_end, uint points, double base)
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

double NMR_multitau::sum(vector<double> _vec)
{
    double sum = 0.0;
    for(uint idx = 0; idx < _vec.size(); idx++)
    {
        sum += _vec[idx];
    }
    return sum;
}

int NMR_multitau::sum(vector<int> _vec)
{
    int sum = 0;
    for(uint idx = 0; idx < _vec.size(); idx++)
    {
        sum += _vec[idx];
    }
    return sum;
}

uint NMR_multitau::sum(vector<uint> _vec)
{
    uint sum = 0;
    for(uint idx = 0; idx < _vec.size(); idx++)
    {
        sum += _vec[idx];
    }
    return sum;
}