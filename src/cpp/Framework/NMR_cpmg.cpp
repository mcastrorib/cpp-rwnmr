#include "NMR_cpmg.h"

NMR_cpmg::NMR_cpmg( Model &_model,  
                    cpmg_config _cpmgConfig,
                    int _mpi_rank,
                    int _mpi_processes) : model(_model),
                                        CPMG_config(_cpmgConfig),
                                        mpi_rank(_mpi_rank),
                                        mpi_processes(_mpi_processes),
                                        penalties(NULL),
                                        internalField(NULL)
{
	// vectors object init
    vector<double> signal_amps();
    vector<double> signal_times();
    vector<double> T2_bins();
    vector<double> T2_amps();
    vector<double> noise();

    (*this).setExposureTime(this->CPMG_config.getObservationTime());
    (*this).setApplyBulkRelaxation(this->CPMG_config.getApplyBulk());
    (*this).setMethod(this->CPMG_config.getMethod());
    (*this).setInternalField(this->CPMG_config.getResidualField());
    // (*this).set();
}

void NMR_cpmg::setInternalField(string _mode)
{
    if(_mode == "uniform")
    {
        this->internalField = new InternalField(this->model.bitBlock, 
                                                this->model.getImageResolution(), 
                                                this->CPMG_config.getGradientValue(), 
                                                this->CPMG_config.getGradientDirection());
    } 
    else if(_mode == "import")
    {
        this->internalField = new InternalField(this->model.bitBlock, 
                                                this->CPMG_config.getPathToField());
    } 

    (*this).checkInternalField();
}

void NMR_cpmg::checkInternalField()
{
    if(this->internalField != NULL)
    {
        if(this->model.getImageWidth() != this->internalField->getDimX() 
           or this->model.getImageHeight() != this->internalField->getDimY() 
           or this->model.getImageDepth() != this->internalField->getDimZ())
        {
            cout << "Internal field dimensions doesnt match image data." << endl;
            exit(1);
        }
    }
}

void NMR_cpmg::set()
{
	(*this).setName();
	(*this).createDirectoryForData();
	(*this).setNMRTimeFramework();
}

void NMR_cpmg::run()
{
    (*this).set();
    (*this).run_simulation();
    (*this).applyLaplace();
    (*this).save();
}


void NMR_cpmg::setName(string parent, string sufix)
{
    int precisionVal = 2;
    string bigDelta = std::to_string(this->exposureTime);
    string trimmedDelta = bigDelta.substr(0, std::to_string(this->exposureTime).find(".") + precisionVal + 1);
	this->name = parent + "/NMR_cpmg_t=" + trimmedDelta + "ms" + sufix;
}

void NMR_cpmg::createDirectoryForData()
{
	string path = this->model.getDBPath();
    BaseFunctions::createDirectory(path, this->model.simulationName + "/" + this->name );
    this->dir = (path + this->model.simulationName + "/" + this->name);
}


void NMR_cpmg::setNMRTimeFramework(bool _map)
{
    this->model.setTimeFramework((*this).getExposureTime()); 

    // reserve memory space for signal amps
    if(this->signal_amps.size() != 0) this->signal_amps.clear();
    this->signal_amps.reserve(this->model.getNumberOfEchoes() + 1);
    
    // reserve memory space for signal times
    double time = 0.0;
    double delta_t = this->model.getStepsPerEcho() * this->model.getTimeInterval();
    if(this->signal_times.size() != 0) this->signal_times.clear();
    this->signal_times.reserve(this->model.getNumberOfEchoes() + 1);
    this->signal_times.push_back(time);
    for(int echo = 0; echo < this->model.getNumberOfEchoes(); echo++)
    {   
        time += delta_t;
        this->signal_times.push_back(time);
    }

    cout << "- Initial map time: " << (*this).getExposureTime() << " ms ";
    cout << "[" << this->model.simulationSteps << " RW-steps]" << endl;
    if(_map) this->model.mapSimulation();

    // Update xi_rate and relaxivity of walkers
    vector<double> rho = this->model.rwNMR_config.getRho();
    if(this->model.rwNMR_config.getRhoType() == "uniform")    
    {
        this->model.updateWalkersRelaxativity(rho[0]);
    } 
    else if(this->model.rwNMR_config.getRhoType() == "sigmoid")
    {
        this->model.updateWalkersRelaxativity(rho);
    }
}

// -- Simulations
void NMR_cpmg::run_simulation()
{
    if((*this).getMethod() == "image-based")
    {
        // Choose method considering GPU usage
        if(this->model.getGPU()) (*this).image_simulation_cuda();
        else (*this).image_simulation_omp();

    } else
    if((*this).getMethod() == "histogram")
    {
        vector<double> rho;
        rho = this->model.rwNMR_config.getRho();
        if(this->model.rwNMR_config.getRhoType() == "uniform") (*this).createPenaltiesVector(rho[0]);
        else if(this->model.rwNMR_config.getRhoType() == "sigmoid") (*this).createPenaltiesVector(rho);
        (*this).histogram_simulation();
    }

    // Apply bulk relaxation
    if((*this).getApplyBulkRelaxation())
    {
        (*this).applyBulk();
    }

    // Normalize global energy decay 
    (*this).normalizeSignal();
}

void NMR_cpmg::image_simulation_omp()
{
    double begin_time = omp_get_wtime();

    cout << "initializing CPMG-NMR simulation... ";

    for (uint id = 0; id < this->model.walkers.size(); id++)
    {
        this->model.walkers[id].resetPosition();
        this->model.walkers[id].resetSeed();
        this->model.walkers[id].resetEnergy();
    }

    // reset vector to store energy decay
    this->resetSignal();
    this->signal_amps.reserve(this->model.numberOfEchoes);

    // get initial energy state
    double energySum = 0.0;
    for (uint id = 0; id < this->model.walkers.size(); id++)
    {
        energySum += this->model.walkers[id].energy;
    }
    this->signal_amps.push_back(energySum);


    energySum = 0.0;
    uint id, step;
    for (uint echo = 0; echo < this->model.numberOfEchoes; echo++)
    {

        // walkers walk some steps with omp parallel
        // #pragma omp parallel for if(NMR_OPENMP) private(id, step) shared(walkers, bitBlock, simulationSteps)
        for (id = 0; id < this->model.numberOfWalkers; id++)
        {
            for (step = 0; step < this->model.stepsPerEcho; step++)
            {
                this->model.walkers[id].walk(this->model.bitBlock);
            }
        }

        // collect energy from all walkers with omp reduce
        energySum = 0.0; // reset energy summation
        // #pragma omp parallel for if(NMR_OPENMP) reduction(+:energySum) private(id) shared(walkers)
        for (id = 0; id < this->model.numberOfWalkers; id++)
        {
            energySum += this->model.walkers[id].energy;
        }

        //energySum = energySum / (double)numberOfWalkers;
        this->signal_amps.push_back(energySum);
    }

    cout << "Completed.";
    double finish_time = omp_get_wtime();
    BaseFunctions::printElapsedTime(begin_time, finish_time);
}

void NMR_cpmg::createPenaltiesVector(vector<double> &_sigmoid)
{
    // initialize penalties array
    if(this->penalties != NULL)
    {
        delete[] this->penalties;
        this->penalties = NULL;
    } 
    this->penalties = new double[this->model.histogram.getSize()];
    
    Walker toy;
    double artificial_xirate;
    double artificial_steps = (double) this->model.getStepsPerEcho();
    for(int idx = 0; idx < this->model.histogram.getSize(); idx++)
    {   
        artificial_xirate = this->model.histogram.getBin(idx);
        toy.setXIrate(artificial_xirate);
        toy.setSurfaceRelaxivity(_sigmoid);
        toy.computeDecreaseFactor(this->model.getImageVoxelResolution(), this->model.getDiffusionCoefficient());
        this->penalties[idx] = pow(toy.getDecreaseFactor(), (artificial_xirate * artificial_steps));
    }
}

void NMR_cpmg::createPenaltiesVector(double rho)
{
    // initialize penalties array
    if(this->penalties != NULL)
    {
        delete[] this->penalties;
        this->penalties = NULL;
    } 
    this->penalties = new double[this->model.histogram.getSize()];
    
    Walker toy;
    double artificial_xirate;
    double artificial_steps = (double) this->model.getStepsPerEcho();
    for(int idx = 0; idx < this->model.histogram.getSize(); idx++)
    {   
        artificial_xirate = this->model.histogram.getBin(idx);
        toy.setXIrate(artificial_xirate);
        toy.setSurfaceRelaxivity(rho);
        toy.computeDecreaseFactor(this->model.getImageVoxelResolution(), this->model.getDiffusionCoefficient());
        this->penalties[idx] = pow(toy.getDecreaseFactor(), (artificial_xirate * artificial_steps));
    }
}

void NMR_cpmg::histogram_simulation()
{
    double beginTime = omp_get_wtime();
    string bc = this->model.boundaryCondition;
    cout << "- starting RW-CPMG simulation (histrogram) [bc:" << bc << "]...";

    if(this->model.histogramList.size() == 0)  
    {
        cout << "could not start simulation without histogram list" << endl;
        return;
    }
    if(this->penalties == NULL)
    {
        cout << "could not start simulation without penalties vector" << endl;
        return;
    }

    // initialize energyDistribution array
    double *energyDistribution = NULL;
    energyDistribution = new double[this->model.histogram.getSize()];


    // reset vector to store energy decay
    (*this).resetSignal();
    this->signal_amps.push_back(1.0);

    // histogram simulation main loop    
    for(int hst_ID = 0; hst_ID < this->model.histogramList.size(); hst_ID++)
    {
        for(uint id = 0; id < this->model.histogram.getSize(); id++)
        {
            energyDistribution[id] = this->signal_amps.back() * this->model.histogramList[hst_ID].getAmp(id);
        }

        double energyLvl;
        int eBegin = this->model.histogramList[hst_ID].getFirstEcho();
        int eEnd = this->model.histogramList[hst_ID].getLastEcho();
        for(uint echo = eBegin; echo < eEnd; echo++)
        {
            // apply penalties
            for(uint id = 0; id < this->model.histogram.getSize(); id++)
            {
                energyDistribution[id] *= this->penalties[id];
            }

            // get global energy
            energyLvl = 0.0;
            for(uint id = 0; id < this->model.histogram.getSize(); id++)
            {
                energyLvl += energyDistribution[id];
            }

            // add to global energy vector
            this->signal_amps.push_back(energyLvl);
        }
    }

    delete[] energyDistribution;
    energyDistribution = NULL;

    double elapsedTime = omp_get_wtime() - beginTime;
    cout << "Done.\nCpu elapsed time: " << elapsedTime << " s" << endl;
}

// apply bulk relaxation to NMR signal
void NMR_cpmg::applyBulk()
{
    cout << "applying bulk relaxation." << endl;
    double bulkTime = -1.0 / this->model.getBulkRelaxationTime();

    if(this->signal_amps.size() == this->signal_times.size())
    {
        for(uint echo = 0; echo < this->signal_amps.size(); echo++)
        {
            this->signal_amps[echo] = exp(bulkTime * this->signal_times[echo]) * this->signal_amps[echo]; 
        }
    } 
    cout << "Ok." << endl;
}

void NMR_cpmg::resetSignal() 
{
    if (this->signal_amps.size() > 0)
    {
        this->signal_amps.clear();
    }
}

void NMR_cpmg::normalizeSignal()
{
    // check if energy decay was done
    if(this->signal_amps.size() == 0) 
    {
        cout << "no data available, could not apply normalization." << endl;
        return; 
    } 

    // normalize global energy signal
    double normalizer = 1.0 / this->signal_amps[0];
    for(uint echo = 0; echo < this->signal_amps.size(); echo++)
    {
        this->signal_amps[echo] = normalizer * this->signal_amps[echo]; 
    } 
}

// apply laplace inversion explicitly
void NMR_cpmg::applyLaplace()
{   
    cout << "Recovering T2 distribution [Tikhonov-ILT]...";
    double tick = omp_get_wtime();

    // check if energy decay was done
    if(this->signal_amps.size() == 0) 
    {
        cout << "no data available, could not apply inversion." << endl;
        return; 
    }

    // reset T2 distribution from previous simulation
    if(this->T2_bins.size() > 0) this->T2_bins.clear();
    if(this->T2_amps.size() > 0) this->T2_amps.clear();

    // get copy of decay info and remove first elements
    vector<double> decay = (*this).getSignalAmps();
    vector<double> times = this->getSignalTimes();
    times.erase(times.begin());
    decay.erase(decay.begin());     

    NMRInverterConfig nmr_inv_config(this->CPMG_config.getMinT2(), 
                                     this->CPMG_config.getMaxT2(),
                                     this->CPMG_config.getUseT2Logspace(),
                                     this->CPMG_config.getNumT2Bins(),
                                     this->CPMG_config.getMinLambda(),
                                     this->CPMG_config.getMaxLambda(),
                                     this->CPMG_config.getNumLambdas(),
                                     this->CPMG_config.getPruneNum(),
                                     this->CPMG_config.getNoiseAmp());

    NMRInverter nmr_inverter;
    nmr_inverter.set_config(nmr_inv_config, times);
    nmr_inverter.find_best_lambda(decay.size(), decay.data());
    nmr_inverter.invert(decay.size(), decay.data());
    for(uint i = 0; i < nmr_inverter.used_t2_bins.size(); i++)
    {
        this->T2_bins.push_back(nmr_inverter.used_t2_bins[i]);
        this->T2_amps.push_back(nmr_inverter.used_t2_amps[i]);
    }


    // Get noise vector
    vector<double> rawNoise = nmr_inverter.get_raw_noise();
    vector<double> newNoise;
    newNoise.reserve(this->signal_amps.size());
    newNoise.push_back(0.0);
    if(rawNoise.size() == (this->signal_amps.size() - 1))
    {
        for(int idx = 1; idx < this->signal_amps.size(); idx++)
        {
            newNoise.push_back(rawNoise[idx-1]);
        }
    } else
    {
        for(int idx = 1; idx < this->signal_amps.size(); idx++)
        {
            newNoise.push_back(0.0);
        }
    }
    (*this).setNoise(newNoise);

    double time = omp_get_wtime() - tick;
    cout << "Done in " << time << " secs." << endl;
    cout << "filtering lambda: " << nmr_inverter.get_inversion_lambda() << endl;
}

// -- Savings
void NMR_cpmg::save()
{
	double time = omp_get_wtime();
    cout << "saving results...";
       
    if(this->CPMG_config.getSaveWalkers())
    {
        (*this).writeWalkers();
    }

    if(this->CPMG_config.getSaveHistogram())
    {
        (*this).writeHistogram();
    }    

    if(this->CPMG_config.getSaveHistogramList())
    {
        (*this).writeHistogramList();
    }

    // write cpmg data
    if(this->CPMG_config.getSaveDecay()) 
    {
        (*this).writeT2decay();
        (*this).writeT2dist();
    }

	time = omp_get_wtime() - time;
    cout << "Ok. (" << time << " seconds)." << endl; 
}

void NMR_cpmg::writeT2decay()
{
    string filename = this->dir + "/cpmg_decay.csv";

    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const size_t num_points = this->signal_amps.size();
    const int precision = std::numeric_limits<double>::max_digits10;

    file << "time,signal,noise,noiseless" << endl;
    for (int idx = 0; idx < num_points; idx++)
    {
        file << setprecision(precision) << this->signal_times[idx] << ",";
        file << setprecision(precision) << this->signal_amps[idx] + this->noise[idx] << ",";
        file << setprecision(precision) << this->noise[idx] << ",";
        file << setprecision(precision) << this->signal_amps[idx] << endl;    
    }
    
    file.close();
}

void NMR_cpmg::writeT2dist()
{
    string filename = this->dir + "/cpmg_T2.csv";

    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const size_t num_points = T2_bins.size();
    const int precision = std::numeric_limits<double>::max_digits10;

    file << "NMRT2_bins,NMRT2_amps" << endl;
    for (int idx = 0; idx < num_points; idx++)
    {
        file << setprecision(precision) << this->T2_bins[idx] << "," << this->T2_amps[idx] << endl;
    }
    
    file.close();
}

void NMR_cpmg::writeWalkers()
{
    string filename = this->dir + "/cpmg_walkers.csv";
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

void NMR_cpmg::writeHistogram()
{
    string filename = this->dir + "/cpmg_histogram.csv";
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
        << this->model.histogram.getBin(i) 
        << "," << this->model.histogram.getAmp(i) << endl;
    }

    file.close();
}

void NMR_cpmg::writeHistogramList()
{
    string filename = this->dir + "/cpmg_histList.csv";
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
            file << setprecision(precision) << this->model.histogramList[hIdx].getBin(i) << ",";
            file << setprecision(precision) << this->model.histogramList[hIdx].getAmp(i) << ",";
        }

        file << endl;
    }

    file.close();
}