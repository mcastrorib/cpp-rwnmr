#include "rwnmr_config.h"

using namespace std;

// default constructors
rwnmr_config::rwnmr_config(const string configFile, const string croot) : BaseConfig(croot, configFile), WALKER_SAMPLES(1)
{
	vector<double> RHO();
	string defaultFile = (*this).getProjectRoot() + RWNMR_CONFIG_DEFAULT;
    if(configFile != (defaultFile)) (*this).readConfigFile(configFile);
    else (*this).readConfigFile(defaultFile);	
}

//copy constructors
rwnmr_config::rwnmr_config(const rwnmr_config &otherConfig)
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

// read config file
void rwnmr_config::readConfigFile(const string configFile)
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

void rwnmr_config::readName(string s)
{
	this->NAME = s;
}

void rwnmr_config::readWalkers(string s)
{
	this->WALKERS = std::stoi(s);
}

void rwnmr_config::readWalkerSamples(string s)
{
	this->WALKER_SAMPLES = std::stoi(s);
}

void rwnmr_config::readWalkersPlacement(string s)
{
	this->WALKERS_PLACEMENT = s;
}

void rwnmr_config::readPlacementDeviation(string s)
{
	this->PLACEMENT_DEVIATION = std::stoi(s);
}

void rwnmr_config::readRhoType(string s)
{
	if(s == "uniform") this->RHO_TYPE = "uniform";
	else if(s == "sigmoid") this->RHO_TYPE = "sigmoid";
	else this->RHO_TYPE = "undefined";
}

void rwnmr_config::readRho(string s) // vector?
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

void rwnmr_config::readGiromagneticRatio(string s)
{
	this->GIROMAGNETIC_RATIO = std::stod(s);
}

void rwnmr_config::readGiromagneticUnit(string s)
{
	if(s == "MHertz" or s == "mhertz") this->GIROMAGNETIC_UNIT = "mhertz";
	else this->GIROMAGNETIC_UNIT = "rad";
}

void rwnmr_config::readD0(string s)
{
	this->D0 = std::stod(s);
}

void rwnmr_config::readBulkTime(string s)
{
	this->BULK_TIME = std::stod(s);
}

void rwnmr_config::readStepsPerEcho(string s)
{
	this->STEPS_PER_ECHO = std::stoi(s);
}

void rwnmr_config::readSeed(string s)
{
	this->SEED = std::stol(s);
}

void rwnmr_config::readBC(string s)
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
void rwnmr_config::readSaveImgInfo(string s)
{
	if(s == "true") this->SAVE_IMG_INFO = true;
	else this->SAVE_IMG_INFO = false;
}

void rwnmr_config::readSaveBinImg(string s)
{
	if(s == "true") this->SAVE_BINIMG = true;
	else this->SAVE_BINIMG = false;
}

void rwnmr_config::readSaveWalkers(string s)
{
	if(s == "true") this->SAVE_WALKERS = true;
	else this->SAVE_WALKERS = false;
}

// Histograms
void rwnmr_config::readHistograms(string s)
{
	this->HISTOGRAMS = std::stoi(s);
}

void rwnmr_config::readHistogramSize(string s)
{
	this->HISTOGRAM_SIZE = std::stoi(s);
}

void rwnmr_config::readHistogramScale(string s)
{
	if(s == "log") this->HISTOGRAM_SCALE = "log";
	else this->HISTOGRAM_SCALE = "linear";
}

// -- OpenMP
void rwnmr_config::readOpenMPUsage(string s)
{
	if(s == "true") this->OPENMP_USAGE = true;
	else this->OPENMP_USAGE = false;
}

void rwnmr_config::readOpenMPThreads(string s)
{
	this->OPENMP_THREADS = std::stoi(s);
}

// -- CUDA/GPU Params
void rwnmr_config::readGPUUsage(string s)
{
	if(s == "true") this->GPU_USAGE = true;
	else this->GPU_USAGE = false;
}

void rwnmr_config::readBlocks(string s)
{
	this->BLOCKS = std::stoi(s);
}

void rwnmr_config::readThreadsPerBlock(string s)
{
	this->THREADSPERBLOCK = std::stoi(s);
}

void rwnmr_config::readEchoesPerKernel(string s)
{
	this->ECHOESPERKERNEL = std::stoi(s);
}

void rwnmr_config::readMaxRWSteps(string s)
{
	this->MAX_RWSTEPS = std::stoi(s);
}

void rwnmr_config::readReduceInGPU(string s)
{
	if(s == "true") this->REDUCE_IN_GPU = true;
	else this->REDUCE_IN_GPU = false;
}
