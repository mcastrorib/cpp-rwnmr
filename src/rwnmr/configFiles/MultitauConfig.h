#ifndef MULTITAU_CONFIG_H_
#define MULTITAU_CONFIG_H_

#include "configFiles_defs.h"
#include "BaseConfig.h"

class MultitauConfig : public BaseConfig
{
private:
    double TAU_MIN;
    double TAU_MAX;
    int TAU_POINTS;
    vector<double> TAU_VALUES;
    string TAU_SCALE;
    bool COMPLETE_DECAY;

    // --- cpmg SAVE. 
    bool SAVE_MODE;
    bool SAVE_DECAY;
    bool SAVE_WALKERS;
    bool SAVE_HISTOGRAM;
    bool SAVE_HISTOGRAM_LIST;

public:   
    // default constructors
    MultitauConfig():BaseConfig(),SAVE_MODE(false){};
    MultitauConfig(const string configFile, const string croot);

    //copy constructors
    MultitauConfig(const MultitauConfig &otherConfig);

    // default destructor
    virtual ~MultitauConfig(){} 

    vector<string> checkConfig();
    void readConfigFile(const string configFile);
    
    // -- Read methods
    void readTauMin(string s);
    void readTauMax(string s);
    void readTauPoints(string s);
    void readTauScale(string s);
    void readTauValues(string s);
    void readCompleteDecay(string s);
    void readSaveMode(string s);
    void readSaveDecay(string s);
    void readSaveWalkers(string s);
    void readSaveHistogram(string s);
    void readSaveHistogramList(string s);   

    // -- Set methods
    void setTauMin(double s){ this->TAU_MIN = s; }
    void setTauMax(double s){ this->TAU_MAX = s; }
    void setTauPoints(int s){ this->TAU_POINTS = s;}
    void setTauValues(vector<double> s){ this->TAU_VALUES = s;}
    void setTauScale(string s){ this->TAU_SCALE = s; }
    void setCompleteDecay(bool s){ this->COMPLETE_DECAY = s;}
    void setSaveMode(bool _v){ this->SAVE_MODE = _v;}
    void setSaveDecay(bool _v){ this->SAVE_DECAY = _v;}
    void setSaveWalkers(bool _v){ this->SAVE_WALKERS = _v;}
    void setSaveHistogram(bool _v){ this->SAVE_HISTOGRAM = _v;}
    void setSaveHistogramList(bool _v){ this->SAVE_HISTOGRAM_LIST = _v;} 
    
    // -- Get methods
    double getTauMin(){ return this->TAU_MIN; }
    double getTauMax(){ return this->TAU_MAX; }
    int getTauPoints(){ return this->TAU_POINTS; }
    vector<double> getTauValues(){ return this->TAU_VALUES; }
    string getTauScale(){ return this->TAU_SCALE; }
    bool getCompleteDecay(){ return this->COMPLETE_DECAY; }
    bool getSaveMode(){return this->SAVE_MODE; }
    bool getSaveDecay() {return this->SAVE_DECAY; }
    bool getSaveWalkers(){return this->SAVE_WALKERS; }
    bool getSaveHistogram() {return this->SAVE_HISTOGRAM; }
    bool getSaveHistogramList() {return this->SAVE_HISTOGRAM_LIST; }  
};

#endif