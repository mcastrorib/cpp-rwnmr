#ifndef PFGSE_CONFIG_H_
#define PFGSE_CONFIG_H_

#include "configFiles_defs.h"
#include "BaseConfig.h"
#include "../math/Math.h"


using namespace std;

class PfgseConfig : public BaseConfig
{
private:
    // --- Physical attributes.
    bool APPLY_BULK;
    double PULSE_WIDTH;
    Vector3d MAX_GRADIENT;
    uint GRADIENT_SAMPLES;

    // --- Time sequence 
    string TIME_SEQ;
    uint TIME_SAMPLES;
    vector<double> TIME_VALUES;
    double TIME_MIN;
    double TIME_MAX;
    bool APPLY_SCALE_FACTOR;
    double INSPECTION_LENGTH;

    // --- Threshold application for D(t) recovering.
    double NOISE_AMP;
    double TARGET_SNR;
    string THRESHOLD_TYPE;
    double THRESHOLD_VALUE;
    uint THRESHOLD_WINDOW;

    // --- Some useful flags.
    bool ALLOW_WALKER_SAMPLING;
    bool APPLY_ABSORPTION;

    // --- PFGSE SAVE. 
    bool SAVE_MODE;
    bool SAVE_PFGSE;
    bool SAVE_WALKERS;
    bool SAVE_HISTOGRAM;
    bool SAVE_HISTOGRAM_LIST;

public:
    // default constructors
    PfgseConfig() : BaseConfig(), 
                    APPLY_BULK(false), 
                    APPLY_SCALE_FACTOR(false), 
                    ALLOW_WALKER_SAMPLING(true), 
                    APPLY_ABSORPTION(false), 
                    SAVE_MODE(false), 
                    THRESHOLD_WINDOW(1), 
                    NOISE_AMP(0.0), 
                    TARGET_SNR(-1.0) {};
    PfgseConfig(const string configFile, const string croot);

    //copy constructors
    PfgseConfig(const PfgseConfig &otherConfig);

    // default destructor
    virtual ~PfgseConfig(){} 

    vector<string> checkConfig();
    void readConfigFile(const string configFile);
    void createTimeSamples();
    
    // -- Read methods
    void readApplyBulk(string s);
    void readPulseWidth(string s);
    void readMaxGradient(string s);
    void readGradientSamples(string s);
    void readTimeSequence(string s);
    void readTimeSamples(string s); 
    void readTimeValues(string s);
    void readTimeMin(string s);
    void readTimeMax(string s);
    void readApplyScaleFactor(string s);
    void readInspectionLength(string s);
    void readNoiseAmp(string s);
    void readTargetSNR(string s);
    void readThresholdType(string s);
    void readThresholdValue(string s);
    void readThresholdWindow(string s);
    void readAllowWalkerSampling(string s);
    void readApplyAbsorption(string s);
    void readSaveMode(string s);
    void readSavePFGSE(string s);
    void readSaveWalkers(string s);
    void readSaveHistogram(string s);
    void readSaveHistogramList(string s);  

    // -- Read methods
    void setApplyBulk(bool s){this->APPLY_BULK = s;}
    void setPulseWidth(double s){this->PULSE_WIDTH = s;}
    void setMaxGradient(Vector3d s){this->MAX_GRADIENT = s;}
    void setGradientSamples(uint s){this->GRADIENT_SAMPLES = s;}
    void setTimeSequence(string s){this->TIME_SEQ = s;}
    void setTimeSamples(uint s){this->TIME_SAMPLES = s;}
    void setTimeValues(vector<double> s){this->TIME_VALUES = s;}
    void setTimeMin(double s){this->TIME_MIN = s;}
    void setTimeMax(double s){this->TIME_MAX = s;}
    void setApplyScaleFactor(bool s){this->APPLY_SCALE_FACTOR = s;}
    void setInspectionLength(double s){this->INSPECTION_LENGTH = s;}
    void setNoiseAmp(double s){this->NOISE_AMP = s;}
    void setTargetSNR(double s){this->TARGET_SNR = s;}
    void setThresholdType(string s){this->THRESHOLD_TYPE = s;}
    void setThresholdValue(double s){this->THRESHOLD_VALUE = s;}
    void setThresholdWindow(uint s){this->THRESHOLD_WINDOW = s;}
    void setAllowWalkerSampling(bool s){this->ALLOW_WALKER_SAMPLING = s;}
    void setApplyAbsorption(bool s){this->APPLY_ABSORPTION = s;}
    void setSaveMode(bool _v){ this->SAVE_MODE = _v;}
    void setSavePFGSE(bool _v){ this->SAVE_PFGSE = _v;}
    void setSaveWalkers(bool _v){ this->SAVE_WALKERS = _v;}
    void setSaveHistogram(bool _v){ this->SAVE_HISTOGRAM = _v;}
    void setSaveHistogramList(bool _v){ this->SAVE_HISTOGRAM_LIST = _v;} 

    // -- Get methods
    bool getApplyBulk(){ return this->APPLY_BULK; }
    double getPulseWidth() { return this->PULSE_WIDTH ; }
    Vector3d getMaxGradient() { return this->MAX_GRADIENT ; }
    uint getGradientSamples() { return this->GRADIENT_SAMPLES ; }
    string getTimeSequence() { return this->TIME_SEQ ; }
    uint getTimeSamples() { return this->TIME_SAMPLES ; } 
    vector<double> getTimeValues() { return this->TIME_VALUES ; }
    double getTimeMin() { return this->TIME_MIN; }
    double getTimeMax() { return this->TIME_MAX; }
    bool getApplyScaleFactor() { return this->APPLY_SCALE_FACTOR; }
    double getInspectionLength() { return this->INSPECTION_LENGTH; }
    double getNoiseAmp() { return this->NOISE_AMP; }
    double getTargetSNR() { return this->TARGET_SNR; }
    string getThresholdType() { return this->THRESHOLD_TYPE; }
    double getThresholdValue() { return this->THRESHOLD_VALUE; }
    uint getThresholdWindow() { return this->THRESHOLD_WINDOW; }
    bool getAllowWalkerSampling() { return this->ALLOW_WALKER_SAMPLING; }
    bool getApplyAbsorption() { return this->APPLY_ABSORPTION; }
    bool getSaveMode() { return this->SAVE_MODE; }
    bool getSavePFGSE() { return this->SAVE_PFGSE; }
    bool getSaveWalkers() { return this->SAVE_WALKERS; }
    bool getSaveHistogram() { return this->SAVE_HISTOGRAM; }
    bool getSaveHistogramList() { return this->SAVE_HISTOGRAM_LIST; }  

private:
    // Returns a vector<double> linearly space from @start to @end with @points
    vector<double> linspace(double start, double end, uint points);

    // Returns a vector<double> logarithmly space from 10^@exp_start to 10^@end with @points
    vector<double> logspace(double exp_start, double exp_end, uint points, double base=10.0);
    
};

#endif