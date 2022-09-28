#ifndef NMR_MULTITAU_H
#define NMR_MULTITAU_H

// include configuration file classes
#include "Defs.h"
#include "Model.h"
#include "NMR_cpmg.h"

using namespace std;

class NMR_multitau
{
private:
	Model &model;
	NMR_cpmg *cpmg;
	multitau_config MultiTau_config;
	cpmg_config CPMG_config;
	string name;
	string dir;
	vector<uint> requiredSteps;
    vector<double> signalTimes;
    vector<double> signalAmps;

public:
	NMR_multitau(Model &_model, 
				 multitau_config _multitauConfig, 
				 cpmg_config _cpmgConfig, 
				 int _mpi_rank = 0, 
				 int _mpi_processes = 0);	

	virtual ~NMR_multitau()
	{
		if(this->cpmg != NULL)
		{
			delete this->cpmg;
        	this->cpmg = NULL;
		}
	};

	void setModel(Model &_model){ this->model = _model; }
	void setCPMG(NMR_cpmg *_cpmg){ this->cpmg = _cpmg; }
	void setMultitauConfig(multitau_config _config){ this->MultiTau_config = _config; }
	void setCPMGConfig(cpmg_config _config){ this->CPMG_config = _config; }
	void setName(string _name){ this->name = _name; }
	void setDir(string _dir){ this->dir = _dir; }
	void setRequiredSteps(vector<uint> _vec){ this->requiredSteps = _vec; }
	void setSignalTimes(vector<double> _vec){ this->signalTimes = _vec; }
	void setSignalAmps(vector<double> _vec){ this->signalAmps = _vec; }
	void setRequiredStep(uint _val, uint idx){ this->requiredSteps[idx] = _val; }
	void setSignalTime(double _val, uint idx){ this->signalTimes[idx] = _val; }
	void setSignalAmp(double _val, uint idx){ this->signalAmps[idx] = _val; }
	void addRequiredStep(uint _val){ this->requiredSteps.push_back(_val); }
	void addSignalTime(double _val){ this->signalTimes.push_back(_val); }
	void addSignalAmp(double _val){ this->signalAmps.push_back(_val); }
	void clearRequiredSteps(){ this->requiredSteps.clear(); }
	void clearSignalTimes(){ this->signalTimes.clear(); }
	void clearSignalAmps(){ this->signalAmps.clear(); }

	Model &getModel(){return this->model;}
	NMR_cpmg *getCPMG(){ return this->cpmg; }
	multitau_config getMultitauConfig(){ return this->MultiTau_config; }
	cpmg_config getCPMGConfig(){ return this->CPMG_config; }
	string getName(){ return this->name; }
	string getDir(){ return this->dir; }
	vector<uint> getRequiredSteps(){ return this->requiredSteps; }
	uint getRequiredStep(uint idx){ return this->requiredSteps[idx]; }
    vector<double> getSignalTimes(){ return this->signalTimes; }
	double getSignalTime(uint idx){ return this->signalTimes[idx]; }
    vector<double> getSignalAmps(){ return this->signalAmps; }
	double getSignalAmp(uint idx){ return this->signalAmps[idx]; }

	void createName();
	void createDirectoryForData();
	void setTauSequence();
	void setExposureTime(uint index);
	void setCPMG(uint index);
	void runCPMG();
	void saveCPMG();
	void run();
	void save();
	void writeDecay();
	void writeWalkers();
	void writeHistogram();
	void writeHistogramList();

private:
	int mpi_rank;
	int mpi_processes;

	// Returns a vector<double> linearly space from @start to @end with @points
    vector<double> linspace(double start, double end, uint points);

    // Returns a vector<double> logarithmly space from 10^@exp_start to 10^@end with @points
    vector<double> logspace(double exp_start, double exp_end, uint points, double base=10.0);

    // Returns the sum of elements of a vector
    int sum(vector<int> _vec);
    uint sum(vector<uint> _vec);
    double sum(vector<double> _vec);
};

#endif