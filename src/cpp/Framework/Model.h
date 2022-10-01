#ifndef MODEL_H_
#define MODEL_H_

#include "Defs.h"
#include "Point3D.h"
#include "BitBlock.h"
#include "Walker.h"
#include "CollisionHistogram.h"

class Model
{
public:
    // RNG State
    static std::mt19937 _rng;

private:    
    // Class attributes:
    // Config attributes
    rwnmr_config rwNMR_config;
    uct_config uCT_config;

    // RW simulation parameters
    string name;
    string dir;
    string dbPath;
    uint simulationSteps;
    uint stepsPerEcho;
    uint numberOfEchoes;
    uint64_t initialSeed;
    bool seedFlag;
    bool gpuUsage;

    uint numberOfPores;
    double porosity;
    uint interfacePoreMatrix;
    double svpRatio;
    double walkerOccupancy;
    uint numberOfWalkers;
    uint walkerSamples;

    vector<Point3D> pores;
    vector<uint> walkersIdxList;
    vector<Walker> *walkers;

    // physical properties
    double timeInterval; // time interval between each walker step
    double diffusionCoefficient;
    double giromagneticRatio;
    double bulkRelaxationTime;

    // image attributes
    ImagePath imagePath;
    uint numberOfImages;
    double imageResolution;
    double imageVoxelResolution;    
    uint height;
    uint width;
    uint depth;
    vector<Mat> binaryMap;
    BitBlock *bitBlock;
    string boundaryCondition;
    int voxelDivision;
    bool voxelDivisionApplied;

    // Collision histogram 
    CollisionHistogram histogram;
    vector<CollisionHistogram> histogramList;

public:
    // NMR_3D methods:
    // default constructors
    Model(rwnmr_config _rwNMR_config, uct_config _uCT_config, string _project_root);

    //copy constructors
    // copy constructor
    Model(const Model &_otherSimulation)
    {
        this->rwNMR_config = _otherSimulation.rwNMR_config;
        this->uCT_config = _otherSimulation.uCT_config;
        this->name = _otherSimulation.name;
        this->dir = _otherSimulation.dir;
        this->simulationSteps = _otherSimulation.simulationSteps;
        this->stepsPerEcho = _otherSimulation.stepsPerEcho;
        this->numberOfEchoes = _otherSimulation.numberOfEchoes;
        this->initialSeed = _otherSimulation.initialSeed;
        this->seedFlag = _otherSimulation.seedFlag;
        this->gpuUsage = _otherSimulation.gpuUsage;

        this->numberOfPores = _otherSimulation.numberOfPores;
        this->porosity = _otherSimulation.porosity;
        this->interfacePoreMatrix = _otherSimulation.interfacePoreMatrix;
        this->svpRatio = _otherSimulation.svpRatio;
        this->walkerOccupancy = _otherSimulation.walkerOccupancy;
        this->numberOfWalkers = _otherSimulation.numberOfWalkers;
        this->walkerSamples = _otherSimulation.walkerSamples;

        this->pores = _otherSimulation.pores;
        this->walkersIdxList = _otherSimulation.walkersIdxList;
        this->walkers = _otherSimulation.walkers;

        this->timeInterval = _otherSimulation.timeInterval;
        this->diffusionCoefficient = _otherSimulation.diffusionCoefficient;
        this->giromagneticRatio = _otherSimulation.giromagneticRatio;
        this->bulkRelaxationTime = _otherSimulation.bulkRelaxationTime;

        this->imagePath = _otherSimulation.imagePath;
        this->numberOfImages = _otherSimulation.numberOfImages;
        this->imageResolution = _otherSimulation.imageResolution;
        this->imageVoxelResolution = _otherSimulation.imageVoxelResolution;
        this->height = _otherSimulation.height;
        this->width = _otherSimulation.width;
        this->depth = _otherSimulation.depth;
        this->binaryMap = _otherSimulation.binaryMap;
        this->bitBlock = _otherSimulation.bitBlock;
        this->boundaryCondition = _otherSimulation.boundaryCondition;
        this->voxelDivision = _otherSimulation.voxelDivision;
        this->voxelDivisionApplied = _otherSimulation.voxelDivisionApplied;

        this->histogram = _otherSimulation.histogram;
        this->histogramList = _otherSimulation.histogramList;

        // pointers-to-method
        this->mapSimulationPointer = _otherSimulation.mapSimulationPointer;
    }

    // default destructor
    virtual ~Model()
    {
        (*this).clear();
        if(this->bitBlock != NULL)
        {
            delete this->bitBlock;
            this->bitBlock = NULL;
        }

        if(this->walkers != NULL)
        {
            delete this->walkers;
            this->walkers = NULL;
        }
        cout << "Simulation model object destroyed." << endl;
    }

    // Set methods
    void setRWNMRConfig(rwnmr_config _config){ this->rwNMR_config = _config; }
    void setUCTConfig(uct_config _config){ this->uCT_config = _config; }
    void setName(string _n){ this->name = _n; }
    void setDir(string _d){ this->dir = _d; }
    void setDbPath(string _p){ this->dbPath = _p; }
    void setSimulationSteps(uint _s){ this->simulationSteps = _s; }
    void setStepsPerEcho(uint _s){ this->stepsPerEcho = _s; }
    void setNumberOfEchoes(uint _n){ this->numberOfEchoes = _n; }
    void setInitialSeed(uint64_t _is){ this->initialSeed = _is; }
    void setSeedFlag(bool _v){ this->seedFlag = _v; }
    void setGpuUsage(bool _v){ this->gpuUsage = _v; }
    void setNumberOfPores(uint _np){ this->numberOfPores = _np; }
    void setPorosity(double _v){ this->porosity = _v; }
    void setInterfacePoreMatrix(uint _v){ this->interfacePoreMatrix = _v; }
    void setSvpRatio(double _r){ this->svpRatio = _r; }
    void setWalkerOccupancy(double _wo){ this->walkerOccupancy = _wo; }
    void setNumberOfWalkers(uint _nw){ this->numberOfWalkers = _nw; }
    void setWalkerSamples(uint _ws){ this->walkerSamples = _ws; }
    void setPores(vector<Point3D> _p){ this->pores = _p; }
    void setPores(Point3D _p, uint _idx){ this->pores[_idx] = _p; }
    void addPores(Point3D _p){ this->pores.push_back(_p); }
    void clearPores(){ this->pores.clear(); }
    void setWalkersIdxList(vector<uint> _wid){ this->walkersIdxList = _wid; }
    void setWalkersIdxList(uint _wid, uint _idx){ this->walkersIdxList[_idx] = _wid; }
    void addWalkerIdxList(uint _wid){ this->walkersIdxList.push_back(_wid); }
    void clearWalkerIdxList(){ this->walkersIdxList.clear(); }    
    void setWalkers(vector<Walker> *_w){ this->walkers = _w; }
    void setWalkers(Walker _w, uint _idx){ (*this->walkers)[_idx] = _w; }
    void addWalkers(Walker _w){ this->walkers->push_back(_w); }
    void clearWalkers(){ this->walkers->clear(); }
    void setTimeInterval(double _t){ this->timeInterval = _t; }
    void setDiffusionCoefficient(double _d){ this->diffusionCoefficient = _d; }
    void setGiromagneticRatio(double _g){ this->giromagneticRatio = _g; }
    void setBulkRelaxationTime(double _bt){ this->bulkRelaxationTime = _bt; }
    void setImagePath(ImagePath _p){ this->imagePath = _p; }
    void setNumberOfImages(uint _n){ this->numberOfImages = _n; }
    void setImageResolution(double _r){ this->imageResolution = _r; }
    void setImageVoxelResolution(double _vr){ this->imageVoxelResolution = _vr; }
    void setHeight(uint _h){ this->height = _h; }
    void setWidth(uint _w){ this->width = _w; }
    void setDepth(uint _d){ this->depth = _d; }
    void setBinaryMap(vector<Mat> _bm){ this->binaryMap = _bm; }
    void setBinaryMap(Mat _bm, uint _idx){ this->binaryMap[_idx] = _bm; }
    void addBinaryMap(Mat _bm){ this->binaryMap.push_back(_bm); }
    void clearBinaryMap(){ this->binaryMap.clear(); }
    void setBitBlock(BitBlock *_bb){ this->bitBlock = _bb; }
    void setBoundaryCondition(string _bc){ this->boundaryCondition = _bc; }
    void setVoxelDivision(int _v){ this->voxelDivision = _v; }
    void setVoxelDivisionApplied(bool _b){ this->voxelDivisionApplied = _b; }
    void setHistogram(CollisionHistogram _h){ this->histogram = _h; }
    void setHistogramList(vector<CollisionHistogram> _hl){ this->histogramList = _hl; }
    void setHistogramList(CollisionHistogram _hl, uint _idx){ this->histogramList[_idx] = _hl; }
    void addHistogramList(CollisionHistogram _hl){ this->histogramList.push_back(_hl); }
    void clearHistogramList(){ this->histogramList.clear(); }
    

    // Get methods
    rwnmr_config getRWNMRConfig(){ return this->rwNMR_config; }
    uct_config getUCTConfig(){ return this->uCT_config; }
    string getName(){ return this->name; }
    string getDir(){ return this->dir; }
    string getDbPath(){ return this->dbPath; }
    uint getSimulationSteps(){ return this->simulationSteps; }
    uint getStepsPerEcho(){ return this->stepsPerEcho; }
    uint getNumberOfEchoes(){ return this->numberOfEchoes; }
    uint64_t getInitialSeed(){ return this->initialSeed; }
    bool getSeedFlag(){ return this->seedFlag; }
    bool getGpuUsage(){ return this->gpuUsage; }
    uint getNumberOfPores(){ return this->numberOfPores; }
    double getPorosity(){ return this->porosity; }
    uint getInterfacePoreMatrix(){ return this->interfacePoreMatrix; }
    double getSvpRatio(){ return this->svpRatio; }
    double getWalkerOccupancy(){ return this->walkerOccupancy; }
    uint getNumberOfWalkers(){ return this->numberOfWalkers; }
    uint getWalkerSamples(){ return this->walkerSamples; }
    vector<Point3D> getPores(){ return this->pores; }
    Point3D getPores(uint idx){ return this->pores[idx]; }
    vector<uint> getWalkersIdxList(){ return this->walkersIdxList; }
    uint getWalkersIdxList(uint idx){ return this->walkersIdxList[idx]; }
    vector<Walker> *getWalkers(){ return this->walkers; }
    Walker getWalkers(uint idx){ return (*this->walkers)[idx]; }
    double getTimeInterval(){ return this->timeInterval; }
    double getDiffusionCoefficient(){ return this->diffusionCoefficient; }
    double getGiromagneticRatio(){ return this->giromagneticRatio; }
    double getBulkRelaxationTime(){ return this->bulkRelaxationTime; }
    ImagePath getImagePath(){ return this->imagePath; }
    uint getNumberOfImages(){ return this->numberOfImages; }
    double getImageResolution(){ return this->imageResolution; }
    double getImageVoxelResolution(){ return this->imageVoxelResolution; }  
    uint getHeight(){ return this->height; }
    uint getWidth(){ return this->width; }
    uint getDepth(){ return this->depth; }
    vector<Mat> getBinaryMap(){ return this->binaryMap; }
    Mat getBinaryMap(uint idx){ return this->binaryMap[idx]; }
    BitBlock *getBitBlock(){ return this->bitBlock; }
    string getBoundaryCondition(){ return this->boundaryCondition; }
    int getVoxelDivision(){ return this->voxelDivision; }
    bool getVoxelDivisionApplied(){ return this->voxelDivisionApplied; }
    CollisionHistogram getHistogram(){ return this->histogram; }
    vector<CollisionHistogram> getHistogramList(){ return this->histogramList; }
    CollisionHistogram getHistogramList(uint idx){ return this->histogramList[idx]; }

    // Class methods:
    // read
    void readImage();
    void loadRockImage();
    void loadRockImageFromList();
    void createBinaryMap(Mat &_rockImage, uint slice);
    void createBitBlockMap();
    void initSeed(bool _flag=false);
    void initGiromagneticRatio(double _gamma, string _unit = "rad");
    void initImageVoxelResolution();
    void initTimeInterval();
    void initVoxelDivision(uint _shifts);
    void applyVoxelDivision(uint _shifts);
    void initNumberOfStepsFromTime(double time);
    void buildTimeFramework(uint _steps);
    void buildTimeFramework(double _time);
    void initWalkers(void);
    void initWalkers(Point3D _point, uint _numberOfWalkers);
    void initWalkers(Point3D _point1, Point3D _point2, uint _numberOfWalkers);
    void initWalkers(uint _numberOfWalkers, bool _randomInsertion = false);
    void countPoresInBinaryMap();
    void countPoresInBitBlock();
    void countPoresInCubicSpace(Point3D _vertex1, Point3D _vertex2);
    void updatePorosity();
    void countInterfacePoreMatrix();
    void updateSvpRatio();
    void updateNumberOfPores();
    void createPoreList();
    void createPoreList(Point3D _vertex1, Point3D _vertex2);
    void freePoreList(){ if(this->pores.size() > 0) this->pores.clear();}
    void updateWalkerOccupancy();
    void createWalkersIdxList();
    uint removeRandomIndexFromPool(vector<uint> &_pool, uint _randomIndex);
    void createWalkers();
    void placeWalkers();
    void placeWalkersUniformly();
    void placeWalkersByChance();
    void placeWalkersInSamePoint(uint x = 0, uint y = 0, uint z = 0);
    void placeWalkersInCubicSpace(Point3D _vertex1, Point3D _vertex2);

    // histogram
    void initHistogramList();
    void createHistogram();
    void createHistogram(uint histID, uint _steps);
    void updateHistogram();

    // cost function methods
    void updateWalkersRelaxativity(vector<double> &parameters);
    void updateWalkersRelaxativity(double rho);

    typedef void (Model::*mapSim_ptr)(bool);
    typedef void (Model::*walkSim_ptr)();

private:
    mapSim_ptr mapSimulationPointer;

    /* 
        RW collision mapping simulations
        CUDA/GPU routines generate collision histograms
    */ 
    void mapSimulation_OMP(bool reset=true);
    void mapSimulation_CUDA_2D_histograms(bool reset=true);
    void mapSimulation_CUDA_3D_histograms(bool reset=true);


public:
    void mapSimulation(bool reset=true)
    {
        (this->*mapSimulationPointer)(reset);
    }
    void associateMapSimulation();

    // Class supermethod:
    void saveInfo(string _dir);
    void save();
    void save(string _otherDir);

    uint pickRandomIndex(uint _minValue, uint _maxValue);
    Point3D removeRandomPore(vector<Point3D> &_pores, uint _randomIndex);

    void printDetails();
    void info();
    void dummy(){ cout << "hey, I'm here dude." << endl;}

    inline string convertFileIDToString(uint id, uint digits)
    {
        stringstream result;
        result << std::setfill('0') << std::setw(digits) << id;
        return result.str();
    }

    // output generation class methods
    string createDirectoryForResults(string _root);
    void saveImageInfo(string filedir);
    void saveWalkers(string filedir);
    void saveBitBlock(string filedir);
    void saveHistogram(string filedir);
    void saveHistogramList(string filedir);    
    void assemblyImagePath();

    void reset()
    {
        if (this->walkers->size() > 0)
        {
            this->walkers->clear();
        }
    }

    void clear()
    {
        // RW simulation parameters
        this->numberOfPores = 0;
        this->numberOfWalkers = 0;

        // vector objects
        this->pores.clear();
        this->walkers->clear();

        // image attributes
        this->binaryMap.clear();
        this->bitBlock->clear();

        // histogram used in fast simulations
        this->histogram.clear();
        this->histogramList.clear();
    }
};

#endif