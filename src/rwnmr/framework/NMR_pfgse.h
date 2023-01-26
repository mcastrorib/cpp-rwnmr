#ifndef NMR_PFGSE_H
#define NMR_PFGSE_H

#include "Defs.h"
#include "Model.h"
#include "Walker.h"

class NMR_PFGSE
{
private:
	Model &model;
	PfgseConfig PFGSE_config;
	string name;
	string dir;
	vector<double> gradient;
	double gradientMax;
	double noiseAmp;	
	double targetSNR;
	vector<double> rawNoise;
	double gradientX;
	double gradientY;
	double gradientZ;
	vector<Vector3d> vecGradient;
	vector<Vector3d> vecK;
	vector<double> rhs;	
	int gradientPoints;
	vector<uint> requiredSteps;
	vector<double> exposureTimes;
	double exposureTime;
	double pulseWidth;
	bool applyBulkRelaxation;
	vector<double> mkt;
	vector<double> mktStdev;
	vector<double> lhs;
	vector<double> lhsStdev;	
	int DsatAdjustSamples;
	double Dsat;
	double DsatError;
	double DsatStdev;
	double Dmsd;
	double DmsdStdev;
	double msd;
	double msdStdev;
	Vector3d vecMsd;
	Vector3d vecMsdStdev;	
	Vector3d vecDmsd;
	Vector3d vecDmsdStdev;
	uint stepsTaken;
	int currentTime;	

public:
	NMR_PFGSE(Model &_model, PfgseConfig _pfgseConfig);	
	
	virtual ~NMR_PFGSE(){};

	// Get methods
	Model &getModel(){ return this->model; }
	PfgseConfig getPfgseConfig(){ return this->PFGSE_config;}
	string getName(){ return this->name; }
	string getDir(){ return this->dir; }
	vector<double> getGradient(){ return this->gradient; }
	double getGradientMax(){ return this->gradientMax; }
	double getNoiseAmp(){ return this->noiseAmp; }
	double getTargetSNR(){ return this->targetSNR; }
	vector<double> getRawNoise(){ return this->rawNoise; }
	double getGradientX(){ return this->gradientX; }
	double getGradientY(){ return this->gradientY; }
	double getGradientZ(){ return this->gradientZ; }	
	vector<Vector3d> getVecGradient(){ return this->vecGradient; }
	vector<Vector3d> getVecK(){ return this->vecK; }
	vector<double> getRhs() {return this->rhs; }
	double getRhs(uint idx){ return this->rhs[idx];}
	vector<uint> getRequiredSteps(){ return this->requiredSteps; }
	uint getRequiredStep(uint idx){ return this->requiredSteps[idx]; }
	int getGradientPoints() { return this->gradientPoints; }
	vector<double> getExposureTimes() {return this->exposureTimes; }
	double getExposureTime(uint _idx) {return this->exposureTimes[_idx]; }
	double getExposureTime() {return this->exposureTime; }
	double getPulseWidth() {return this->pulseWidth; }
	bool getApplyBulkRelaxation() { return this->applyBulkRelaxation; }
	vector<double> getMkt() {return this->mkt; }
	double getMkt(uint idx) {return this->mkt[idx]; }
	vector<double> getMktStdev() {return this->mktStdev; }
	double getMktStdev(uint idx) {return this->mktStdev[idx]; }
	vector<double> getLhs() {return this->lhs; }
	double getLhs(uint idx) {return this->lhs[idx]; }
	vector<double> getLhsStdev() {return this->lhsStdev; }
	double getLhsStdev(uint idx) {return this->lhsStdev[idx]; }
	int getDsatAdjustSamples() { return this->DsatAdjustSamples; }
	double getDsat() { return this->Dsat; }
	double getDsatError() { return this->DsatError; }
	double getDsatStdev() { return this->DsatStdev; }
	double getDmsd() { return this->Dmsd; }
	double getDmsdStdev() { return this->DmsdStdev; }
	double getMsd() { return this->msd; }
	double getMsdStdev() { return this->msdStdev; }
	Vector3d getVecMsd() { return this->vecMsd; }
	Vector3d getVecMsdStdev() { return this->vecMsdStdev; }
	Vector3d getVecDmsd() { return this->vecDmsd; }
	Vector3d getVecDmsdStdev() { return this->vecDmsdStdev; }
	int getCurrentTime() { return this->currentTime; }

	// Set methods
	void setModel(Model &_model){ this->model = _model; }
	void setPfgseConfig(PfgseConfig _config){ this->PFGSE_config = _config;}
	void setName(string _name){ this->name = _name; }
	void setDir(string _dir){ this->dir = _dir; }
	void setGradient(vector<double> _vec){ this->gradient = _vec; }
	void setGradient(double _val, uint idx){ this->gradient[idx] = _val; }
	void addGradient(double _val){ this->gradient.push_back(_val); }
	void clearGradient(){ this->gradient.clear(); }
	void setGradientMax(double _val){ this->gradientMax = _val; }
	void setNoiseAmp(double _val){ this->noiseAmp = _val; }
	void setTargetSNR(double _val){ this->targetSNR = _val; }
	void setRawNoise(vector<double> _vec){ this->rawNoise = _vec; }
	void setRawNoise(double _val, uint idx){ this->rawNoise[idx] = _val; }
	void addRawNoise(double _val){ this->rawNoise.push_back(_val); }
	void clearRawNoise(){ this->rawNoise.clear(); }
	void setGradientX(double _v){ this->gradientX = _v; }
	void setGradientY(double _v){ this->gradientY = _v; }
	void setGradientZ(double _v){ this->gradientZ = _v; }
	void setVecGradient(vector<Vector3d> _vec){ this->vecGradient = _vec; }
	void setVecGradient(Vector3d _vec, uint idx){ this->vecGradient[idx] = _vec; }
	void addVecGradient(Vector3d _vec){ this->vecGradient.push_back(_vec); }
	void clearVecGradient(){ this->vecGradient.clear(); }
	void setVecK(vector<Vector3d> _vec){ this->vecK = _vec; }
	void setVecK(Vector3d _vec, uint idx){ this->vecK[idx] = _vec; }
	void addVecK(Vector3d _vec){ this->vecK.push_back(_vec); }
	void clearVecK(){ this->vecK.clear(); }
	void setRhs(vector<double> _vec){ this->rhs = _vec; }
	void setRhs(double _val, uint idx){ this->rhs[idx] = _val; }
	void addRhs(double _val){ this->rhs.push_back(_val); }
	void clearRhs(){ this->rhs.clear(); }
	void setGradientPoints(int _p){ this->gradientPoints = _p; }
	void setRequiredSteps(vector<uint> _vec){ this->requiredSteps = _vec; }
	void setRequiredSteps(uint _val, uint idx){ this->requiredSteps[idx] = _val; }
	void addRequiredSteps(uint _val){ this->requiredSteps.push_back(_val); }
	void clearRequiredSteps(){ this->exposureTimes.clear(); }
	void setExposureTimes(vector<double> _vec){ this->exposureTimes = _vec; }
	void setExposureTime(double _val, uint idx){ this->exposureTimes[idx] = _val; }
	void addExposureTime(double _val){ this->exposureTimes.push_back(_val); }
	void clearExposureTimes(){ this->exposureTimes.clear(); }
	void setExposureTime(double _value){ this->exposureTime = _value; }
	void setPulseWidth(double _value){ this->pulseWidth = _value; }
	void setApplyBulkRelaxation(bool _bulk) { this->applyBulkRelaxation = _bulk; }
	void setMkt(vector<double> _vec){ this->mkt = _vec; }
	void setMkt(double _val, uint idx){ this->mkt[idx] = _val; }
	void addMkt(double _val){ this->mkt.push_back(_val); }
	void clearMkt(){ this->mkt.clear(); }
	void setMktStdev(vector<double> _vec){ this->mktStdev = _vec; }
	void setMktStdev(double _val, uint idx){ this->mktStdev[idx] = _val; }
	void addMktStdev(double _val){ this->mktStdev.push_back(_val); }
	void clearMktStdev(){ this->mktStdev.clear(); }
	void setLhs(vector<double> _vec){ this->lhs = _vec; }
	void setLhs(double _val, uint idx){ this->lhs[idx] = _val; }
	void addLhs(double _val){ this->lhs.push_back(_val); }
	void clearLhs(){ this->lhs.clear(); }
	void setLhsStdev(vector<double> _vec){ this->lhsStdev = _vec; }
	void setLhsStdev(double _val, uint idx){ this->lhsStdev[idx] = _val; }
	void addLhsStdev(double _val){ this->lhsStdev.push_back(_val); }
	void clearLhsStdev(){ this->lhsStdev.clear(); }
	void setDsatAdjustSamples(int _v){ this->DsatAdjustSamples = _v; }
	void setDsat(double _value) { this->Dsat = _value; }
	void setDsatError(double _value) { this->DsatError = _value; }
	void setDsatStdev(double _value) { this->DsatStdev = _value; }
	void setDmsd(double _value) { this->Dmsd = _value; }
	void setDmsdStdev(double _value) { this->DmsdStdev = _value; }	
	void setMsd(double _value) { this->msd = _value; }
	void setMsdStdev(double _value) { this->msdStdev = _value; }
	void setVecMsd(Vector3d _vec){ this->vecMsd = _vec; }	
	void setVecMsd(double msdX, double msdY, double msdZ) 
	{
		this->vecMsd.setX(msdX);
		this->vecMsd.setY(msdY);
		this->vecMsd.setZ(msdZ);
	}
	void setVecMsdStdev(Vector3d _vec){ this->vecMsdStdev = _vec; }
	void setVecMsdStdev(double msdX_stdev, double msdY_stdev, double msdZ_stdev) 
	{
		this->vecMsdStdev.setX(msdX_stdev);
		this->vecMsdStdev.setY(msdY_stdev);
		this->vecMsdStdev.setZ(msdZ_stdev);
	}
	void setVecDmsd(Vector3d _vec){ this->vecDmsd = _vec; }
	void setVecDmsd(double DmsdX, double DmsdY, double DmsdZ) 
	{
		this->vecDmsd.setX(DmsdX);
		this->vecDmsd.setY(DmsdY);
		this->vecDmsd.setZ(DmsdZ);
	}
	void setVecDmsdStdev(Vector3d _vec){ this->vecDmsdStdev = _vec; }
	void setVecDmsdStdev(double DmsdX_stdev, double DmsdY_stdev, double DmsdZ_stdev) 
	{
		this->vecDmsdStdev.setX(DmsdX_stdev);
		this->vecDmsdStdev.setY(DmsdY_stdev);
		this->vecDmsdStdev.setZ(DmsdZ_stdev);
	}
	void setStepsTaken(uint _v){ this->stepsTaken = _v; }
	void setCurrentTime(int _v){ this->currentTime = _v; }

	void initExposureTimes();
	void buildModelTimeFramework();
	void correctExposureTimes();
	void runInitialMapSimulation();
	void buildGradientVector(double _GF, int _GPoints);
	void buildGradientVector();
	void createNoiseVector();
	vector<double> getNewNoiseVector(double _noiseAmp, uint _size = 0);
	double computeTargetNoiseAmp();
	double computeCurrentSNR();
	void buildVectorK();
	void buildVectorMkt();
	void buildVectorRHS();
	void buildThresholdFromLHS(double _value, uint _window=5);
    void buildThresholdFromLHSValue(double _value, uint _window=5);
    void buildThresholdFromLHSWindow(double _value, uint _window=5);
    void buildThresholdFromSamples(int _samples);
    void applyThreshold();
	double computeRHS_legacy(double _Gvalue);
	double computeRHS(double _kValue);
	void buildVectorLHS();
	double computeLHS(double _Mg, double _M0);
	double computeWaveVectorK(double gradientMagnitude, double pulse_width, double giromagneticRatio);
	double ** getSamplesMagnitude();
	double ** computeSamplesMagnitude();
	double ** computeSamplesMagnitudeWithOmp();
	double ** computeSamplesMagnitudeWithGpu();
	double ** computeSamplesNoise();
	void computeMktSmallPopulation(double **Mkt_samples, bool time_verbose);
	void computeMktSmallPopulation2(double **Mkt_samples, bool time_verbose);
	void computeMktSmallSamples(double **Mkt_samples, bool time_verbose);
	void computeMktBigSamples(double **Mkt_samples, bool time_verbose);
	void set();
	void run();
	void runSequence();
	void runSequenceWithoutSampling();
	void runSequenceWithSampling();
	void simulation();
	void applyBulk();
	void applyNoiseToSignal();
	void recoverD(string _method = "sat");
	void recoverDsat();
	void recoverDsatWithoutSampling();
	void recoverDsatWithSampling();
	void recoverDmsd();
	void recoverDmsdWithoutSampling();
	void recoverDmsdWithSampling();	
	void clear();
	void resetModel();
	void updateWalkersXiRate(uint _rwsteps);
	void reset(double _newBigDelta);
	void reset();
	void presave();
	void save();
	void writeResults();
	void writeParameters();
	void writeEchoes();
	void writeGvector();
	void writeMsd();
	void writeWalkers();
	void writeHistogram();
	void writeHistogramList();
	void setName();
	void createDirectoryForData();
	void createResultsFile();

	
	void resetCurrentTime() { this->currentTime = 0; }
	void incrementCurrentTime() { this->currentTime++; }

	void printTimeFramework()
	{
		for(uint i = 0; i<this->exposureTimes.size(); i++)
		{
			cout << "t[" << i << "] = " << this->exposureTimes[i] << " ms, ";
			cout << this->requiredSteps[i] << " rw-steps" << endl;
		}
	}

private:
	void simulation_cuda();
	void simulation_omp();
	vector<double> getNormalDistributionSamples(const double loc, const double std, const int size);
};

#endif