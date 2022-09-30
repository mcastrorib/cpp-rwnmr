#include "NMR_pfgse.h"

std::mt19937 NMR_PFGSE::_rng;

NMR_PFGSE::NMR_PFGSE(Model &_model,  
				     pfgse_config _pfgseConfig,
					 int _mpi_rank,
					 int _mpi_processes) : model(_model),
										   PFGSE_config(_pfgseConfig),
										   Dsat(0.0),
										   DsatError(0.0),
										   DsatStdev(0.0),
										   Dmsd(0.0),
										   DmsdStdev(0.0),
										   msd(0.0),
										   msdStdev(0.0),
										   vecMsd(0.0, 0.0, 0.0),
										   vecMsdStdev(0.0, 0.0, 0.0),
										   vecDmsd(0.0, 0.0, 0.0),
										   vecDmsdStdev(0.0, 0.0, 0.0),
										   stepsTaken(0),
										   currentTime(0),
										   DsatAdjustSamples(0),
										   mpi_rank(_mpi_rank),
										   mpi_processes(_mpi_processes)
{
	// Initialize random state
	NMR_PFGSE::_rng.seed(this->model.getInitialSeed());
	NMR_PFGSE::_rng.discard(4096);

	// vectors object init
	vector<double> exposureTimes();
	vector<double> gradient();
	vector<double> rawNoise();
	vector<double> rhs();
	vector<double> mkt();
	vector<double> mktStdev();
	vector<double> lhs();
	vector<double> lhs_stdev();
	vector<Vector3D> vecGradient();
	vector<Vector3D> vecK();

	// read config file
	Vector3D gradientMax = this->PFGSE_config.getMaxGradient();
	this->gradientX = gradientMax.getX();
	this->gradientY = gradientMax.getY();
	this->gradientZ = gradientMax.getZ();	
	this->gradientPoints = this->PFGSE_config.getGradientSamples();
	this->exposureTimes = this->PFGSE_config.getTimeValues(); 
	this->pulseWidth = this->PFGSE_config.getPulseWidth();
	(*this).setApplyBulkRelaxation(this->PFGSE_config.getApplyBulk());
	(*this).setNoiseAmp(this->PFGSE_config.getNoiseAmp());
	(*this).setTargetSNR(this->PFGSE_config.getTargetSNR());
	(*this).buildThresholdFromSamples(this->gradientPoints);
	(*this).buildGradientVector();
	(*this).buildVectorK();

	// new
	(*this).setName();
	(*this).createDirectoryForData();
}

void NMR_PFGSE::set()
{
	// (*this).setName();
	// (*this).createDirectoryForData();
	(*this).buildModelTimeFramework();
	(*this).buildVectorMkt();
	(*this).buildVectorLHS();
	(*this).buildVectorRHS();
}

void NMR_PFGSE::run()
{
	// before everything, reset conditions and map with highest time value
	double tick = omp_get_wtime();
	cout << endl << "-- Pre-processing:" << endl;
	(*this).resetCurrentTime();
	(*this).correctExposureTimes();
	(*this).runInitialMapSimulation();
	(*this).resetModel();
	(*this).presave();
	cout << "-- Done in " << omp_get_wtime() - tick << " seconds." << endl;

	double interiorTick;
	for(uint timeSample = 0; timeSample < this->exposureTimes.size(); timeSample++)
	{
		interiorTick = omp_get_wtime();
		(*this).setExposureTime((*this).getExposureTime(timeSample));
		(*this).set();
		(*this).runSequence();

		// D(t) extraction
		cout << "-- Results:" << endl;
		(*this).recoverDsat();
		(*this).recoverDmsd();
		
		// save results in disc
		(*this).save(); 
		(*this).incrementCurrentTime();
		cout << "-- Done in " << omp_get_wtime() - interiorTick << " seconds." << endl << endl;
	}

	double time = omp_get_wtime() - tick;
	cout << endl << "pfgse_time: " << time << " seconds." << endl;
}

void NMR_PFGSE::applyThreshold()
{
	// apply threshold for D(t) extraction
	string threshold_type = this->PFGSE_config.getThresholdType();
	double threshold_value = this->PFGSE_config.getThresholdValue();
	uint threshold_window = this->PFGSE_config.getThresholdWindow();
	if(threshold_window > (*this).getGradientPoints()) threshold_window = (*this).getGradientPoints();
	if(threshold_type == "lhs") (*this).buildThresholdFromLHS(threshold_value, threshold_window);
	else if(threshold_type == "samples") (*this).buildThresholdFromSamples(int(threshold_value));
	else (*this).buildThresholdFromSamples(this->gradientPoints);
}

void NMR_PFGSE::resetModel()
{
	// reset walker's initial state with omp parallel for
	cout << "- Reseting walker initial state" << endl;

    if(this->model.getRWNMRConfig().getOpenMPUsage())
    {
        // set omp variables for parallel loop throughout walker list
        const int num_cpu_threads = omp_get_max_threads();
        const int loop_size = this->model.walkers.size();
        int loop_start, loop_finish;

        #pragma omp parallel private(loop_start, loop_finish) 
        {
            const int thread_id = omp_get_thread_num();
            OMPLoopEnabler looper(thread_id, num_cpu_threads, loop_size);
            loop_start = looper.getStart();
            loop_finish = looper.getFinish(); 

            for (uint id = loop_start; id < loop_finish; id++)
            {
                this->model.walkers[id].resetPosition();
                this->model.walkers[id].resetSeed();
                this->model.walkers[id].resetEnergy();
                this->model.walkers[id].resetCollisions();
            }
        }
    } else
    {
        for (uint id = 0; id < this->model.walkers.size(); id++)
        {
            this->model.walkers[id].resetPosition();
            this->model.walkers[id].resetSeed();
            this->model.walkers[id].resetEnergy();
            this->model.walkers[id].resetCollisions();
        }
    }   
}

void NMR_PFGSE::updateWalkersXiRate(uint _rwsteps)
{
	// update walker's xirate with omp parallel for

    if(this->model.getRWNMRConfig().getOpenMPUsage())
    {
        // set omp variables for parallel loop throughout walker list
        const int num_cpu_threads = omp_get_max_threads();
        const int loop_size = this->model.walkers.size();
        int loop_start, loop_finish;

        #pragma omp parallel private(loop_start, loop_finish) 
        {
            const int thread_id = omp_get_thread_num();
            OMPLoopEnabler looper(thread_id, num_cpu_threads, loop_size);
            loop_start = looper.getStart();
            loop_finish = looper.getFinish(); 

            for (uint id = loop_start; id < loop_finish; id++)
            {
                this->model.walkers[id].updateXiRate(_rwsteps);
            }
        }
    } else
    {
        for (uint id = 0; id < this->model.walkers.size(); id++)
        {
            this->model.walkers[id].updateXiRate(_rwsteps);
        }
    }   
}

void NMR_PFGSE::correctExposureTimes()
{
	// apply 'physical' scaling
    if(this->PFGSE_config.getApplyScaleFactor())
    {
        double scale_factor = (this->PFGSE_config.getInspectionLength() * this->PFGSE_config.getInspectionLength()) / this->model.getDiffusionCoefficient();
        cout << "applying scale factor: " << scale_factor << endl;
        for(int time = 0; time < this->exposureTimes.size(); time++)
        	this->exposureTimes[time] *= scale_factor;
    }
	
	cout << "- Correcting time samples to rw parameters" << endl;
	double timePerStep = this->model.getTimeInterval();
	double stepsPerEcho = (double) this->model.getStepsPerEcho();
	uint stepsPerExpTime;
	for(int time = 0; time < this->exposureTimes.size(); time++)
	{
		stepsPerExpTime = this->exposureTimes[time] / timePerStep;
		if(stepsPerExpTime < 1) stepsPerExpTime = 1;
		if(stepsPerExpTime % (uint) stepsPerEcho != 0) 
		{
			stepsPerExpTime += stepsPerExpTime % (uint) stepsPerEcho;
		}
	}
}

void NMR_PFGSE::setName()
{
	this->name = "/NMR_pfgse";
}

void NMR_PFGSE::createDirectoryForData()
{
	string path = this->model.getDbPath() + this->model.getName();
    BaseFunctions::createDirectory(path, this->name);
    this->dir = (path + "/" + this->name);
    BaseFunctions::createDirectory(this->dir, "/timesamples");
    
}

void NMR_PFGSE::buildGradientVector(double _GF, int _GPoints)
{
	this->gradientMax = _GF;
	this->gradientPoints = _GPoints;
	(*this).buildGradientVector();
}

void NMR_PFGSE::buildVectorMkt()
{
	if(this->mkt.size() > 0) this->mkt.clear();
	this->mkt.reserve(this->gradientPoints);

	if(this->mktStdev.size() > 0) this->mktStdev.clear();
	this->mktStdev.reserve(this->gradientPoints);
}

void NMR_PFGSE::buildGradientVector()
{
	if(this->vecGradient.size() > 0) this->vecGradient.clear();
	this->vecGradient.reserve(this->gradientPoints);

	if(this->gradient.size() > 0) this->gradient.clear();
	this->gradient.reserve(this->gradientPoints);
	
	double gapX = (this->gradientX) / ((double) (this->gradientPoints - 1));
	double gapY = (this->gradientY) / ((double) (this->gradientPoints - 1));
	double gapZ = (this->gradientZ) / ((double) (this->gradientPoints - 1));
	
	double gvalueX = 0.0;
	double gvalueY = 0.0;
	double gvalueZ = 0.0;

	for(uint index = 0; index < this->gradientPoints; index++)
	{
		Vector3D newGradient(gvalueX, gvalueY, gvalueZ);
		this->vecGradient.push_back(newGradient);
		this->gradient.push_back(newGradient.getNorm());
		gvalueX += gapX;
		gvalueY += gapY;
		gvalueZ += gapZ;
	}
}

void NMR_PFGSE::buildVectorK()
{
	if(this->vecK.size() > 0) this->vecK.clear();
	this->vecK.reserve(this->gradientPoints);

	double Kx, Ky, Kz;
	for(uint index = 0; index < this->gradientPoints; index++)
	{
		Kx = (*this).computeWaveVectorK(this->vecGradient[index].getX(), (*this).getPulseWidth(), (*this).getModel().getGiromagneticRatio());
		Ky = (*this).computeWaveVectorK(this->vecGradient[index].getY(), (*this).getPulseWidth(), (*this).getModel().getGiromagneticRatio());
		Kz = (*this).computeWaveVectorK(this->vecGradient[index].getZ(), (*this).getPulseWidth(), (*this).getModel().getGiromagneticRatio());
		Vector3D Knew(Kx, Ky, Kz);
		this->vecK.push_back(Knew);
	}
}

void NMR_PFGSE::buildModelTimeFramework()
{
	cout << endl << "-- Exposure time: " << this->exposureTime << " ms";
	this->model.buildTimeFramework(this->exposureTime);
	cout << " [" << this->model.getSimulationSteps() << " RW-steps]" << endl;
}

void NMR_PFGSE::runInitialMapSimulation()
{
	if(this->exposureTimes.size() > 0)
	{	
		double longestTime = (*this).getExposureTime(this->exposureTimes.size() - 1);
		uint mapSteps = 40000; // ??
		bool mapByTime = true;
		if(mapByTime) this->model.buildTimeFramework(longestTime);
		else this->model.buildTimeFramework(mapSteps);
		
		cout << "- Initial map time: ";
		if(mapByTime) cout << longestTime << " ms ";
		cout << "[" << this->model.getSimulationSteps() << " RW-steps]" << endl;
		this->model.mapSimulation();

		// Update xi_rate and relaxivity of walkers
		vector<double> rho = this->model.getRWNMRConfig().getRho();
		if(this->model.getRWNMRConfig().getRhoType() == "uniform")    
        {
        	this->model.updateWalkersRelaxativity(rho[0]);
    	} 
    	else if(this->model.getRWNMRConfig().getRhoType() == "sigmoid")
        {
        	this->model.updateWalkersRelaxativity(rho);
        }
	}
}

void NMR_PFGSE::buildVectorRHS()
{
	if(this->rhs.size() > 0) this->rhs.clear();
	this->rhs.reserve(this->gradientPoints);

	for(uint idx = 0; idx < this->gradientPoints; idx++)
	{
		double vrhs = (*this).computeRHS(this->vecK[idx].getNorm());
		// double rhs = (*this).computeRHS_legacy(this->gradient[idx]);
		this->rhs.push_back(vrhs);
	}
}

void NMR_PFGSE::buildThresholdFromLHS(double _value, uint _window)
{

	if(this->lhs.size() < _window) 
		return;

	if(_value > 0.0 && _value < 1.0)
	{
		if((*this).getNoiseAmp() == 0.0)
		{
			(*this).buildThresholdFromLHSValue(_value, _window);
		} 
		else
		{
			(*this).buildThresholdFromLHSWindow(_value, _window);
		}
	}
}

void NMR_PFGSE::buildThresholdFromLHSValue(double _value, uint _window)
{
	uint minSize = _window;
	int idx = minSize - 1;
	bool isGreater = true;
	double logValue = log(_value);

	while(idx < this->lhs.size() && isGreater == true)
	{
		if(this->lhs[idx] < logValue)
		{
			isGreater = false;
		}
		else
		{
			idx++;
		}
	}

	if(isGreater) idx--;
	this->DsatAdjustSamples = idx;
}

void NMR_PFGSE::buildThresholdFromLHSWindow(double _value, uint _window)
{
	if(this->lhs.size() < _window) 
		return;

	vector<double> windowValues;
	for(uint idx = 0; idx < _window; idx++) 
		windowValues.push_back(this->lhs[idx]);
	int idx = _window;
	bool isGreater = true;
	double logValue = log(_value);

	if((*this).mean(windowValues) < logValue)
	{
		isGreater = false;
	}
	
	while(idx < this->lhs.size() && isGreater == true)
	{
		uint currentIdx = idx % _window;
		windowValues[currentIdx] = this->lhs[idx];

		if((*this).mean(windowValues) < logValue)
		{
			isGreater = false;
		}
		else
		{
			idx++;
		}
	}

	if(isGreater) idx--;
	this->DsatAdjustSamples = idx;
}

void NMR_PFGSE::buildThresholdFromSamples(int _samples)
{
	if(_samples <= this->gradientPoints && _samples > 1)	
	{
		this->DsatAdjustSamples = _samples;
	}
}


double NMR_PFGSE::computeRHS(double _kValue)
{
	return (-1.0) * _kValue * _kValue * (this->exposureTime - ((this->pulseWidth) / 3.0));  
}

double NMR_PFGSE::computeRHS_legacy(double _Gvalue)
{
	double gamma = (*this).getModel().getGiromagneticRatio();
	// if(this->PFGSE_config.getUseWaveVectorTwoPi()) gamma *= TWO_PI;
	
	return (-1.0e-10) * (gamma * this->pulseWidth) * (gamma * this->pulseWidth) 
			* (this->exposureTime - ((this->pulseWidth) / 3.0)) 
			* _Gvalue *  _Gvalue ;  
}

void NMR_PFGSE::buildVectorLHS()
{
	if(this->lhs.size() > 0) this->lhs.clear();
	this->lhs.reserve(this->gradientPoints);

	if(this->lhsStdev.size() > 0) this->lhsStdev.clear();
	this->lhsStdev.reserve(this->gradientPoints);
}

double NMR_PFGSE::computeLHS(double _Mg, double _M0)
{
	double threshold = 1.0e-06;
	double ratio = _Mg/_M0;
	if(ratio > threshold) return log(ratio);
	else return log(threshold);

}

double NMR_PFGSE::computeWaveVectorK(double gradientMagnitude, double pulse_width, double giromagneticRatio)
{
    return (pulse_width * 1.0e-03) *  (giromagneticRatio * 1.0e+06) * (gradientMagnitude * 1.0e-08);
}

void NMR_PFGSE::runSequence()
{
	// run pfgse experiment -- this method will fill mkt vector
	(*this).simulation();

	// apply bulk relaxation to raw signal
	if((*this).getApplyBulkRelaxation())
	{
		(*this).applyBulk();
	}

	// add white noise to raw signal
	(*this).createNoiseVector();
	if((*this).getNoiseAmp() > 0.0)
	{
		(*this).applyNoiseToSignal();
	}
}

void NMR_PFGSE::applyBulk()
{
	double bulkTime = -1.0 / this->model.getBulkRelaxationTime();
	double bulkMagnitude = exp(bulkTime * (*this).getExposureTime());
	
	// Apply bulk relaxation in simulated signal
	for(uint kIdx = 0; kIdx < this->mkt.size(); kIdx++)
	{
		this->mkt[kIdx] *= bulkMagnitude;
	}
}

vector<double> NMR_PFGSE::getNewNoiseVector(double _noiseAmp, uint _size)
{
	vector<double> newNoise;
	if(_size == 0) newNoise = getNormalDistributionSamples(0.0, _noiseAmp, (*this).getGradientPoints());
	else newNoise = getNormalDistributionSamples(0.0, _noiseAmp, _size);
	return newNoise;
}

void NMR_PFGSE::createNoiseVector()
{
	if(this->rawNoise.size() != (*this).getGradientPoints()) 
		this->rawNoise.clear();

	double noiseBasis;
	if((*this).getTargetSNR() > 0.0) 
	{
		(*this).setNoiseAmp((*this).computeTargetNoiseAmp()); 
		noiseBasis = 1.0;
	} else 
	{
		noiseBasis = (double) this->model.getNumberOfWalkers(); 
	}

	this->rawNoise = getNewNoiseVector(noiseBasis * (*this).getNoiseAmp());
	
	cout << "Noise:: Amp: " << (*this).getNoiseAmp();
	cout << ", SNR: " << (*this).computeCurrentSNR() << endl;
}

double NMR_PFGSE::computeTargetNoiseAmp()
{
	// sum of squared signal data
	double sss = 0.0;
	double uss = 0.0;
	for(uint idx = 0; idx < (*this).getGradientPoints(); idx++)
	{
		sss += this->mkt[idx] * this->mkt[idx];
		uss += this->mkt[idx];
	}
	sss /= (double) (*this).getGradientPoints();
	uss /= (double) (*this).getGradientPoints();

	return sqrt((sss - uss*uss)/(*this).getTargetSNR());	
}

double NMR_PFGSE::computeCurrentSNR()
{
	if(this->mkt.size() != this->rawNoise.size() or this->rawNoise.size() == 0)
		return 0.0;

	// sum of squared signal/noise data
	double sss = 0.0;
	double ssn = 0.0;
	for(uint idx = 0; idx < (*this).getGradientPoints(); idx++)
	{
		sss += this->mkt[idx] * this->mkt[idx];
		ssn += this->rawNoise[idx] * this->rawNoise[idx];
	}

	return (sss/ssn);	
}

void NMR_PFGSE::applyNoiseToSignal()
{
	// Add noise to signal
	if((*this).getNoiseAmp() > 0.0 and this->mkt.size() == this->getGradientPoints())
	{
		for(uint kIdx = 0; kIdx < this->getGradientPoints(); kIdx++)
		{
			this->mkt[kIdx] += this->rawNoise[kIdx];		
		}			
	}
}

void NMR_PFGSE::simulation()
{
	if(this->model.getGpuUsage() == true)
	{
		(*this).simulation_cuda();
	}
	else
	{
		(*this).simulation_omp();
	}
}

void NMR_PFGSE::recoverDsat()
{
	cout << "- Stejskal-Tanner (s&t) ";
	double time = omp_get_wtime();

	if((this->model.getWalkerSamples() > 1) and this->PFGSE_config.getAllowWalkerSampling())
	{
		cout << "with sampling:" <<  endl;
		(*this).recoverDsatWithSampling();
	} else	
	{
		cout << "without sampling:" <<  endl;
		(*this).recoverDsatWithoutSampling();
	}

	cout << "in " << omp_get_wtime() - time << " seconds." << endl;
}

void NMR_PFGSE::recoverDmsd()
{
	cout << "- Mean squared displacement (msd) ";
	double time = omp_get_wtime();

	if(this->PFGSE_config.getAllowWalkerSampling())
	{
		cout << "with sampling:" <<  endl;
		(*this).recoverDmsdWithSampling();
	} else	
	{
		cout << "without sampling:" <<  endl;
		(*this).recoverDmsdWithoutSampling();
	}

	cout << "in " << omp_get_wtime() - time << " seconds." << endl;
}

void NMR_PFGSE::recoverDsatWithoutSampling()
{
	// Get magnetization levels 
	// get M0 (reference value)
	int idx_begin = 0;
	int idx_end = this->gradientPoints;

	// Normalize for k=0
	double M0 = this->mkt[0];
	vector<double> normMkt;
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		normMkt.push_back(this->mkt[kIdx] / M0);
	}
	

	for(uint point = idx_begin; point < idx_end; point++)
	{	
		// this->LHS.push_back((*this).computeLHS(this->Mkt[point], this->Mkt[0]));
		this->lhs.push_back((*this).computeLHS(normMkt[point], normMkt[0]));
	}

	// fill standard deviation vectors with null values
	for(uint point = 0; point < this->gradientPoints; point++)
	{
		this->mktStdev.push_back(0.0);
		this->lhsStdev.push_back(0.0);
	}

	(*this).applyThreshold();
	cout << "points to sample: " << this->DsatAdjustSamples << " where lhs: " << exp(this->lhs[this->DsatAdjustSamples-1]) << endl;
	vector<double> RHS_buffer; RHS_buffer.reserve(this->DsatAdjustSamples);
	vector<double> LHS_buffer; LHS_buffer.reserve(this->DsatAdjustSamples);
	// fill RHS data buffer only once
	for(int point = 0; point < this->DsatAdjustSamples; point++)
	{
		RHS_buffer.push_back(this->rhs[point]);
		LHS_buffer.push_back(this->lhs[point]);
	}
	bool intercept = false;
	LeastSquareAdjust lsa(RHS_buffer, LHS_buffer, intercept);
	lsa.setPoints(this->DsatAdjustSamples);
	lsa.solve();
	
	(*this).setDsat(lsa.getB());
	double DstdError = sqrt(lsa.getMSE() * (((double) this->DsatAdjustSamples) /((double) this->DsatAdjustSamples - 1.0)));
	(*this).setDsatError(DstdError);

	// log results
	cout << "D(" << (*this).getExposureTime((*this).getCurrentTime()) << " ms) {s&t} = " << (*this).getDsat();
	cout << "[+/- " << 1.96 * (*this).getDsatError() << "]" << endl;	
}

double ** NMR_PFGSE::getSamplesMagnitude()
{
	if(this->model.getGpuUsage() == true)
	{
		return (*this).computeSamplesMagnitudeWithGpu();
	}
	else
	{
		if(this->model.getRWNMRConfig().getOpenMPUsage())
		{
			return (*this).computeSamplesMagnitudeWithOmp();
		} else
		{	
			return (*this).computeSamplesMagnitude();
		}
	}
}

double ** NMR_PFGSE::computeSamplesMagnitudeWithOmp()
{
	/* 
		alloc table for mkt data each row will represent a wavevector K value, 
		while each column represent a sample of random walkers
	*/
	double **Mkt_samples;
	Mkt_samples = new double*[this->gradientPoints];
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		Mkt_samples[kIdx] = new double[this->model.getWalkerSamples()];
	}

	/*
		initialize each element in table with zeros
	*/
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
		{
			Mkt_samples[kIdx][sample] = 0.0;
		}
	}	

	double resolution = this->model.getImageVoxelResolution();
	double phase;
	double dX, dY, dZ;

    // set omp variables for parallel loop throughout walker list
    const int walkersPerSample = this->model.getNumberOfWalkers() / this->model.getWalkerSamples();	
	const int num_cpu_threads = omp_get_max_threads();
    int loop_start, loop_finish;
	const int loop_size = this->model.getWalkerSamples();	

    /*
		collect sum of data from phaseMagnitudes table
	*/
	#pragma omp parallel shared(Mkt_samples, resolution) private(loop_start, loop_finish, phase, dX, dY, dZ) 
    {
        const int thread_id = omp_get_thread_num();
        OMPLoopEnabler looper(thread_id, num_cpu_threads, loop_size);
        loop_start = looper.getStart();
        loop_finish = looper.getFinish(); 

        cout << "thread " << thread_id << " here with omp ^^" << endl;

        for (uint sampleId = loop_start; sampleId < loop_finish; sampleId++)
        {			
			int offset = sampleId * walkersPerSample;
			for(uint idx = 0; idx < walkersPerSample; idx++)
	        {
	            // Get walker displacement
				dX = ((double) this->model.walkers[offset + idx].getInitialPositionX() - (double) this->model.walkers[offset + idx].getCurrentPositionX());
				dY = ((double) this->model.walkers[offset + idx].getInitialPositionY() - (double) this->model.walkers[offset + idx].getCurrentPositionY());
				dZ = ((double) this->model.walkers[offset + idx].getInitialPositionZ() - (double) this->model.walkers[offset + idx].getCurrentPositionZ());
				Vector3D dR(resolution * dX, resolution * dY, resolution * dZ);
					
				for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
				{
					phase = this->vecK[kIdx].dotProduct(dR);	
					Mkt_samples[kIdx][sampleId] += cos(phase) * this->model.walkers[idx].getEnergy();	
				}
	        }

		}
	}

	return Mkt_samples;
 }

double ** NMR_PFGSE::computeSamplesMagnitude()
{
	/* 
		alloc table for Mkt data each row will represent a wavevector K value, 
		while each column represent a sample of random walkers
	*/
	double **Mkt_samples;
	Mkt_samples = new double*[this->gradientPoints];
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		Mkt_samples[kIdx] = new double[this->model.getWalkerSamples()];
	}

	/*
		initialize each element in table with zeros
	*/
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
		{
			Mkt_samples[kIdx][sample] = 0.0;
		}
	}	

	double resolution = this->model.getImageVoxelResolution();
	double phase;
	double dX, dY, dZ;

	int walkersPerSample = this->model.getNumberOfWalkers() / this->model.getWalkerSamples();	
	for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
	{		
		int offset = sample * walkersPerSample;	
		for(uint idx = 0; idx < walkersPerSample; idx++)
		{
			// Get walker displacement
			dX = ((double) this->model.walkers[offset + idx].getInitialPositionX() - (double) this->model.walkers[offset + idx].getCurrentPositionX());
			dY = ((double) this->model.walkers[offset + idx].getInitialPositionY() - (double) this->model.walkers[offset + idx].getCurrentPositionY());
			dZ = ((double) this->model.walkers[offset + idx].getInitialPositionZ() - (double) this->model.walkers[offset + idx].getCurrentPositionZ());
			Vector3D dR(resolution * dX, resolution * dY, resolution * dZ);
			
			for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
			{
				phase = this->vecK[kIdx].dotProduct(dR);
				Mkt_samples[kIdx][sample] += cos(phase) * this->model.walkers[idx].getEnergy();;	
			}
		}		 
	}

	return Mkt_samples;
}

double ** NMR_PFGSE::computeSamplesNoise()
{
	/* 
		alloc table for Mkt data each row will represent a wavevector K value, 
		while each column represent a sample of random walkers
	*/
	double **Mkt_noise;
	Mkt_noise = new double*[(*this).getGradientPoints()];
	for(uint kIdx = 0; kIdx < (*this).getGradientPoints(); kIdx++)
	{
		Mkt_noise[kIdx] = new double[this->model.getWalkerSamples()];
	}

	/*
		initialize each element in table with zeros
	*/
	for(uint kIdx = 0; kIdx < this->getGradientPoints(); kIdx++)
	{
		for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
		{
			Mkt_noise[kIdx][sample] = 0.0;
		}
	}	

	if((*this).getNoiseAmp() > 0.0)
	{
		// this factor is applied beacuse of the decreased magnetization Mkt
		double M0 = (double) this->model.getNumberOfWalkers() / (double) this->model.getWalkerSamples();
		
		
		for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
		{
			vector<double> noise = (*this).getNewNoiseVector(M0 * (*this).getNoiseAmp());
			for(uint kIdx = 0; kIdx < this->getGradientPoints(); kIdx++)
			{
				Mkt_noise[kIdx][sample] = noise[kIdx];
			}
		}	
	}

	return Mkt_noise;	
}

void NMR_PFGSE::recoverDsatWithSampling()
{
	bool time_verbose = false;
	int walkersPerSample = this->model.getNumberOfWalkers() / this->model.getWalkerSamples();	
	double tick, phaseTime, normTime, lhsTime, statTime, lsTime;	

	/* 
		Compute magnitude Mkt for each sample of walkers
	*/
	tick = omp_get_wtime();
	double **Mkt_samples, **Mkt_noise;
	Mkt_samples = (*this).getSamplesMagnitude();	
	Mkt_noise = (*this).computeSamplesNoise();
	phaseTime = omp_get_wtime() - tick;


	tick = omp_get_wtime();

	// Apply bulk relaxation in simulated signal
	if((*this).getApplyBulkRelaxation())
	{
		double bulkTime = -1.0 / this->model.getBulkRelaxationTime();
		double bulkMagnitude = exp(bulkTime * (*this).getExposureTime());
		for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
		{
			for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
			{
				Mkt_samples[kIdx][sample] *= bulkMagnitude;
			}
		}
	}

	// Add noise to signal
	if((*this).getNoiseAmp() > 0.0)
	{
		for(uint sample = 0; sample < this->model.getWalkerSamples(); sample++)
		{
			for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
			{
				Mkt_samples[kIdx][sample] += Mkt_noise[kIdx][sample];
			}
		}	
	}

	// Normalize for k=0
	/* 
		alloc table for normalized Mkt data
		each row will represent a wavevector K value, 
		while each column represent a sample of random walkers
	*/
	double **nMkt_samples;
	nMkt_samples = new double*[this->gradientPoints];
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		nMkt_samples[kIdx] = new double[this->model.getWalkerSamples()];
	}

	for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
	{
		double M0 = Mkt_samples[0][sample];
		for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
		{
			nMkt_samples[kIdx][sample] = Mkt_samples[kIdx][sample] / M0;
		}
	}	
	normTime = omp_get_wtime() - tick;

	/* 
		alloc table for LHS data each row will represent a wavevector K value, 
		while each column represent a sample of random walkers
	*/
	double **LHS_samples;
	LHS_samples = new double*[this->gradientPoints];
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		LHS_samples[kIdx] = new double[this->model.getWalkerSamples()];
	} 	
	
	/*
		compute lhs for each sample
	*/
	tick = omp_get_wtime();
	double lhs_value;
	for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
	{
		for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
		{	
			LHS_samples[kIdx][sample] = (*this).computeLHS(nMkt_samples[kIdx][sample], nMkt_samples[0][sample]);
		}
	}
	lhsTime = omp_get_wtime() - tick;

	/*
		 get data statistics 
	*/
	tick = omp_get_wtime();
	vector<double> meanMkt; meanMkt.reserve(this->gradientPoints);
	vector<double> stDevMkt; stDevMkt.reserve(this->gradientPoints);
	vector<double> meanNoise; meanNoise.reserve(this->gradientPoints);
	vector<double> meanLHS; meanLHS.reserve(this->gradientPoints);
	vector<double> stDevLHS; stDevLHS.reserve(this->gradientPoints);
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		meanMkt.push_back((*this).mean(Mkt_samples[kIdx], this->model.getWalkerSamples()));
		stDevMkt.push_back((*this).stdDev(Mkt_samples[kIdx], this->model.getWalkerSamples(), meanMkt[kIdx]));
		meanNoise.push_back((*this).mean(Mkt_noise[kIdx], this->model.getWalkerSamples()));
		meanLHS.push_back((*this).mean(LHS_samples[kIdx], this->model.getWalkerSamples()));
		stDevLHS.push_back((*this).stdDev(LHS_samples[kIdx], this->model.getWalkerSamples(), meanLHS[kIdx]));
	}
	statTime = omp_get_wtime() - tick;

	
	// copy data to class members
	this->mkt = meanMkt;
	this->mktStdev = stDevMkt;
	this->rawNoise = meanNoise;
	this->lhs = meanLHS;
	this->lhsStdev = stDevLHS;

	/*
		Stejskal-Tanner (s&t)
	*/
	tick = omp_get_wtime();
	vector<double> Dsat; Dsat.reserve(this->model.getWalkerSamples());
	vector<double> DsatError; DsatError.reserve(this->model.getWalkerSamples());
	double DstdError;
	(*this).applyThreshold();
	cout << "points to sample: " << this->DsatAdjustSamples << endl;

	vector<double> RHS_buffer; RHS_buffer.reserve(this->DsatAdjustSamples);
	vector<double> LHS_buffer; LHS_buffer.reserve(this->DsatAdjustSamples);
	// fill RHS data buffer only once
	for(int point = 0; point < this->DsatAdjustSamples; point++)
	{
		RHS_buffer.push_back(this->rhs[point]);
	}

	for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
	{
		// fill LHS data buffer for each sample
		if(LHS_buffer.size() > 0)
		{
			LHS_buffer.clear();
		}
		for(int point = 0; point < this->DsatAdjustSamples; point++)
		{
			LHS_buffer.push_back(LHS_samples[point][sample]);
		}

		bool intercept = false;	
		LeastSquareAdjust lsa(RHS_buffer, LHS_buffer, intercept);
		lsa.setPoints(this->DsatAdjustSamples);
		lsa.solve();
		Dsat.push_back(lsa.getB());
		DstdError = sqrt(lsa.getMSE() * (((double) this->DsatAdjustSamples) /((double) this->DsatAdjustSamples - 1.0)));
		DsatError.push_back(DstdError);		
	}
	lsTime = omp_get_wtime() - tick;	

	// 
	double meanDsat = (*this).mean(Dsat);
	double meanDsatError = (*this).mean(DsatError);
	(*this).setDsat(meanDsat);
	(*this).setDsatError(meanDsatError);
	(*this).setDsatStdev(((*this).stdDev(Dsat, meanDsat)));

	// log results	
	cout << "D(" << (*this).getExposureTime((*this).getCurrentTime()) << " ms) {s&t} = " << (*this).getDsat();
	cout << " +/- " << (*this).getDsatStdev();
	cout << " [+/- " << 1.96 * (*this).getDsatError() << "]" << endl;
	
	if(time_verbose)
    {
        cout << "--- Time analysis ---" << endl;
        cout << "phase computation: " << phaseTime << " s" << endl;
        cout << "data normalization: " << normTime << " s" << endl;
        cout << "LHS computation: " << lhsTime << " s" << endl;
        cout << "stats computation: " << statTime << " s" << endl;
        cout << "least-squares computation: " << lsTime << " s" << endl;
        cout << "---------------------" << endl;
    }

    /*
		delete data from pointers
    */
	// free data for Mkt_samples
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		delete [] Mkt_samples[kIdx];
		Mkt_samples[kIdx] = NULL;
	}
	delete [] Mkt_samples;
	Mkt_samples = NULL;

	// free data for nMkt_samples
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		delete [] nMkt_samples[kIdx];
		nMkt_samples[kIdx] = NULL;
	}
	delete [] nMkt_samples;
	nMkt_samples = NULL;

	// free data for Mkt_noise
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		delete [] Mkt_noise[kIdx];
		Mkt_noise[kIdx] = NULL;
	}
	delete [] Mkt_noise;
	Mkt_noise = NULL; 

	// free data for LHS_samples
	for(uint kIdx = 0; kIdx < this->gradientPoints; kIdx++)
	{
		delete [] LHS_samples[kIdx];
		LHS_samples[kIdx] = NULL;
	}
	delete [] LHS_samples;
	LHS_samples = NULL; 
}

void NMR_PFGSE::recoverDmsdWithoutSampling()
{
	double squaredDisplacement = 0.0;
	double displacementX, displacementY, displacementZ;
	double X0, Y0, Z0;
	double XF, YF, ZF;
	double normalizedDisplacement;
	double nDx = 0.0; double nDy = 0.0; double nDz = 0.0;
	double resolution = this->model.getImageVoxelResolution();
	double aliveWalkerFraction = 0.0;
	
	// Relaxation / Absorption equivalence
	double absorptionFraction;
	double absorption = 0.0;
	double nonAbsorption = 1.0;
	if(this->PFGSE_config.getApplyAbsorption())
	{
		absorption = 1.0;
		nonAbsorption = 0.0;
	}

	// debug
	// int imgX, imgY, imgZ;
	for(uint idx = 0; idx < this->model.getNumberOfWalkers(); idx++)
	{
		Walker particle(this->model.walkers[idx]);

		// Get walker displacement
		// X:
		X0 = (double) particle.getInitialPositionX();
		XF = (double) particle.getCurrentPositionX();
		displacementX = resolution * (XF - X0);
		displacementX *= displacementX;
		
		// Y:
		Y0 = (double) particle.getInitialPositionY();
		YF = (double) particle.getCurrentPositionY();
		displacementY = resolution * (YF - Y0);
		displacementY *= displacementY;

		// Z:
		Z0 = (double) particle.getInitialPositionZ();
		ZF = (double) particle.getCurrentPositionZ();
		displacementZ = resolution * (ZF - Z0);
		displacementZ *= displacementZ;

		absorptionFraction = (absorption * particle.getEnergy() + nonAbsorption);
		aliveWalkerFraction += absorptionFraction;
		nDx += (absorptionFraction * displacementX);
		nDy += (absorptionFraction * displacementY);
		nDz += (absorptionFraction * displacementZ);
		normalizedDisplacement = displacementX + displacementY + displacementZ;
		squaredDisplacement += (absorptionFraction * normalizedDisplacement);
 	}

	// set diffusion coefficient (see eq 2.18 - ref. Bergman 1995)
	squaredDisplacement = squaredDisplacement / aliveWalkerFraction;
	(*this).setDmsd(squaredDisplacement/(6.0 * (*this).getExposureTime()));
	(*this).setMsd(squaredDisplacement);

	nDx /= aliveWalkerFraction;
	nDy /= aliveWalkerFraction;
	nDz /= aliveWalkerFraction;
	(*this).setVecMsd(nDx, nDy, nDz);
	(*this).setVecDmsd((nDx / (2.0 * (*this).getExposureTime())), 
					   (nDy / (2.0 * (*this).getExposureTime())), 
					   (nDz / (2.0 * (*this).getExposureTime()))); 

	
	cout << "D(" << (*this).getExposureTime((*this).getCurrentTime()) << " ms) {msd} = " << (*this).getDmsd() << endl;
	cout << "Dxx = " << this->vecDmsd.getX() << ", \t";
	cout << "Dyy = " << this->vecDmsd.getY() << ", \t";
	cout << "Dzz = " << this->vecDmsd.getZ() << endl;
}

void NMR_PFGSE::recoverDmsdWithSampling()
{
	double time = omp_get_wtime();
	int walkersPerSample = this->model.getNumberOfWalkers() / this->model.getWalkerSamples();
	double squaredDisplacement;
	double displacementX, displacementY, displacementZ;
	double X0, Y0, Z0;
	double XF, YF, ZF;
	double normalizedDisplacement;
	double nDx, nDy, nDz;
	double resolution = this->model.getImageVoxelResolution();
	double aliveWalkerFraction;

	// Relaxation / Absorption equivalence
	double absorptionFraction;
	double absorption = 0.0;
	double nonAbsorption = 1.0;
	if(this->PFGSE_config.getApplyAbsorption())
	{
		absorption = 1.0;
		nonAbsorption = 0.0;
	}

	vector<double> Dmsd; Dmsd.reserve(this->model.getWalkerSamples());
	vector<double> DmsdX; DmsdX.reserve(this->model.getWalkerSamples());
	vector<double> DmsdY; DmsdY.reserve(this->model.getWalkerSamples());
	vector<double> DmsdZ; DmsdZ.reserve(this->model.getWalkerSamples());
	vector<double> msd; msd.reserve(this->model.getWalkerSamples());
	vector<double> msdX; msdX.reserve(this->model.getWalkerSamples());
	vector<double> msdY; msdY.reserve(this->model.getWalkerSamples());
	vector<double> msdZ; msdZ.reserve(this->model.getWalkerSamples());

	// measure msd and Dmsd for each sample of walkers
	for(int sample = 0; sample < this->model.getWalkerSamples(); sample++)
	{	
		squaredDisplacement = 0.0;
		nDx = 0.0; nDy = 0.0; nDz = 0.0;
		aliveWalkerFraction = 0.0;

		for(uint idx = 0; idx < walkersPerSample; idx++)
		{
			int offset = sample * walkersPerSample;
			Walker particle(this->model.walkers[idx + offset]);

			// Get walker displacement
			// X:
			X0 = (double) particle.getInitialPositionX();
			XF = (double) particle.getCurrentPositionX();
			displacementX = resolution * (XF - X0);
			displacementX *= displacementX;
			
			// Y:
			Y0 = (double) particle.getInitialPositionY();
			YF = (double) particle.getCurrentPositionY();
			displacementY = resolution * (YF - Y0);
			displacementY *= displacementY;

			// Z:
			Z0 = (double) particle.getInitialPositionZ();
			ZF = (double) particle.getCurrentPositionZ();
			displacementZ = resolution * (ZF - Z0);
			displacementZ *= displacementZ;

			absorptionFraction = (absorption * particle.getEnergy() + nonAbsorption);
			aliveWalkerFraction += absorptionFraction;
			nDx += (absorptionFraction * displacementX);
			nDy += (absorptionFraction * displacementY);
			nDz += (absorptionFraction * displacementZ);
			normalizedDisplacement = displacementX + displacementY + displacementZ;
			squaredDisplacement += (absorptionFraction * normalizedDisplacement);
	 	}

		// set diffusion coefficient (see eq 2.18 - ref. Bergman 1995)
		squaredDisplacement = squaredDisplacement / aliveWalkerFraction;
		Dmsd.push_back(squaredDisplacement/(6.0 * (*this).getExposureTime()));
		msd.push_back(squaredDisplacement);

		nDx /= aliveWalkerFraction;
		nDy /= aliveWalkerFraction;
		nDz /= aliveWalkerFraction;

		msdX.push_back(nDx);
		msdY.push_back(nDy);
		msdZ.push_back(nDz);
		DmsdX.push_back((nDx / (2.0 * (*this).getExposureTime())));
		DmsdY.push_back((nDy / (2.0 * (*this).getExposureTime())));
		DmsdZ.push_back((nDz / (2.0 * (*this).getExposureTime())));

	}

	// measure mean value among all the samples
	double meanDmsd = (*this).mean(Dmsd);
	double meanDmsdX = (*this).mean(DmsdX);
	double meanDmsdY = (*this).mean(DmsdY);
	double meanDmsdZ = (*this).mean(DmsdZ);
	double meanMsd = (*this).mean(msd);
	double meanMsdX = (*this).mean(msdX);
	double meanMsdY = (*this).mean(msdY);
	double meanMsdZ = (*this).mean(msdZ);
	
	// set mean value among all the samples
	(*this).setMsd(meanMsd);
	(*this).setDmsd(meanDmsd);
	(*this).setVecMsd(meanMsdX, meanMsdY, meanMsdZ);
	(*this).setVecDmsd(meanDmsdX, meanDmsdY, meanDmsdZ);

	// set std deviation among all the samples
	(*this).setDmsdStdev((*this).stdDev(Dmsd, meanDmsd));
	(*this).setMsdStdev((*this).stdDev(msd, meanMsd));
	(*this).setVecDmsdStdev((*this).stdDev(DmsdX, meanDmsdX), (*this).stdDev(DmsdY, meanDmsdY), (*this).stdDev(DmsdZ, meanDmsdZ));
	(*this).setVecMsdStdev((*this).stdDev(msdX, meanMsdX), (*this).stdDev(msdY, meanMsdY), (*this).stdDev(msdZ, meanMsdZ));
	
	// print results
	cout << "D(" << (*this).getExposureTime((*this).getCurrentTime()) << " ms) {msd} = " << (*this).getDmsd();
	cout << " +/- " << (*this).getDmsdStdev() << endl;
	cout << "Dxx = " << this->vecDmsd.getX() << " +/- " << 1.96 * this->vecDmsdStdev.getX() << endl;
	cout << "Dyy = " << this->vecDmsd.getY() << " +/- " << 1.96 * this->vecDmsdStdev.getY() << endl;
	cout << "Dzz = " << this->vecDmsd.getZ() << " +/- " << 1.96 * this->vecDmsdStdev.getZ() << endl;	
}

void NMR_PFGSE::reset(double newBigDelta)
{
	(*this).clear();
	(*this).setExposureTime(newBigDelta);
	(*this).set();
	(*this).buildThresholdFromSamples(this->gradientPoints);
}

void NMR_PFGSE::reset()
{
	(*this).clear();
	(*this).set();
	(*this).buildThresholdFromSamples(this->gradientPoints);
}

void NMR_PFGSE::clear()
{
	if(this->gradient.size() > 0) this->gradient.clear();
	if(this->lhs.size() > 0) this->lhs.clear();
	if(this->rhs.size() > 0) this->rhs.clear();
}

void NMR_PFGSE::presave()
{
	// write pfgse data
	if(this->PFGSE_config.getSavePFGSE())
	{
		(*this).createResultsFile();
		(*this).writeParameters();
		(*this).writeGvector();
	}
}

void NMR_PFGSE::save()
{
	double time = omp_get_wtime();
    cout << "- saving results...";
    
    // write pfgse data
	if(this->PFGSE_config.getSavePFGSE())
	{
		(*this).writeResults();
		(*this).writeEchoes();
		(*this).writeMsd();
	}

    if(this->PFGSE_config.getSaveWalkers())
    {
        (*this).writeWalkers();
    }

    if(this->PFGSE_config.getSaveHistogram())
    {
    	this->model.updateHistogram();
    	(*this).writeHistogram();
    }    

    if(this->PFGSE_config.getSaveHistogramList())
    {
        (*this).writeHistogramList();;
    }  
	
	time = omp_get_wtime() - time;
    cout << "Ok. (" << time << " seconds)." << endl; 
}

void NMR_PFGSE::writeParameters()
{
	string filename = this->dir + "/PFGSE_parameters.txt";

	ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    Vector3D maxGradient(this->vecGradient[this->vecGradient.size() - 1]);
    const int precision = std::numeric_limits<double>::max_digits10;  
	file << "RWNMR-PFGSE Parameters" << endl; 
	file << setprecision(precision) << "D_0: " << this->model.getDiffusionCoefficient() << endl;  
    file << setprecision(precision) << "Pulse width: " << this->pulseWidth << endl;
    file << setprecision(precision) << "Giromagnetic Ratio: " << (*this).getModel().getGiromagneticRatio() << endl;
	file << setprecision(precision) << "Gradient direction: {" 
		 << (maxGradient.getX() / maxGradient.getNorm()) << ", "
		 << (maxGradient.getY() / maxGradient.getNorm()) << ", "
		 << (maxGradient.getZ() / maxGradient.getNorm()) << "}" << endl;
	file << setprecision(precision) << "Gradients: " << this->gradientPoints << endl;
	file << "Times: [";
	for(int time = 0; time < this->exposureTimes.size(); time++)
	{
		if(time > 0) file << ", ";
		file << setprecision(precision) << this->exposureTimes[time];
	}
	file << "]" << endl; 
    
    file.close();
}

void NMR_PFGSE::writeGvector()
{
	string filename = this->dir + "/PFGSE_gradient.csv";

	ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    file << "Id,";
    file << "Gx,";
    file << "Gy,";
    file << "Gz,";
    file << "Kx,";
    file << "Ky,";
    file << "Kz" << endl;

    uint size = this->gradientPoints;
    const int precision = std::numeric_limits<double>::max_digits10;    
    for (uint index = 0; index < size; index++)
    {
        file << setprecision(precision) << index
        << "," << this->vecGradient[index].getX()
        << "," << this->vecGradient[index].getY()
        << "," << this->vecGradient[index].getZ()
        << "," << this->vecK[index].getX()
        << "," << this->vecK[index].getY()
        << "," << this->vecK[index].getZ() << endl;
    }

    file.close();
}

void NMR_PFGSE::writeEchoes()
{
	string filename = this->dir + "/timesamples/echoes_" + std::to_string((*this).getCurrentTime()) + ".csv";

	ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    file << "Idx,";
    file << "Gradient,";
    file << "NMR_signal(mean),NMR_signal(noise),NMR_signal(std),";
    file << "SAT_lhs(mean),SAT_lhs(std),";
    file << "SAT_rhs" << endl;

    uint size = this->gradientPoints;
    const int precision = std::numeric_limits<double>::max_digits10;
    for (uint index = 0; index < size; index++)
    {
        file << setprecision(precision) << index
        << "," << this->gradient[index]
        << "," << this->mkt[index]
        << "," << this->rawNoise[index]
        << "," << this->mktStdev[index]
        << "," << this->lhs[index]
        << "," << this->lhsStdev[index]
        << "," << this->rhs[index] << endl;
    }

    file.close();
}

void NMR_PFGSE::writeMsd()
{
	string filename = this->dir + "/timesamples/msd_" + std::to_string((*this).getCurrentTime()) + ".csv";
	ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

	int walkersPerSample = this->model.getNumberOfWalkers();
	if(this->PFGSE_config.getAllowWalkerSampling())
		walkersPerSample /= this->model.getWalkerSamples();
    
    file << "msdX(mean),msdX(std),";
    file << "msdY(mean),msdY(std),";
    file << "msdZ(mean),msdZ(std),";
    file << "DmsdX(mean),DmsdX(std),";
    file << "DmsdY(mean),DmsdY(std),";
    file << "DmsdZ(mean),DmsdZ(std)" << endl;

    const int precision = std::numeric_limits<double>::max_digits10;
    file << setprecision(precision) << this->vecMsd.getX() << "," << this->vecMsdStdev.getX() << ",";
    file << setprecision(precision) << this->vecMsd.getY() << "," << this->vecMsdStdev.getY() << ",";
    file << setprecision(precision) << this->vecMsd.getZ() << "," << this->vecMsdStdev.getZ() << ",";
    file << setprecision(precision) << this->vecDmsd.getX() << "," << this->vecDmsdStdev.getX() << ",";
    file << setprecision(precision) << this->vecDmsd.getY() << "," << this->vecDmsdStdev.getY() << ",";
    file << setprecision(precision) << this->vecDmsd.getZ() << "," << this->vecDmsdStdev.getZ();  

    file.close();
}

void NMR_PFGSE::writeWalkers()
{
	string filename = this->dir + "/timesamples/walkers_" + std::to_string((*this).getCurrentTime()) + ".csv";
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
        << "," << this->model.walkers[index].getCurrentPositionX() 
        << "," << this->model.walkers[index].getCurrentPositionY() 
        << "," << this->model.walkers[index].getCurrentPositionZ() 
        << "," << this->model.walkers[index].getCollisions() 
        << "," << this->model.walkers[index].getXiRate() 
        << "," << this->model.walkers[index].getEnergy() 
        << "," << this->model.walkers[index].getInitialSeed() << endl;
    }

    file.close();
}

void NMR_PFGSE::writeHistogram()
{
	string filename = this->dir + "/timesamples/histogram_" + std::to_string((*this).getCurrentTime()) + ".csv";
	ofstream file;
	file.open(filename, ios::out);
	if (file.fail())
	{
		cout << "Could not open file from disc." << endl;
		exit(1);
	}

	file << "Bins"; 
	file << ",Amps" << endl;
	const int num_points = this->model.getHistogram().getSize();
	const int precision = std::numeric_limits<double>::max_digits10;
	for (int i = 0; i < num_points; i++)
	{
		file << setprecision(precision) 
		<< this->model.getHistogram().getBin(i) 
		<< "," << this->model.getHistogram().getAmp(i) << endl;
	}

	file.close();
}

void NMR_PFGSE::writeHistogramList()
{
	string filename = this->dir + "/timesamples/histList_" + std::to_string((*this).getCurrentTime()) + ".csv";
	ofstream file;
	file.open(filename, ios::out);
	if (file.fail())
	{
		cout << "Could not open file from disc." << endl;
		exit(1);
	}

	const int histograms = this->model.getHistogramList().size();

	for(int hIdx = 0; hIdx < histograms; hIdx++)
	{
		file << "Bins" << hIdx << ",";
		file << "Amps" << hIdx << ",";
	}
	file << endl;

	const int num_points = this->model.getHistogram().getSize();
	const int precision = std::numeric_limits<double>::max_digits10;
	for (int i = 0; i < num_points; i++)
	{
		for(int hIdx = 0; hIdx < histograms; hIdx++)
		{
			file << setprecision(precision)	<< this->model.getHistogramList()[hIdx].getBin(i) << ",";
			file << setprecision(precision)	<< this->model.getHistogramList()[hIdx].getAmp(i) << ",";
		}

		file << endl;
	}

	file.close();
}

void NMR_PFGSE::createResultsFile()
{
	string filename = this->dir + "/PFGSE_results.csv";

	ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

	file << "Time";
    file << ",Dsat";
    file << ",Dsat(error)";
    file << ",Dsat(std)";
    file << ",Dsat(pts)";
    file << ",Dmsd";
    file << ",Dmsd(std)";
    file << ",DmsdX";
    file << ",DmsdX(std)";
    file << ",DmsdY";
    file << ",DmsdY(std)";
    file << ",DmsdZ";
    file << ",DmsdZ(std)";
    file << endl;
    file.close();
}

void NMR_PFGSE::writeResults()
{
	string filename = this->dir + "/PFGSE_results.csv";

	ofstream file;
    file.open(filename, ios::app);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    const int precision = std::numeric_limits<double>::max_digits10;
    file << setprecision(precision)  << this->exposureTimes[this->getCurrentTime()]
    << "," << this->Dsat
    << "," << this->DsatError
    << "," << this->DsatStdev
    << "," << this->DsatAdjustSamples
    << "," << this->Dmsd
    << "," << this->DmsdStdev
    << "," << this->vecDmsd.getX()
    << "," << this->vecDmsdStdev.getX()
    << "," << this->vecDmsd.getY()
    << "," << this->vecDmsdStdev.getY()
    << "," << this->vecDmsd.getZ()
    << "," << this->vecDmsdStdev.getZ()
    << endl; 
	file.close();
}

// pfgse simulation cpu-only implementation -- needs revision! 
void NMR_PFGSE::simulation_omp()
{
	double begin_time = omp_get_wtime();

    cout << "initializing RW-PFGSE-NMR simulation... ";

    // reset walker's initial state with omp parallel for
// #pragma if(NMR_OPENMP) omp parallel for private(id) shared(walkers)
    for (uint id = 0; id < this->model.walkers.size(); id++)
    {
        this->model.walkers[id].resetPosition();
        this->model.walkers[id].resetSeed();
        this->model.walkers[id].resetEnergy();
    }

    // set derivables
    double gamma = (*this).getModel().getGiromagneticRatio();
    
	myAllocator arrayFactory; 
	double *globalPhase = arrayFactory.getDoubleArray(this->gradientPoints);
    double globalEnergy = 0.0;
    double resolution = this->model.getImageVoxelResolution();
    
    // main loop
	// reset walker's initial state with omp parallel for
    if(this->model.getRWNMRConfig().getOpenMPUsage())
    {
        // set omp variables for parallel loop throughout walker list
        const int num_cpu_threads = omp_get_max_threads();
        const int loop_size = this->model.walkers.size();
        int loop_start, loop_finish; 

		#pragma omp parallel shared(gamma, globalPhase, globalEnergy, resolution) private(loop_start, loop_finish) 
        {
            const int thread_id = omp_get_thread_num();
            OMPLoopEnabler looper(thread_id, num_cpu_threads, loop_size);
            loop_start = looper.getStart();
            loop_finish = looper.getFinish(); 

			double walkerPhase;
			double walkerEnergy;

            for(uint id = loop_start; id < loop_finish; id++)
            {
				// reset energy
				this->model.walkers[id].resetPosition();
				this->model.walkers[id].resetSeed();
				this->model.walkers[id].resetEnergy();
				
				// make walkers walk througout image
				for (uint step = 0; step < this->model.getSimulationSteps(); step++)
				{
					this->model.walkers[id].walk(this->model.getBitBlock());     
				}

				// get final individual signal
				walkerEnergy = this->model.walkers[id].getEnergy();
				#pragma omp critical
				{
					globalEnergy += walkerEnergy;
				}

				// get final individual phase
				double dX = ((double) this->model.walkers[id].getCurrentPositionX()) - ((double) this->model.walkers[id].getInitialPositionX());
				double dY = ((double) this->model.walkers[id].getCurrentPositionY()) - ((double) this->model.walkers[id].getInitialPositionY());
				double dZ = ((double) this->model.walkers[id].getCurrentPositionZ()) - ((double) this->model.walkers[id].getInitialPositionZ());

				Vector3D dR(dX,dY,dZ);
				Vector3D wavevector_k;
				for(int point = 0; point < this->gradientPoints; point++)
				{ 
					double kx = computeWaveVectorK(this->vecGradient[point].getX(), (*this).getPulseWidth(), gamma);
					double ky = computeWaveVectorK(this->vecGradient[point].getY(), (*this).getPulseWidth(), gamma);
					double kz = computeWaveVectorK(this->vecGradient[point].getZ(), (*this).getPulseWidth(), gamma);
					wavevector_k.setX(kx);
					wavevector_k.setY(ky);
					wavevector_k.setZ(kz);
					
					walkerPhase = walkerEnergy * cos(wavevector_k.dotProduct(dR) * resolution);

					// add contribution to global sum
					globalPhase[point] += walkerPhase;
				}
			}
		}
	} else
	{
		double walkerPhase;
		double walkerEnergy;

		for(uint id = 0; id < this->model.walkers.size(); id++)
		{
			// reset energy
			this->model.walkers[id].resetPosition();
			this->model.walkers[id].resetSeed();
			this->model.walkers[id].resetEnergy();
			
			// make walkers walk througout image
			for (uint step = 0; step < this->model.getSimulationSteps(); step++)
			{
				this->model.walkers[id].walk(this->model.getBitBlock());
			}

			// get final individual signal
			walkerEnergy = this->model.walkers[id].getEnergy();
			globalEnergy += walkerEnergy;
			

			// get final individual phase
			double dX = ((double) this->model.walkers[id].getCurrentPositionX()) - ((double) this->model.walkers[id].getInitialPositionX());
			double dY = ((double) this->model.walkers[id].getCurrentPositionY()) - ((double) this->model.walkers[id].getInitialPositionY());
			double dZ = ((double) this->model.walkers[id].getCurrentPositionZ()) - ((double) this->model.walkers[id].getInitialPositionZ());

			Vector3D dR(dX,dY,dZ);
			Vector3D wavevector_k;
			for(int point = 0; point < this->gradientPoints; point++)
			{ 
				double kx = computeWaveVectorK(this->vecGradient[point].getX(), (*this).getPulseWidth(), gamma);
				double ky = computeWaveVectorK(this->vecGradient[point].getY(), (*this).getPulseWidth(), gamma);
				double kz = computeWaveVectorK(this->vecGradient[point].getZ(), (*this).getPulseWidth(), gamma);
				wavevector_k.setX(kx);
				wavevector_k.setY(ky);
				wavevector_k.setZ(kz);
				
				walkerPhase = walkerEnergy * cos(wavevector_k.dotProduct(dR) * resolution);

				// add contribution to global sum
				globalPhase[point] += walkerPhase;
			}
		}
	}
	
	
	// get magnitudes M(k,t)
    for(int point = 0; point < this->gradientPoints; point++)
    {
        this->mkt.push_back((globalPhase[point]/globalEnergy));
    }

	// delete global phase array
	delete [] globalPhase;
	globalPhase = NULL;

    double finish_time = omp_get_wtime();
    cout << "Completed."; BaseFunctions::printElapsedTime(begin_time, finish_time);
    return;
}

double NMR_PFGSE::sum(vector<double> &_vec)
{
	double sum = 0;
	double size = (double) _vec.size();

    for (uint id = 0; id < _vec.size(); id++)
    {
        sum += _vec[id];
    }

    return sum;
}

double NMR_PFGSE::sum(double *_vec, int _size)
{
	double sum = 0;

    for (uint id = 0; id < _size; id++)
    {
        sum += _vec[id];
    }

    return sum;
}

double NMR_PFGSE::mean(vector<double> &_vec)
{
	double sum = 0;
	double size = (double) _vec.size();

    for (uint id = 0; id < _vec.size(); id++)
    {
        sum += _vec[id];
    }

    return (sum / size);
}

double NMR_PFGSE::mean(double *_vec, int _size)
{
	double sum = 0;

    for (uint id = 0; id < _size; id++)
    {
        sum += _vec[id];
    }

    return (sum / (double) _size);
}

double NMR_PFGSE::stdDev(vector<double> &_vec)
{
    return (*this).stdDev(_vec, (*this).mean(_vec));
}

double NMR_PFGSE::stdDev(vector<double> &_vec, double mean)
{
	double sum = 0.0;
    int size = _vec.size();

    for(uint idx = 0; idx < _vec.size(); idx++)
    {
        sum += (_vec[idx] - mean) * (_vec[idx] - mean); 
    }

    return sqrt(sum/((double) size));
}

double NMR_PFGSE::stdDev(double *_vec, int _size)
{
    return (*this).stdDev(_vec, _size, (*this).mean(_vec, _size));
}

double NMR_PFGSE::stdDev(double *_vec, int _size, double mean)
{
	double sum = 0.0;

    for(uint idx = 0; idx < _size; idx++)
    {
        sum += (_vec[idx] - mean) * (_vec[idx] - mean); 
    }

    return sqrt(sum/((double) _size));
}

vector<double> NMR_PFGSE::getNormalDistributionSamples(const double loc, const double std, const int size)
{
	vector<double> randomData;
	randomData.reserve(size);
	std::normal_distribution<double> distribution(loc, std);
	
	for (int i = 0; i < size; i++)
	{
		randomData.emplace_back(distribution(Model::_rng));
	}

	return randomData;
}