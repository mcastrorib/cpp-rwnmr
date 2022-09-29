#ifndef COLLISION_HISTOGRAM_H_
#define COLLISION_HISTOGRAM_H_

#include "Defs.h"
#include "Walker.h"

class CollisionHistogram
{
private:	
	int size;
	string scale;
	double gap;
	vector<double> amps;
	vector<double> bins;
	int firstEcho;
	int lastEcho;
	bool isSet;
	
public:
	CollisionHistogram();
	CollisionHistogram(int _size, string _scale);
	CollisionHistogram(const CollisionHistogram &_otherHistogram);
	virtual ~CollisionHistogram(){}

	void clear()
	{
		this->size = 0;
		this->gap = 0;
		this->amps.clear();
		this->bins.clear();
		this->isSet = false;
	}

	void setSize(int _size) { this->size = _size; }
	void setScale(string _scale) { this->scale = _scale; }
	void setGap(double _gap) { this->gap = _gap; }
	void setAmps(vector<double> _amps){ this->amps = _amps;}
	void setBins(vector<double> _bins){ this->bins = _bins;}
	void setFirstEcho(int _firstEcho) { this->firstEcho = _firstEcho;}
	void setLastEcho(int _lastEcho) { this->lastEcho = _lastEcho;}
	void reserveAmps(int _size){ this->amps.reserve(_size);}
	void reserveBins(int _size){ this->bins.reserve(_size);}
	void addAmp(double item){ this->amps.push_back(item);}
	void addBin(double item){ this->bins.push_back(item);}
	void setAmp(double item, int idx){ if(idx < this->amps.size()) this->amps[idx] = item; }
	void setBin(double item, int idx){ if(idx < this->bins.size()) this->bins[idx] = item; }
	void clearAmps(){ this->amps.clear();}
	void clearBins(){ this->bins.clear();}	

	int getSize(){return this->size;}
	string getScale(){return this->scale;}
	double getGap(){return this->gap;}
	vector<double> getAmps(){return this->amps;}
	vector<double> getBins(){return this->bins;}
	double getAmp(int idx){ return this->amps[idx]; }
	double getBin(int idx){ return this->bins[idx]; }
	int getFirstEcho(){return this->firstEcho;}
	int getLastEcho(){return this->lastEcho;}
	bool getIsSet(){return this->isSet;}

	void setIsSet(bool _set){this->isSet = _set;}
	void createBlankHistogram(int _size, string scale);
	void fillHistogram(vector<Walker> &_walkers, uint _numberOfSteps);
	void updateHistogram(vector<Walker> &_walkers, uint _numberOfSteps);
	void createBinsLinearVector(vector<Walker> &_walkers);
	void createAmpsLinearVector(vector<Walker> &_walkers, uint _numberOfSteps);
	void createBinsLogVector(vector<Walker> &_walkers, uint _numberOfSteps);
	void createAmpsLogVector(vector<Walker> &_walkers, uint _numberOfSteps);
};

#endif