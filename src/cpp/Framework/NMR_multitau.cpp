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
    (*this).setCPMG(new NMR_cpmg((*this).getModel(), (*this).getCPMGConfig()));

	// vectors object init
    vector<uint> requiredSteps();
    vector<double> signalTimes();
    vector<double> signalAmps();

    (*this).createName();
    (*this).createDirectoryForData();
    (*this).setTauSequence();
}


void NMR_multitau::createName()
{
    int precisionVal = 2;
    string tauMin, tauMax, points;
    string scale = (*this).getMultitauConfig().getTauScale();
    if(scale == "manual")
    {
        vector<double> tauValues = (*this).getMultitauConfig().getTauValues();
        tauMin = std::to_string(tauValues[0]);
        tauMax = std::to_string(tauValues[tauValues.size()-1]);
        points = std::to_string(tauValues.size());
    } else
    {
        tauMin = std::to_string((*this).getMultitauConfig().getTauMin());
        tauMax = std::to_string((*this).getMultitauConfig().getTauMax());
        points = std::to_string((*this).getMultitauConfig().getTauPoints());
    }

    string trimmedTauMin = tauMin.substr(0, std::to_string((*this).getMultitauConfig().getTauMin()).find(".") + precisionVal + 1);
    string trimmedTauMax = tauMax.substr(0, std::to_string((*this).getMultitauConfig().getTauMax()).find(".") + precisionVal + 1);
	(*this).setName("/NMR_multitau_min=" + trimmedTauMin + "ms_max=" + trimmedTauMax + "ms_pts=" + points + "_scale=" + scale);
}

void NMR_multitau::createDirectoryForData()
{
	string path = (*this).getModel().getDBPath();
    BaseFunctions::createDirectory(path, (*this).getModel().simulationName + "/" + (*this).getName());
    (*this).setDir(path + (*this).getModel().simulationName + "/" + (*this).getName());
}

void NMR_multitau::setTauSequence()
{
    double tauMin = (*this).getMultitauConfig().getTauMin();
    double tauMax = (*this).getMultitauConfig().getTauMax();
    int tauPoints = (*this).getMultitauConfig().getTauPoints();
    string scale = (*this).getMultitauConfig().getTauScale();

    vector<double> times;
    if(scale == "manual") times = (*this).getMultitauConfig().getTauValues();
    else if(scale == "log") times = MathFunctions::logspace(log10(tauMin), log10(tauMax), tauPoints);
    else times = MathFunctions::linspace(tauMin, tauMax, tauPoints);

    double timeInterval = (*this).getModel().getTimeInterval();
    if((*this).getRequiredSteps().size() != 0) (*this).clearRequiredSteps();
    if((*this).getSignalTimes().size() != 0) (*this).clearSignalTimes();
    uint minSteps = 0;
    for(uint idx = 0; idx < times.size(); idx++)
    {
        int steps = std::ceil(times[idx]/timeInterval);
        if(steps % 2 != 0) steps++;
        if(steps > minSteps)
        {
            (*this).addRequiredStep(steps);
            minSteps = steps;
        } else
        {
            steps = minSteps + 2;
            (*this).addRequiredStep(steps);
            minSteps = steps;
        }

        (*this).addSignalTime(steps*timeInterval);
    }
}

void NMR_multitau::setExposureTime(uint index)
{
    (*this).getModel().setNumberOfStepsPerEcho((*this).getRequiredStep(index));
    if(!(*this).getMultitauConfig().getCompleteDecay()) 
        (*this).getCPMG()->setExposureTime((*this).getSignalTime(index));
}


void NMR_multitau::setCPMG(uint index)
{
    (*this).setExposureTime(index);
    (*this).getCPMG()->buildModelTimeFramework(false);
    
    int precisionVal = 3;
    string te = std::to_string((*this).getRequiredStep(index) * (*this).getModel().getTimeInterval());
    string sufix = "_te=" + te.substr(0, std::to_string((*this).getRequiredStep(index) * (*this).getModel().getTimeInterval()).find(".") + precisionVal + 1);
    (*this).getCPMG()->buildName((*this).getName(), sufix);
    (*this).getCPMG()->createDirectoryForData();
}

void NMR_multitau::runCPMG()
{
    (*this).getCPMG()->run_simulation();
    int size = (*this).getCPMG()->getSignalAmps().size();
    if(size > 1) 
    {
        cout << "M[0] = " << (*this).getCPMG()->getSignalAmps(0) << endl;
        cout << "M[1] = " << (*this).getCPMG()->getSignalAmps(1) << endl;
    }

    if(size > 0)
    {
        (*this).addSignalAmp((*this).getCPMG()->getSignalAmps(1));
    }
    if((*this).getMultitauConfig().getCompleteDecay())
    {
        (*this).getCPMG()->applyLaplace();
    }
}

void NMR_multitau::saveCPMG()
{
    if((*this).getMultitauConfig().getCompleteDecay()) 
    {
        double time = omp_get_wtime();
        cout << "saving results...";

        if((*this).getMultitauConfig().getSaveWalkers())
        {
            (*this).getCPMG()->writeWalkers();
        }
    
        if((*this).getMultitauConfig().getSaveHistogram())
        {
            (*this).getCPMG()->writeHistogram();
        }    
    
        if((*this).getMultitauConfig().getSaveHistogramList())
        {
            (*this).getCPMG()->writeHistogramList();
        }
    
        if((*this).getMultitauConfig().getSaveDecay()) 
        {
            (*this).getCPMG()->writeT2decay();
            (*this).getCPMG()->writeT2dist();
        }

        time = omp_get_wtime() - time;
        cout << "Ok. (" << time << " seconds)." << endl; 
    }

}

void NMR_multitau::run()
{
    // first of all, reset conditions and map with highest time value
    double tick = omp_get_wtime();
    
    for(uint index = 0; index < (*this).getRequiredSteps().size(); index++)
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
    if((*this).getMultitauConfig().getSaveDecay()) 
    {
        (*this).writeDecay();
    }

	time = omp_get_wtime() - time;
    cout << "Ok. (" << time << " seconds)." << endl; 
}

void NMR_multitau::writeDecay()
{
    string filename = (*this).getDir() + "/multitau_decay.csv";

    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const int num_points = (*this).getSignalAmps().size();
    const int precision = std::numeric_limits<double>::max_digits10;

    file << "echo_time,signal" << endl;
    if((*this).getSignalTimes().size() == (*this).getSignalAmps().size())
    {
        for (int idx = 0; idx < num_points; idx++)
        {
            file << setprecision(precision) << (*this).getSignalTime(idx) << ",";
            file << setprecision(precision) << (*this).getSignalAmp(idx) << endl;    
        }
    }    
    file.close();
}

void NMR_multitau::writeWalkers()
{
    string filename = (*this).getDir() + "/multitau_walkers.csv";
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
    for (uint index = 0; index < (*this).getModel().walkers.size(); index++)
    {
        file << setprecision(precision) << (*this).getModel().walkers[index].getInitialPositionX()
        << "," << (*this).getModel().walkers[index].getInitialPositionY()
        << "," << (*this).getModel().walkers[index].getInitialPositionZ()
        << "," << (*this).getModel().walkers[index].getPositionX() 
        << "," << (*this).getModel().walkers[index].getPositionY() 
        << "," << (*this).getModel().walkers[index].getPositionZ() 
        << "," << (*this).getModel().walkers[index].getCollisions() 
        << "," << (*this).getModel().walkers[index].getXIrate() 
        << "," << (*this).getModel().walkers[index].getEnergy() 
        << "," << (*this).getModel().walkers[index].getInitialSeed() << endl;
    }

    file.close();
}

void NMR_multitau::writeHistogram()
{
    string filename = (*this).getDir() + "/multitau_histogram.csv";
    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    file << "Bins"; 
    file << ",Amps" << endl;
    const int num_points = (*this).getModel().histogram.getSize();
    const int precision = std::numeric_limits<double>::max_digits10;
    for (int i = 0; i < num_points; i++)
    {
        file << setprecision(precision) 
        << (*this).getModel().histogram.getBin(i) 
        << "," << (*this).getModel().histogram.getAmp(i) << endl;
    }

    file.close();
}

void NMR_multitau::writeHistogramList()
{
    string filename = (*this).getDir() + "/multitau_histList.csv";
    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const int histograms = (*this).getModel().histogramList.size();

    for(int hIdx = 0; hIdx < histograms; hIdx++)
    {
        file << "Bins" << hIdx << ",";
        file << "Amps" << hIdx << ",";
    }
    file << endl;

    const int num_points = (*this).getModel().histogram.getSize();
    const int precision = std::numeric_limits<double>::max_digits10;
    for (int i = 0; i < num_points; i++)
    {
        for(int hIdx = 0; hIdx < histograms; hIdx++)
        {
            file << setprecision(precision) << (*this).getModel().histogramList[hIdx].getBin(i) << ",";
            file << setprecision(precision) << (*this).getModel().histogramList[hIdx].getAmp(i) << ",";
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