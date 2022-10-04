#ifndef NMR_CPMG_H
#define NMR_CPMG_H

#include "Defs.h"
#include "Model.h"
#include "InternalField.h"
#include "Walker.h"

class NMR_cpmg
{
private:
	Model &model;
	cpmg_config CPMG_config;
	string name;
	string dir;
    double exposureTime;
    bool applyBulkRelaxation;
    string method;
    vector<double> signalAmps;
    vector<double> signalTimes;
    vector<double> T2bins;
    vector<double> T2amps;
    vector<double> noise;
    double *penalties;
    InternalField *internalField;
    
public:
	NMR_cpmg(Model &_model, cpmg_config _cpmgConfig, int _mpi_rank = 0, int _mpi_processes = 0);	
	virtual ~NMR_cpmg()
	{
		if(this->penalties != NULL)
        {
            delete[] this->penalties;
            this->penalties = NULL;
        }

        if(this->internalField != NULL)
        {
        	delete this->internalField;
        	this->internalField = NULL;
        }
	};

    // -- Get methods
    Model &getModel(){ return this->model; }
    cpmg_config getCPMGConfig(){ return this->CPMG_config; }
    string getName(){return this->name;}
    string getDir(){return this->dir;}
    double getExposureTime() { return this->exposureTime; }
	bool getApplyBulkRelaxation(){ return this->applyBulkRelaxation; }
    string getMethod() { return this->method; }
    vector<double> getSignalAmps() { return this->signalAmps; }
    double getSignalAmps(uint idx) { return this->signalAmps[idx]; }
    vector<double> getSignalTimes() { return this->signalTimes; }
    double getSignalTimes(uint idx) { return this->signalTimes[idx]; }
    vector<double> getT2bins() { return this->T2bins; }
    double getT2bins(uint idx) { return this->T2bins[idx]; }
    vector<double> getT2amps() { return this->T2amps; }
    double getT2amps(uint idx) { return this->T2amps[idx]; }
    vector<double> getNoise() { return this->noise; }
    double getNoise(uint idx) { return this->noise[idx]; }
    double *getPenalties(){ return this->penalties; }
    InternalField *getInternalField() {return this->internalField; }
    double *getInternalFieldData() { return (this->internalField == NULL) ? NULL : this->internalField->getData(); }
    double getInternalFieldSize() { return (this->internalField == NULL) ? 0 : this->internalField->getSize(); }

// -- Set methods
    void setModel(Model &_model){ this->model = _model; }
	void setPFGSE_config(cpmg_config _config){ this->CPMG_config = _config;}
	void setName(string _name){ this->name = _name; }
	void setDir(string _dir){ this->dir = _dir; }
	void setExposureTime(double _value){ this->exposureTime = _value; }
	void setApplyBulkRelaxation(bool _bulk){ this->applyBulkRelaxation = _bulk; }
    void setMethod(string _method){ this->method = _method; }
    void setSignalAmps(vector<double> _vec){ this->signalAmps = _vec; }
    void setSignalAmps(double _v, uint idx){ this->signalAmps[idx] = _v; }
    void addSignalAmps(double _v){ this->signalAmps.push_back(_v); }
    void clearSignalAmps(double _v){ this->signalAmps.clear(); }
    void setSignalTimes(vector<double> _vec){ this->signalTimes = _vec; }
    void setSignalTimes(double _v, uint idx){ this->signalTimes[idx] = _v; }
    void addSignalTimes(double _v){ this->signalTimes.push_back(_v); }
    void clearSignalTimes(double _v){ this->signalTimes.clear(); }
    void setT2bins(vector<double> _vec){ this->T2bins = _vec; }
    void setT2bins(double _v, uint idx){ this->T2bins[idx] = _v; }
    void addT2bins(double _v){ this->T2bins.push_back(_v); }
    void clearT2bins(double _v){ this->T2bins.clear(); }
    void setT2amps(vector<double> _vec){ this->T2amps = _vec; }
    void setT2amps(double _v, uint idx){ this->T2amps[idx] = _v; }
    void addT2amps(double _v){ this->T2amps.push_back(_v); }
    void clearT2amps(double _v){ this->T2amps.clear(); }
    void setNoise(vector<double> _vec){ this->noise = _vec; }
    void setNoise(double _v, uint idx){ this->noise[idx] = _v; }
    void addNoise(double _v){ this->noise.push_back(_v); }
    void clearNoise(double _v){ this->noise.clear(); }
    void setPenalties(double *_p){ this->penalties = _p; }
    void setInternalField(InternalField *_f){ this->internalField = _f; }
    
    
    void buildInternalField(string _mode);
    void checkInternalField();
    
    // -- Essentials
	void buildModelTimeFramework(bool map = true);
    void build();
    void run();
    void resetSignal();
    void normalizeSignal();
    void applyBulk();
    void applyLaplace();
	void save();
	void writeWalkers();
	void writeHistogram();
	void writeHistogramList();
	void writeT2decay();
	void writeT2dist();
	void buildName(string parent = "", string sufix = "");
	void createDirectoryForData();

    // -- Simulations
    void run_simulation();
    void image_simulation_cuda();
	void image_simulation_omp();
	void createPenaltiesVector(vector<double> &_sigmoid);
    void createPenaltiesVector(double rho);
    void histogram_simulation();
	

private:
	int mpi_rank;
	int mpi_processes;
};

#endif