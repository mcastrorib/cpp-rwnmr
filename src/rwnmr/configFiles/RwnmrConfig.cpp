#include "RwnmrConfig.h"

using namespace std;

// default constructors
RwnmrConfig::RwnmrConfig(const string configFile, const string croot) : BaseConfig(croot, configFile), 
																	    SAVE_IMG_INFO(false), 
																		SAVE_BINIMG(false), 
																		SAVE_WALKERS(false), 
																		OPENMP_USAGE(true), 
																		OPENMP_THREADS(omp_get_max_threads()), 
																		GPU_USAGE(true), 
																		REDUCE_IN_GPU(true), 
																		WALKER_SAMPLES(1)
{
	vector<double> RHO();
	string defaultFile = (*this).getProjectRoot() + RWNMR_CONFIG_DEFAULT;
    if(configFile != (defaultFile)) (*this).readConfigFile(configFile);
    else (*this).readConfigFile(defaultFile);	
}

//copy constructors
RwnmrConfig::RwnmrConfig(const RwnmrConfig &otherConfig)
{
    this->NAME = otherConfig.NAME;
    this->WALKERS = otherConfig.WALKERS;
    this->WALKER_SAMPLES = otherConfig.WALKER_SAMPLES;
    this->WALKERS_PLACEMENT = otherConfig.WALKERS_PLACEMENT;
    this->PLACEMENT_DEVIATION = otherConfig.PLACEMENT_DEVIATION;
    this->RHO_TYPE = otherConfig.RHO_TYPE;
    this->RHO = otherConfig.RHO;
    this->GIROMAGNETIC_RATIO = otherConfig.GIROMAGNETIC_RATIO;
    this->GIROMAGNETIC_UNIT = otherConfig.GIROMAGNETIC_UNIT;
    this->D0 = otherConfig.D0; 
    this->BULK_TIME = otherConfig.BULK_TIME;
    this->STEPS_PER_ECHO = otherConfig.STEPS_PER_ECHO;
    this->SEED = otherConfig.SEED;
    this->BC = otherConfig.BC;

    // SAVE MODE
    this->SAVE_IMG_INFO = otherConfig.SAVE_IMG_INFO;
    this->SAVE_BINIMG = otherConfig.SAVE_BINIMG;
    this->SAVE_WALKERS = otherConfig.SAVE_WALKERS;

    // HISTOGRAM SIZE
    this->HISTOGRAMS = otherConfig.HISTOGRAMS;  
    this->HISTOGRAM_SIZE = otherConfig.HISTOGRAM_SIZE;
    this->HISTOGRAM_SCALE = otherConfig.HISTOGRAM_SCALE;

    // -- OPENMP MODE
    this->OPENMP_USAGE = otherConfig.OPENMP_USAGE;
    this->OPENMP_THREADS = otherConfig.OPENMP_THREADS;

    // -- CUDA/GPU PARAMETERS
    this->GPU_USAGE = otherConfig.GPU_USAGE;
    this->BLOCKS = otherConfig.BLOCKS;
    this->THREADSPERBLOCK = otherConfig.THREADSPERBLOCK;
    this->ECHOESPERKERNEL = otherConfig.ECHOESPERKERNEL;
    this->MAX_RWSTEPS = otherConfig.MAX_RWSTEPS;
    this->REDUCE_IN_GPU = otherConfig.REDUCE_IN_GPU;
}

vector<string> RwnmrConfig::checkConfig()
{
	vector<string> missingParameters;
    bool validState = true;
   
    validState &= (*this).checkItem((*this).getWalkers() > 0, (string)"WALKERS", missingParameters);
    validState &= (*this).checkItem((*this).getWalkerSamples() > 0, (string)"WALKER_SAMPLES", missingParameters);
    
	vector<string> placements = {"random", "center", "cubic"};
    validState &= (*this).checkItem(std::find(placements.begin(), placements.end(), (*this).getWalkersPlacement()) != placements.end(), 
                      (string)"WALKER_PLACEMENT", missingParameters);

	vector<string> rhoTypes = {"uniform", "sigmoid"};
    validState &= (*this).checkItem(std::find(rhoTypes.begin(), rhoTypes.end(), (*this).getRhoType()) != rhoTypes.end(), 
                      (string)"RHO_TYPE", missingParameters);	
	if((*this).getRhoType() == "uniform")
	{
		validState &= (*this).checkItem((*this).getRho().size() == 1, (string)"RHO_VALUE", missingParameters);
	} else if((*this).getRhoType() == "sigmoid")
	{
		validState &= (*this).checkItem(((*this).getRho().size() > 0 and (*this).getRho().size() % 4 == 0), (string)"RHO_VALUE", missingParameters);
	} 

    validState &= (*this).checkItem((*this).getGiromagneticRatio() > 0.0, (string)"GYROMAGNETIC_RATIO", missingParameters);
	vector<string> gUnits = {"mhertz", "rad"};
    validState &= (*this).checkItem(std::find(gUnits.begin(), gUnits.end(), (*this).getGiromagneticUnit()) != gUnits.end(), 
                      (string)"GYROMAGNETIC_UNIT", missingParameters);
	
	validState &= (*this).checkItem((*this).getD0() > 0.0, (string)"D0", missingParameters);
	validState &= (*this).checkItem((*this).getBulkTime() > 0.0, (string)"BULK_TIME", missingParameters);
	validState &= (*this).checkItem((*this).getStepsPerEcho() > 0, (string)"STEPS_PER_ECHO", missingParameters);
	
	vector<string> bcs = {"no-flux", "periodic", "mirror"};
	validState &= (*this).checkItem(std::find(bcs.begin(), bcs.end(), (*this).getBC()) != bcs.end(), 
                      (string)"BC", missingParameters);
		
	validState &= (*this).checkItem((*this).getHistograms() > 0, (string)"HISTOGRAMS", missingParameters);
	validState &= (*this).checkItem((*this).getHistogramSize() > 0, (string)"HISTOGRAM_SIZE", missingParameters);
	vector<string> hScales = {"linear", "log"};
	validState &= (*this).checkItem(std::find(hScales.begin(), hScales.end(), (*this).getHistogramScale()) != hScales.end(), 
                      (string)"HISTOGRAM_SCALE", missingParameters);
	
	validState &= (*this).checkItem(((*this).getOpenMPUsage() == false or ((*this).getOpenMPUsage() == true and (*this).getOpenMPThreads() > 0)), 
					(string)"OPENMP", missingParameters);
	validState &= (*this).checkItem((*this).getBlocks() > 0, (string)"CUDA_BLOCKS", missingParameters);
	validState &= (*this).checkItem((*this).getThreadsPerBlock() > 0, (string)"CUDA_THREADSPERBLOCK", missingParameters);
	validState &= (*this).checkItem((*this).getEchoesPerKernel() > 0, (string)"CUDA_ECHOESPERKERNEL", missingParameters);
	validState &= (*this).checkItem((*this).getMaxRWSteps() > 0, (string)"CUDA_MAXRWSTEPSPERKERNEL", missingParameters);	

    (*this).setReady(validState);   
    return missingParameters;
}

// read config file
void RwnmrConfig::readConfigFile(const string configFile)
{
    ifstream fileObject;
    fileObject.open(configFile, ios::in);
    if (fileObject.fail())
    {
        cout << "Could not open rwnmr config file from disc." << endl;
        exit(1);
    }

    string line;
    while(fileObject)
    {
    	getline(fileObject, line);

    	string s = line;
    	string delimiter = ": ";
		size_t pos = 0;
		string token, content;
    	while ((pos = s.find(delimiter)) != std::string::npos) 
    	{
			token = s.substr(0, pos);
			content = s.substr(pos + delimiter.length(), s.length());
			s.erase(0, pos + delimiter.length());

			if(token == "NAME")	(*this).readName(content);
			else if(token == "WALKERS") (*this).readWalkers(content);
			else if(token == "WALKER_SAMPLES") (*this).readWalkerSamples(content);
			else if(token == "WALKERS_PLACEMENT") (*this).readWalkersPlacement(content);
			else if(token == "PLACEMENT_DEVIATION") (*this).readPlacementDeviation(content);
			else if(token == "RHO_TYPE") (*this).readRhoType(content);
			else if(token == "RHO") (*this).readRho(content);
			else if(token == "STEPS_PER_ECHO") (*this).readStepsPerEcho(content);
			else if(token == "GIROMAGNETIC_RATIO")	(*this).readGiromagneticRatio(content);
			else if(token == "GIROMAGNETIC_UNIT")	(*this).readGiromagneticUnit(content);
			else if(token == "D0") (*this).readD0(content);
			else if(token == "BULK_TIME") (*this).readBulkTime(content);
			else if(token == "SEED") (*this).readSeed(content);
			else if(token == "BC") (*this).readBC(content);
			else if(token == "SAVE_IMG_INFO") (*this).readSaveImgInfo(content);
			else if(token == "SAVE_BINIMG") (*this).readSaveBinImg(content);
			else if(token == "SAVE_WALKERS") (*this).readSaveWalkers(content);
			else if(token == "HISTOGRAMS") (*this).readHistograms(content);
			else if(token == "HISTOGRAM_SIZE") (*this).readHistogramSize(content);
			else if(token == "HISTOGRAM_SCALE") (*this).readHistogramScale(content);
			else if(token == "OPENMP_USAGE") (*this).readOpenMPUsage(content);
			else if(token == "OPENMP_THREADS") (*this).readOpenMPThreads(content);
			else if(token == "GPU_USAGE") (*this).readGPUUsage(content);
			else if(token == "BLOCKS") (*this).readBlocks(content);
			else if(token == "THREADSPERBLOCK") (*this).readThreadsPerBlock(content);
			else if(token == "ECHOESPERKERNEL") (*this).readEchoesPerKernel(content);
			else if(token == "REDUCE_IN_GPU") (*this).readReduceInGPU(content);
			else if(token == "MAX_RWSTEPS") (*this).readMaxRWSteps(content);
		}
    } 

    fileObject.close();
}

void RwnmrConfig::readName(string s)
{
	this->NAME = s;
}

void RwnmrConfig::readWalkers(string s)
{
	this->WALKERS = std::stoi(s);
}

void RwnmrConfig::readWalkerSamples(string s)
{
	this->WALKER_SAMPLES = std::stoi(s);
}

void RwnmrConfig::readWalkersPlacement(string s)
{
	if(s == "point" or s == "cubic") this->WALKERS_PLACEMENT = s;
	else this->WALKERS_PLACEMENT = "random";
}

void RwnmrConfig::readPlacementDeviation(string s)
{
	this->PLACEMENT_DEVIATION = std::stoi(s);
}

void RwnmrConfig::readRhoType(string s)
{
	if(s == "uniform") this->RHO_TYPE = "uniform";
	else if(s == "sigmoid") this->RHO_TYPE = "sigmoid";
	else this->RHO_TYPE = "undefined";
}

void RwnmrConfig::readRho(string s) // vector?
{
		// parse vector
		if(s.compare(0, 1, "{") == 0 and s.compare(s.length() - 1, 1, "}") == 0)
		{
			string strvec = s.substr(1, s.length() - 2);
			string delimiter = ",";
			size_t pos = 0;
			string token, content;
			while ((pos = strvec.find(delimiter)) != std::string::npos) 
	    	{
				token = strvec.substr(0, pos);
				content = strvec.substr(pos + delimiter.length(), strvec.length());
				strvec.erase(0, pos + delimiter.length());

				// add value to RHO attribute
				this->RHO.push_back(std::stod(token));
			}
			// add value to RHO attribute
			this->RHO.push_back(std::stod(strvec));
		} else
		{
			this->RHO.push_back(std::stod(s));
		}		
}

void RwnmrConfig::readGiromagneticRatio(string s)
{
	this->GIROMAGNETIC_RATIO = std::stod(s);
}

void RwnmrConfig::readGiromagneticUnit(string s)
{
	if(s == "MHertz" or s == "mhertz") this->GIROMAGNETIC_UNIT = "mhertz";
	else this->GIROMAGNETIC_UNIT = "rad";
}

void RwnmrConfig::readD0(string s)
{
	this->D0 = std::stod(s);
}

void RwnmrConfig::readBulkTime(string s)
{
	this->BULK_TIME = std::stod(s);
}

void RwnmrConfig::readStepsPerEcho(string s)
{
	this->STEPS_PER_ECHO = std::stoi(s);
}

void RwnmrConfig::readSeed(string s)
{
	this->SEED = std::stol(s);
}

void RwnmrConfig::readBC(string s)
{
	if(s == "periodic") 
	{
		this->BC = "periodic";
	} else if(s == "mirror") 
	{
		this->BC = "mirror";
	} else if(s == "debug" or s == "test")
	{
		this->BC = "test";
	} else this->BC = "noflux";
}

// -- Saving
void RwnmrConfig::readSaveImgInfo(string s)
{
	if(s == "true") this->SAVE_IMG_INFO = true;
	else this->SAVE_IMG_INFO = false;
}

void RwnmrConfig::readSaveBinImg(string s)
{
	if(s == "true") this->SAVE_BINIMG = true;
	else this->SAVE_BINIMG = false;
}

void RwnmrConfig::readSaveWalkers(string s)
{
	if(s == "true") this->SAVE_WALKERS = true;
	else this->SAVE_WALKERS = false;
}

// Histograms
void RwnmrConfig::readHistograms(string s)
{
	this->HISTOGRAMS = std::stoi(s);
}

void RwnmrConfig::readHistogramSize(string s)
{
	this->HISTOGRAM_SIZE = std::stoi(s);
}

void RwnmrConfig::readHistogramScale(string s)
{
	if(s == "log") this->HISTOGRAM_SCALE = "log";
	else this->HISTOGRAM_SCALE = "linear";
}

// -- OpenMP
void RwnmrConfig::readOpenMPUsage(string s)
{
	if(s == "true") this->OPENMP_USAGE = true;
	else this->OPENMP_USAGE = false;
}

void RwnmrConfig::readOpenMPThreads(string s)
{
	this->OPENMP_THREADS = std::stoi(s);
}

// -- CUDA/GPU Params
void RwnmrConfig::readGPUUsage(string s)
{
	if(s == "true") this->GPU_USAGE = true;
	else this->GPU_USAGE = false;
}

void RwnmrConfig::readBlocks(string s)
{
	this->BLOCKS = std::stoi(s);
}

void RwnmrConfig::readThreadsPerBlock(string s)
{
	this->THREADSPERBLOCK = std::stoi(s);
}

void RwnmrConfig::readEchoesPerKernel(string s)
{
	this->ECHOESPERKERNEL = std::stoi(s);
}

void RwnmrConfig::readMaxRWSteps(string s)
{
	this->MAX_RWSTEPS = std::stoi(s);
}

void RwnmrConfig::readReduceInGPU(string s)
{
	if(s == "true") this->REDUCE_IN_GPU = true;
	else this->REDUCE_IN_GPU = false;
}
