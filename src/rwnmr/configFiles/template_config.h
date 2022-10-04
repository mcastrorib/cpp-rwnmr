#ifndef TEMPLATE_CONFIG_H_
#define TEMPLATE_CONFIG_H_

#include "configFiles_defs.h"

using namespace std;

class template_config
{
public:
    string projectRoot;
    string configFilepath;
    ifstream *fileObject;

    // default constructors
    template_config():fileObject(NULL){}
    template_config(string croot) : projectRoot(croot), fileObject(NULL)
    {}

    //copy constructors
    template_config(const template_config &other)
    {
        this->projectRoot = other.projectRoot;
        this->configFilepath = other.configFilepath;
        this->fileObject = other.fileObject;
    }

    // default destructor
    virtual ~template_config()
    {
        if(this->fileObject != NULL) 
        {       
            delete this->fileObject;
        }
        this->fileObject = NULL;
    } 
    void setProjectRoot(string _croot){ this->projectRoot = _croot;}
    void setConfigFilepath(string _p){ this->configFilepath = _p; }
    void setFileObject(ifstream *_fo){ this->fileObject = _fo; }
    string getProjectRoot(){return this->projectRoot;}
    string getConfigFilepath(){ return this->configFilepath; }
    ifstream *getFileObject(){ return this->fileObject; }

    void readConfigFile(const string configFile)
    {
        if(this->fileObject != NULL) delete this->fileObject;
        this->fileObject = new ifstream(configFile, ios::in);
        if (this->fileObject->fail())
        {
            cout << "Could not open config file " << configFile << " from disc." << endl;
            exit(1);
        }
    }

    void closeFileObject()
    {
        fileObject->close();
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