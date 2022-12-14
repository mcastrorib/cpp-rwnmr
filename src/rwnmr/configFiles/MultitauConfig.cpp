#include "MultitauConfig.h"

using namespace std;

// default constructors
MultitauConfig::MultitauConfig(const string configFile, const string croot) : BaseConfig(croot,configFile), COMPLETE_DECAY(true), SAVE_MODE(false)
{
    vector<double> TAU_VALUES();
    string defaultFile = (*this).getProjectRoot() + MULTITAU_CONFIG_DEFAULT;
    if(configFile != (defaultFile)) (*this).readConfigFile(configFile);
    else (*this).readConfigFile(defaultFile);	
}

//copy constructors
MultitauConfig::MultitauConfig(const MultitauConfig &otherConfig) 
{
    // --- Physical attributes.
    this->TAU_MIN = otherConfig.TAU_MIN;
    this->TAU_MAX = otherConfig.TAU_MAX;
    this->TAU_POINTS = otherConfig.TAU_POINTS;
    this->TAU_VALUES = otherConfig.TAU_VALUES;
    this->TAU_SCALE = otherConfig.TAU_SCALE;
    this->COMPLETE_DECAY = otherConfig.COMPLETE_DECAY;

    // --- cpmg SAVE. 
    this->SAVE_MODE = otherConfig.SAVE_MODE;
    this->SAVE_DECAY = otherConfig.SAVE_DECAY;
    this->SAVE_WALKERS = otherConfig.SAVE_WALKERS;
    this->SAVE_HISTOGRAM = otherConfig.SAVE_HISTOGRAM;
    this->SAVE_HISTOGRAM_LIST = otherConfig.SAVE_HISTOGRAM_LIST;
}

vector<string> MultitauConfig::checkConfig()
{
	vector<string> missingParameters;
    bool validState = true;
   
    validState &= (*this).checkItem((*this).getTauPoints() > 0, (string)"TAU_POINTS", missingParameters);
    if((*this).getTauScale() == "manual") 
    {
        validState &= (*this).checkItem((*this).getTauValues().size() == (*this).getTauPoints(), (string)"TAU_VALUES!=TAU_POINTS", missingParameters);
        for(int i = 0; i < (*this).getTauValues().size(); i++)
            validState &= (*this).checkItem((*this).getTauValues()[i] > 0.0, (string)"TAU_VALUES(<0)", missingParameters);
    }
    else 
    {
        validState &= (*this).checkItem((*this).getTauMin() > 0.0, (string)"TAU_MIN", missingParameters);
        validState &= (*this).checkItem((*this).getTauMax() > 0.0, (string)"TAU_MAX", missingParameters);
        validState &= (*this).checkItem((*this).getTauMin() < (*this).getTauMax(), (string)"TAU_MIN>TAU_MAX", missingParameters);    
    }

	vector<string> scales = {"manual", "linear", "log"};
    validState &= (*this).checkItem(std::find(scales.begin(), scales.end(), (*this).getTauScale()) != scales.end(), 
                      (string)"TAU_SCALE", missingParameters);

    (*this).setReady(validState);   
    return missingParameters;
}


// read config file
void MultitauConfig::readConfigFile(const string configFile)
{
	ifstream fileObject;
    fileObject.open(configFile, ios::in);
    if (fileObject.fail())
    {
        cout << "Could not open multitau config file from disc." << endl;
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

			if(token == "TAU_MIN") (*this).readTauMin(content);
            else if(token == "TAU_MAX") (*this).readTauMax(content);
            else if(token == "TAU_POINTS") (*this).readTauPoints(content);
            else if(token == "TAU_VALUES") (*this).readTauValues(content);
            else if(token == "TAU_SCALE") (*this).readTauScale(content);
            else if(token == "COMPLETE_DECAY") (*this).readCompleteDecay(content);
            else if(token == "SAVE_MODE") (*this).readSaveMode(content);
            else if(token == "SAVE_DECAY") (*this).readSaveDecay(content);
            else if(token == "SAVE_WALKERS") (*this).readSaveWalkers(content);
            else if(token == "SAVE_HISTOGRAM") (*this).readSaveHistogram(content);
            else if(token == "SAVE_HISTOGRAM_LIST") (*this).readSaveHistogramList(content); 
			
		}
    } 

    fileObject.close();
}

void MultitauConfig::readTauMin(string s)
{
	this->TAU_MIN = std::stod(s);
}

void MultitauConfig::readTauMax(string s)
{
    this->TAU_MAX = std::stod(s);
}

void MultitauConfig::readTauPoints(string s)
{
    this->TAU_POINTS = std::stoi(s);
}

void MultitauConfig::readTauValues(string s)
{
    if(this->TAU_VALUES.size() > 0) this->TAU_VALUES.clear();

    // parse vector
    if(s.compare(0, 1, "{") == 0 and s.compare(s.length() - 1, 1, "}") == 0)
    {
        string strvec = s.substr(1, s.length() - 2);
        string delimiter = ",";
        size_t pos = 0;
        string token, content;
        while ((pos = strvec.find(delimiter)) != std::string::npos) 
        {
            token = strvec.substr(0, pos);
            content = strvec.substr(pos + delimiter.length(), strvec.length());
            strvec.erase(0, pos + delimiter.length());

            // add value 
            this->TAU_VALUES.push_back(std::stod(token));
        }
        // add value to RHO attribute
        this->TAU_VALUES.push_back(std::stod(strvec));
    } 

    std::sort(this->TAU_VALUES.begin(), this->TAU_VALUES.end());
}

void MultitauConfig::readTauScale(string s)
{
    if(s == "log") this->TAU_SCALE = s;
    else if(s == "manual") this->TAU_SCALE = s;
    else this->TAU_SCALE = "linear";
}

void MultitauConfig::readCompleteDecay(string s)
{
    if(s == "true" or s == "True" or s == "TRUE") this->COMPLETE_DECAY = true;
    else this->COMPLETE_DECAY = false;
}

void MultitauConfig::readSaveMode(string s)
{
    if(s == "true") this->SAVE_MODE = true;
    else this->SAVE_MODE = false;
}

void MultitauConfig::readSaveWalkers(string s)
{
    if(s == "true") this->SAVE_WALKERS = true;
    else this->SAVE_WALKERS = false;
}

void MultitauConfig::readSaveDecay(string s)
{
    if(s == "true") this->SAVE_DECAY = true;
    else this->SAVE_DECAY = false;
}

void MultitauConfig::readSaveHistogram(string s)
{
    if(s == "true") this->SAVE_HISTOGRAM = true;
    else this->SAVE_HISTOGRAM = false;
}

void MultitauConfig::readSaveHistogramList(string s)
{
    if(s == "true") this->SAVE_HISTOGRAM_LIST = true;
    else this->SAVE_HISTOGRAM_LIST = false;
}
