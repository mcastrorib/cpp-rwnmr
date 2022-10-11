#ifndef RWNMR_CONFIG_H_
#define RWNMR_CONFIG_H_

#include "configFiles_defs.h"

using namespace std;

class rwnmr_config
{
private:
    bool ready;
    string config_filepath;
    string NAME;
    uint WALKERS;
    uint WALKER_SAMPLES;
    string WALKERS_PLACEMENT;
    uint PLACEMENT_DEVIATION;
    string RHO_TYPE;
    vector<double> RHO;
    double D0;
    double GIROMAGNETIC_RATIO;
    string GIROMAGNETIC_UNIT;
    double BULK_TIME; 
    uint STEPS_PER_ECHO;
    uint64_t SEED;
    string BC;

    // SAVE MODE
    bool SAVE_IMG_INFO;
    bool SAVE_BINIMG;
    bool SAVE_WALKERS;

    // HISTOGRAM SIZE
    uint HISTOGRAMS;  
    uint HISTOGRAM_SIZE;
    string HISTOGRAM_SCALE;

    // -- OPENMP MODE
    bool OPENMP_USAGE;
    uint OPENMP_THREADS;

    // -- CUDA/GPU PARAMETERS
    bool GPU_USAGE;
    uint BLOCKS;
    uint THREADSPERBLOCK;
    uint ECHOESPERKERNEL;
    uint MAX_RWSTEPS;
    bool REDUCE_IN_GPU;
    
public:
    // default constructors
    rwnmr_config():ready(false){};
    rwnmr_config(const string configFile, const string croot);

    //copy constructors
    rwnmr_config(const rwnmr_config &otherConfig);    

    // default destructor
    virtual ~rwnmr_config()
    {
        // cout << "OMPLoopEnabler object destroyed succesfully" << endl;
    } 

    void readConfigFile(const string configFile);
    void checkConfig();
    
    // Read methods
    // -- RW Params
    void readName(string s);
    void readWalkers(string s);
    void readWalkerSamples(string s);
    void readWalkersPlacement(string s);
    void readPlacementDeviation(string s);
    void readRhoType(string s);
    void readRho(string s); 
    void readD0(string s); 
    void readGiromagneticRatio(string s);    
    void readGiromagneticUnit(string s);    
    void readBulkTime(string s);
    void readStepsPerEcho(string s);
    void readSeed(string s);
    void readBC(string s);

    // Histograms
    void readHistograms(string s);  
    void readHistogramSize(string s);
    void readHistogramScale(string s);

    // -- OpenMP
    void readOpenMPUsage(string s);
    void readOpenMPThreads(string s);

    // -- CUDA/GPU Params
    void readGPUUsage(string s);
    void readBlocks(string s);
    void readThreadsPerBlock(string s);
    void readEchoesPerKernel(string s);
    void readMaxRWSteps(string s);
    void readReduceInGPU(string s);
    
    // -- MPI Params
    void readBitBlockBatchesSize(string s);
    void readBitBlockPropertiesSize(string s);
    void readNMRT2Size(string s);
    void readStartTag(string s);
    void readBitBlockTag(string s);
    void readBatchTag(string s);
    void readT2Tag(string s);
    void readEndTag(string s);

    // -- Saving
    void readSaveImgInfo(string s);
    void readSaveBinImg(string s);
    void readSaveWalkers(string s);

    // Set methods
    // -- RW Params
    void setName(string s){ this->NAME = s;}
    void setWalkers(uint s){ this->WALKERS = s;}
    void setWalkerSamples(uint s){ this->WALKER_SAMPLES = s;}
    void setWalkersPlacement(string s){ this->WALKERS_PLACEMENT = s;}
    void setPlacementDeviation(uint s){ this->PLACEMENT_DEVIATION = s;}
    void setRhoType(string s){ this->RHO_TYPE = s;}
    void setRho(vector<double> s){ this->RHO = s;} 
    void setD0(double s){ this->D0 = s;} 
    void setGiromagneticRatio(double s){ this->GIROMAGNETIC_RATIO = s;}    
    void setGiromagneticUnit(string s){ this->GIROMAGNETIC_UNIT = s;}    
    void setBulkTime(double s){ this->BULK_TIME = s;}
    void setStepsPerEcho(uint s){ this->STEPS_PER_ECHO = s;}
    void setSeed(uint64_t s){ this->SEED = s;}
    void setBC(string s){ this->BC = s;}

    // Histograms
    void setHistograms(uint s){ this->HISTOGRAMS = s;}  
    void setHistogramSize(uint s){ this->HISTOGRAM_SIZE = s;}
    void setHistogramScale(string s){ this->HISTOGRAM_SCALE = s;}

    // -- OpenMP
    void setOpenMPUsage(bool s){ this->OPENMP_USAGE = s;}
    void setOpenMPThreads(uint s){ this->OPENMP_THREADS = s;}

    // -- CUDA/GPU Params
    void setGPUUsage(bool s){ this->GPU_USAGE = s;}
    void setBlocks(uint s){ this->BLOCKS = s;}
    void setThreadsPerBlock(uint s){ this->THREADSPERBLOCK = s;}
    void setEchoesPerKernel(uint s){ this->ECHOESPERKERNEL = s;}
    void setMaxRWSteps(uint s){ this->MAX_RWSTEPS = s;}
    void setReduceInGPU(bool s){ this->REDUCE_IN_GPU = s;}
    
    // -- Saving
    void setSaveImgInfo(bool s){ this->SAVE_IMG_INFO = s;}
    void setSaveBinImg(bool s){ this->SAVE_BINIMG = s;}
    void setSaveWalkers(bool s){ this->SAVE_WALKERS = s;}

    // Get methods
    // -- RW Params
    string getConfigFilepath(){ return this->config_filepath; }
    string getName(){ return this->NAME;}
    uint getWalkers(){ return this->WALKERS;}
    uint getWalkerSamples(){ return this->WALKER_SAMPLES;}
    string getWalkersPlacement(){ return this->WALKERS_PLACEMENT;}
    uint getPlacementDeviation(){ return this->PLACEMENT_DEVIATION;}
    string getRhoType(){ return this->RHO_TYPE;}
    vector<double> getRho(){ return this->RHO;} 
    double getGiromagneticRatio() { return this->GIROMAGNETIC_RATIO; }
    string getGiromagneticUnit() { return this->GIROMAGNETIC_UNIT; }
    double getD0(){ return this->D0;}
    double getBulkTime(){ return this->BULK_TIME; }
    uint getStepsPerEcho(){ return this->STEPS_PER_ECHO;}
    uint64_t getSeed(){ return  this->SEED;}
    string getBC() { return this->BC; }

    // -- Saving
    bool getSaveImgInfo(){ return this->SAVE_IMG_INFO;}
    bool getSaveBinImg(){ return this->SAVE_BINIMG;}
    bool getSaveWalkers(){ return this->SAVE_WALKERS;}

    // Histograms
    uint getHistograms(){ return this->HISTOGRAMS;}
    uint getHistogramSize(){ return this->HISTOGRAM_SIZE;}
    string getHistogramScale(){ return this->HISTOGRAM_SCALE;}

    // -- OpenMP
    bool getOpenMPUsage(){ return this->OPENMP_USAGE;}
    uint getOpenMPThreads(){ return this->OPENMP_THREADS;}

    // -- CUDA/GPU Params
    bool getGPUUsage(){ return this->GPU_USAGE;}
    uint getBlocks(){ return this->BLOCKS;}
    uint getThreadsPerBlock(){ return this->THREADSPERBLOCK;}
    uint getEchoesPerKernel(){ return this->ECHOESPERKERNEL;}
    uint getMaxRWSteps(){ return this->MAX_RWSTEPS;}
    bool getReduceInGPU(){ return this->REDUCE_IN_GPU;}

    // ready
private:
    void setReady(bool _b){ this->ready = _b; }
public:
    bool getReady(){ return this->ready; }
};

#endif