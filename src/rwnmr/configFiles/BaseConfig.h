#ifndef TEMPLATE_CONFIG_H_
#define TEMPLATE_CONFIG_H_

#include "configFiles_defs.h"

using namespace std;

class BaseConfig
{
public:
    string projectRoot;
    string configFilepath;
    bool ready;

    // default constructors
    BaseConfig():ready(false){}
    BaseConfig(string croot, string cfile) : projectRoot(croot), configFilepath(cfile), ready(false){}

    //copy constructors
    BaseConfig(const BaseConfig &other)
    {
        this->projectRoot = other.projectRoot;
        this->configFilepath = other.configFilepath;
        this->ready = other.ready;
    }

    // default destructor
    virtual ~BaseConfig(){} 

    void setProjectRoot(string _croot){ this->projectRoot = _croot;}
    void setConfigFilepath(string _p){ this->configFilepath = _p; }
    string getProjectRoot(){return this->projectRoot;}
    string getConfigFilepath(){ return this->configFilepath; }
    void setReady(bool _b){ this->ready = _b; }
    bool getReady(){ return this->ready; }
    
    void readConfigFile(const string configFile){};
    vector<string> checkConfig(){ vector<string> base = {"base_class"}; return base;};

    bool checkItem(bool condition, const string item, vector<string> &mp)
    {
        if(!condition) mp.push_back(item);
        return condition;
    }   
    
    string findConfigFile(string filepath, string _default)
    {       
        string searchPath;
        // search in config dir
        searchPath = (*this).getProjectRoot() + CONFIG_ROOT + filepath;
        cout << "searching in " << searchPath << endl;
        ifstream fo;
        fo.open(searchPath, ios::in);
        if (fo.fail())
        {
            // Search in input dir
            searchPath = (*this).getProjectRoot() + INPUT_DIR + filepath;
            cout << "searching in " << searchPath << endl;
            fo.open(searchPath, ios::in);
            if(fo.fail())
            {
                // Use defaul file
                searchPath = _default;
                cout << "using default: " << searchPath << endl;
            }
            else cout << searchPath << " was found." << endl;
        }
        else cout << searchPath << " was found." << endl;

        fo.close();        
        return searchPath;
    }
};

#endif