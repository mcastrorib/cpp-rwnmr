#ifndef NMR_MULTITAU_H
#define NMR_MULTITAU_H

// include configuration file classes
#include "Defs.h"
#include "Model.h"
#include "NMR_cpmg.h"

class NMR_multitau
{
private:
	Model &model;
	NMR_cpmg *cpmg;
	MultitauConfig MultiTau_config;
	CpmgConfig CPMG_config;
	string name;
	string dir;
	vector<uint> requiredSteps;
    vector<double> signalTimes;
    vector<double> signalAmps;

public:
	NMR_multitau(Model &_model, MultitauConfig _multitauConfig, CpmgConfig _CpmgConfig);	

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
	void setMultitauConfig(MultitauConfig _config){ this->MultiTau_config = _config; }
	void setCpmgConfig(CpmgConfig _config){ this->CPMG_config = _config; }
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
	MultitauConfig getMultitauConfig(){ return this->MultiTau_config; }
	CpmgConfig getCpmgConfig(){ return this->CPMG_config; }
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
};

#endif