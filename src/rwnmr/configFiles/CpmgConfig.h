#ifndef CPMG_CONFIG_H_
#define CPMG_CONFIG_H_

#include "configFiles_defs.h"
#include "BaseConfig.h"

using namespace std;

class CpmgConfig : public BaseConfig
{
private:
    // --- Physical attributes.
    bool APPLY_BULK;
    double OBS_TIME;
    string METHOD;
    bool TIME_VERBOSE;

    // --- Diffusive relaxation.
    string RESIDUAL_FIELD;
    double GRADIENT_VALUE;
    int GRADIENT_DIRECTION;
    string PATH_TO_FIELD;
    bool INTERPOLATE_FIELD;

    // LAPLACE INVERSION
    double MIN_T2;
    double MAX_T2;
    bool USE_T2_LOGSPACE; 
    int NUM_T2_BINS;
    double MIN_LAMBDA;
    double MAX_LAMBDA;
    int NUM_LAMBDAS;
    int PRUNE_NUM;
    double NOISE_AMP;

    // --- cpmg SAVE. 
    bool SAVE_MODE;
    bool SAVE_DECAY;
    bool SAVE_T2;
    bool SAVE_WALKERS;
    bool SAVE_HISTOGRAM;
    bool SAVE_HISTOGRAM_LIST;
    
public:
    // default constructors
    CpmgConfig():BaseConfig(), APPLY_BULK(false), TIME_VERBOSE(false), INTERPOLATE_FIELD(false), USE_T2_LOGSPACE(false), SAVE_MODE(false){};
    CpmgConfig(const string configFile, const string croot);

    //copy constructors
    CpmgConfig(const CpmgConfig &otherConfig);

    // default destructor
    virtual ~CpmgConfig()
    {
        // cout << "OMPLoopEnabler object destroyed succesfully" << endl;
    } 

    vector<string> checkConfig();
    void readConfigFile(const string configFile);
    
    // -- Read methods
    void readApplyBulk(string s);
    void readTimeVerbose(string s);
    void readObservationTime(string s);
    void readMethod(string s);
    void readResidualField(string s);
    void readGradientValue(string s);
    void readGradientDirection(string s);
    void readPathToField(string s);
    void readInterpolateField(string s);
    void readMinT2(string s);
    void readMaxT2(string s);
    void readUseT2Logspace(string s);
    void readNumT2Bins(string s);
    void readMinLambda(string s);
    void readMaxLambda(string s);
    void readNumLambdas(string s);
    void readPruneNum(string s);
    void readNoiseAmp(string s); 
    void readSaveMode(string s);
    void readSaveWalkers(string s);
    void readSaveDecay(string s);
    void readSaveHistogram(string s);
    void readSaveHistogramList(string s);   
    void readSaveT2(string s);   

    // set methods
    void setApplyBulk(bool _v){ this->APPLY_BULK = _v; }
    void setTimeVerbose(bool _v){ this->TIME_VERBOSE = _v; }
    void setObservationTime(double _v){ this->OBS_TIME = _v; }
    void setMethod(string _m){ this->METHOD = _m; }
    void setResidualField(string _s){ this->RESIDUAL_FIELD = _s; }
    void setGradientValue(double _v){ this->GRADIENT_VALUE = _v; }
    void setGradientDirection(int _v){ this->GRADIENT_DIRECTION = _v; }
    void setPathToField(string _s){ this->PATH_TO_FIELD = _s; }
    void setInterpolateField(bool _b){ this->INTERPOLATE_FIELD = _b; }
    void setMinT2(double _v){ this->MIN_T2 = _v; }
    void setMaxT2(double _v){ this->MAX_T2 = _v; }
    void setUseT2Logspace(bool _v){ this->USE_T2_LOGSPACE = _v; }
    void setNumT2Bins(int _v){ this->NUM_T2_BINS = _v;}
    void setMinLambda(double _v){ this->MIN_LAMBDA = _v;}
    void setMaxLambda(double _v){ this->MAX_LAMBDA = _v;}
    void setNumLambdas(int _v){ this->NUM_LAMBDAS = _v;}
    void setPruneNum(int _v){ this->PRUNE_NUM = _v;}
    void setNoiseAmp(double _v){ this->NOISE_AMP = _v;}
    void setSaveMode(bool _v){ this->SAVE_MODE = _v;}
    void setSaveDecay(bool _v){ this->SAVE_DECAY = _v;}
    void setSaveT2(bool _v){ this->SAVE_T2 = _v;}
    void setSaveWalkers(bool _v){ this->SAVE_WALKERS = _v;}
    void setSaveHistogram(bool _v){ this->SAVE_HISTOGRAM = _v;}
    void setSaveHistogramList(bool _v){ this->SAVE_HISTOGRAM_LIST = _v;} 
    

    // get methods
    bool getApplyBulk(){ return this->APPLY_BULK; }
    bool getTimeVerbose(){ return this->TIME_VERBOSE; }
    double getObservationTime(){return this->OBS_TIME; }
    string getMethod() { return this->METHOD; }

    string getResidualField() { return this->RESIDUAL_FIELD; }
    double getGradientValue() { return this->GRADIENT_VALUE; }
    int getGradientDirection() { return this->GRADIENT_DIRECTION; }
    string getPathToField() { return this->PATH_TO_FIELD; }
    bool getInterpolateField(){ return this->INTERPOLATE_FIELD; }

    double getMinT2(){ return this->MIN_T2; }
    double getMaxT2(){ return this->MAX_T2; }
    bool getUseT2Logspace(){ return this->USE_T2_LOGSPACE; }
    int getNumT2Bins(){ return this->NUM_T2_BINS; }
    double getMinLambda(){ return this->MIN_LAMBDA; }
    double getMaxLambda(){ return this->MAX_LAMBDA; }
    int getNumLambdas(){ return this->NUM_LAMBDAS; }
    int getPruneNum(){ return this->PRUNE_NUM; }
    double getNoiseAmp(){ return this->NOISE_AMP; }

    bool getSaveMode(){return this->SAVE_MODE; }
    bool getSaveWalkers(){return this->SAVE_WALKERS; }
    bool getSaveDecay() {return this->SAVE_DECAY; }
    bool getSaveHistogram() {return this->SAVE_HISTOGRAM; }
    bool getSaveHistogramList() {return this->SAVE_HISTOGRAM_LIST; }  
    bool getSaveT2() {return this->SAVE_T2; }  
};

#endif