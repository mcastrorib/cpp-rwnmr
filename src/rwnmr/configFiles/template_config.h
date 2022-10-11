#ifndef TEMPLATE_CONFIG_H_
#define TEMPLATE_CONFIG_H_

#include "configFiles_defs.h"

using namespace std;

class template_config
{
public:
    string projectRoot;
    string configFilepath;

    // default constructors
    template_config(){}
    template_config(string croot) : projectRoot(croot){}

    //copy constructors
    template_config(const template_config &other)
    {
        this->projectRoot = other.projectRoot;
        this->configFilepath = other.configFilepath;
    }

    // default destructor
    virtual ~template_config(){} 

    void setProjectRoot(string _croot){ this->projectRoot = _croot;}
    void setConfigFilepath(string _p){ this->configFilepath = _p; }
    string getProjectRoot(){return this->projectRoot;}
    string getConfigFilepath(){ return this->configFilepath; }
    
    virtual void readConfigFile(const string configFile) = 0;
    virtual void checkConfig() = 0;

    bool checkItem(bool condition, const string item, string &msg)
    {
        if(!condition)
        {
            msg += item;
            return false;
        } else 
        {
            return true;
        }
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