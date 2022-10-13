// include C++ standard libraries
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>

#include "cpmg_config.h"

using namespace std;

// default constructors
cpmg_config::cpmg_config(const string configFile, 
                         const string croot) : BaseConfig(croot, configFile),
                                               APPLY_BULK(false), 
                                               TIME_VERBOSE(false), 
                                               USE_T2_LOGSPACE(false), 
                                               SAVE_MODE(false)
{
    string defaultFile = (*this).getProjectRoot() + CPMG_CONFIG_DEFAULT;
    if(configFile != (defaultFile)) (*this).readConfigFile(configFile);
    else (*this).readConfigFile(defaultFile);	
}

//copy constructors
cpmg_config::cpmg_config(const cpmg_config &otherConfig) 
{
    // --- Physical attributes
    this->APPLY_BULK = otherConfig.APPLY_BULK;
    this->OBS_TIME = otherConfig.OBS_TIME;
    this->METHOD = otherConfig.METHOD;
    this->TIME_VERBOSE = otherConfig.TIME_VERBOSE;

    this->RESIDUAL_FIELD = otherConfig.RESIDUAL_FIELD;
    this->GRADIENT_VALUE = otherConfig.GRADIENT_VALUE;
    this->GRADIENT_DIRECTION = otherConfig.GRADIENT_DIRECTION;
    this->PATH_TO_FIELD = otherConfig.PATH_TO_FIELD;

    this->MIN_T2 = otherConfig.MIN_T2;
    this->MAX_T2 = otherConfig.MAX_T2;
    this->USE_T2_LOGSPACE = otherConfig.USE_T2_LOGSPACE;
    this->NUM_T2_BINS = otherConfig.NUM_T2_BINS;
    this->MIN_LAMBDA = otherConfig.MIN_LAMBDA;
    this->MAX_LAMBDA = otherConfig.MAX_LAMBDA;
    this->NUM_LAMBDAS = otherConfig.NUM_LAMBDAS;
    this->PRUNE_NUM = otherConfig.PRUNE_NUM;
    this->NOISE_AMP = otherConfig.NOISE_AMP;

    // --- cpmg SAVE. 
    this->SAVE_MODE = otherConfig.SAVE_MODE;
    this->SAVE_T2 = otherConfig.SAVE_T2;
    this->SAVE_WALKERS = otherConfig.SAVE_WALKERS;
    this->SAVE_DECAY = otherConfig.SAVE_DECAY;
    this->SAVE_HISTOGRAM = otherConfig.SAVE_HISTOGRAM;
    this->SAVE_HISTOGRAM_LIST = otherConfig.SAVE_HISTOGRAM_LIST;
}

vector<string> cpmg_config::checkConfig()
{
    vector<string> missingParameters;
    bool validState = true;

    validState &= (*this).checkItem((*this).getObservationTime() > 0.0, (string)"OBS_TIME", missingParameters);
    
    vector<string> methods = {"image-based", "histogram"};
    validState &= (*this).checkItem(std::find(methods.begin(), methods.end(), (*this).getMethod()) != methods.end(), 
                      (string)"METHOD", missingParameters);
    
    vector<string> rfs = {"none", "uniform", "import"};
    validState &= (*this).checkItem(std::find(rfs.begin(), rfs.end(), (*this).getResidualField()) != rfs.end(), 
                      (string)"RESIDUAL_FIELD", missingParameters);

    validState &= (*this).checkItem(((*this).getGradientDirection() == 0 or (*this).getGradientDirection() == 1 or (*this).getGradientDirection() == 2), 
                      (string)"GRADIENT_DIRECTION", missingParameters);

    validState &= (*this).checkItem((*this).getMinT2() > 0.0, (string)"MIN_T2", missingParameters);
    validState &= (*this).checkItem((*this).getMaxT2() > 0.0, (string)"MAX_T2", missingParameters);
    validState &= (*this).checkItem((*this).getNumT2Bins() > 0, (string)"NUM_T2_BINS", missingParameters);
    validState &= (*this).checkItem((*this).getNumLambdas() > 0, (string)"NUM_LAMBDAS", missingParameters);
    validState &= (*this).checkItem((*this).getPruneNum() >= 0, (string)"PRUNE_NUM", missingParameters);
    
    (*this).setReady(validState);   
    return missingParameters;
}

// read config file
void cpmg_config::readConfigFile(const string configFile)
{
	ifstream fileObject;
    fileObject.open(configFile, ios::in);
    if (fileObject.fail())
    {
        cout << "Could not open cpmg config file from disc." << endl;
        cout << "Check path: " << configFile << endl;
        exit(1);
    }

    string line;
    while(fileObject)
    {
    	getline(fileObject, line);
    	// cout << line << endl;

    	string s = line;
    	string delimiter = ": ";
		size_t pos = 0;
		string token, content;
    	while ((pos = s.find(delimiter)) != std::string::npos) 
    	{
			token = s.substr(0, pos);
			content = s.substr(pos + delimiter.length(), s.length());
			s.erase(0, pos + delimiter.length());

			if(token == "APPLY_BULK") (*this).readApplyBulk(content);  
			else if(token == "OBS_TIME") (*this).readObservationTime(content);  
            else if(token == "METHOD") (*this).readMethod(content);
            else if(token == "TIME_VERBOSE") (*this).readTimeVerbose(content);
            else if(token == "RESIDUAL_FIELD") (*this).readResidualField(content);
            else if(token == "GRADIENT_VALUE") (*this).readGradientValue(content);
            else if(token == "GRADIENT_DIRECTION") (*this).readGradientDirection(content);
            else if(token == "PATH_TO_FIELD") (*this).readPathToField(content);            
            else if(token == "MIN_T2") (*this).readMinT2(content);
            else if(token == "MAX_T2") (*this).readMaxT2(content);
            else if(token == "USE_T2_LOGSPACE") (*this).readUseT2Logspace(content);
            else if(token == "NUM_T2_BINS") (*this).readNumT2Bins(content);
            else if(token == "MIN_LAMBDA") (*this).readMinLambda(content);
            else if(token == "MAX_LAMBDA") (*this).readMaxLambda(content);
            else if(token == "NUM_LAMBDAS") (*this).readNumLambdas(content);
            else if(token == "PRUNE_NUM") (*this).readPruneNum(content);
            else if(token == "NOISE_AMP") (*this).readNoiseAmp(content);                      
            else if(token == "SAVE_MODE") (*this).readSaveMode(content);
            else if(token == "SAVE_T2") (*this).readSaveT2(content);
            else if(token == "SAVE_WALKERS") (*this).readSaveWalkers(content);
            else if(token == "SAVE_DECAY") (*this).readSaveDecay(content);
            else if(token == "SAVE_HISTOGRAM") (*this).readSaveHistogram(content);
            else if(token == "SAVE_HISTOGRAM_LIST") (*this).readSaveHistogramList(content); 
			
		}
    } 

    fileObject.close();
}

void cpmg_config::readApplyBulk(string s)
{
    if(s == "true") (*this).setApplyBulk(true);
    else (*this).setApplyBulk(false);
}

void cpmg_config::readTimeVerbose(string s)
{
    if(s == "true") (*this).setTimeVerbose(true);
    else (*this).setTimeVerbose(false);
}

void cpmg_config::readObservationTime(string s)
{
    (*this).setObservationTime(std::stod(s));
}

void cpmg_config::readMethod(string s)
{
    if(s == "histogram") (*this).setMethod(s);
    else (*this).setMethod("image-based");
}

void cpmg_config::readResidualField(string s)
{
    if(s == "uniform" or s == "import") (*this).setResidualField(s);
    else (*this).setResidualField("none");
}

void cpmg_config::readGradientValue(string s)
{
    (*this).setGradientValue(std::stod(s));
}

void cpmg_config::readGradientDirection(string s)
{
    if(s == "0" or s == "x")  (*this).setGradientDirection(0);
    else if(s == "1" or s == "y") (*this).setGradientDirection(1);
    else (*this).setGradientDirection(2);
}

void cpmg_config::readPathToField(string s)
{
    if(s.length() > 2 and s.substr(0,2) == "./") (*this).setPathToField((*this).getProjectRoot() + s);
	else (*this).setPathToField(s);
}

void cpmg_config::readMinT2(string s)
{
    (*this).setMinT2(std::stod(s));
}

void cpmg_config::readMaxT2(string s)
{
    (*this).setMaxT2(std::stod(s));
}

void cpmg_config::readUseT2Logspace(string s)
{
    if(s == "true") (*this).setUseT2Logspace(true);
    else (*this).setUseT2Logspace(false);
}

void cpmg_config::readNumT2Bins(string s)
{
    (*this).setNumT2Bins(std::stoi(s));
}

void cpmg_config::readMinLambda(string s)
{
    (*this).setMinLambda(std::stod(s));
}

void cpmg_config::readMaxLambda(string s)
{
    (*this).setMaxLambda(std::stod(s));
}

void cpmg_config::readNumLambdas(string s)
{   
    (*this).setNumLambdas(std::stoi(s));
}

void cpmg_config::readPruneNum(string s)
{
    (*this).setPruneNum(std::stoi(s));
}

void cpmg_config::readNoiseAmp(string s)
{
    (*this).setNoiseAmp(std::stod(s));
}

void cpmg_config::readSaveMode(string s)
{
    if(s == "true") (*this).setSaveMode(true);
    else (*this).setSaveMode(false);
}

void cpmg_config::readSaveWalkers(string s)
{
    if(s == "true") (*this).setSaveWalkers(true);
    else (*this).setSaveWalkers(false);
}

void cpmg_config::readSaveDecay(string s)
{
    if(s == "true") (*this).setSaveDecay(true);
    else (*this).setSaveDecay(false);
}

void cpmg_config::readSaveHistogram(string s)
{
    if(s == "true") (*this).setSaveHistogram(true);
    else (*this).setSaveHistogram(false);
}

void cpmg_config::readSaveHistogramList(string s)
{
    if(s == "true") (*this).setSaveHistogramList(true);
    else (*this).setSaveHistogramList(false);
}

void cpmg_config::readSaveT2(string s)
{
    if(s == "true") (*this).setSaveT2(true);
    else (*this).setSaveT2(false);
}
