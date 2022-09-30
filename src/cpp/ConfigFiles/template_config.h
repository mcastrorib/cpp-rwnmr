#ifndef TEMPLATE_CONFIG_H_
#define TEMPLATE_CONFIG_H_

#include "configFiles_defs.h"

using namespace std;

class template_config
{
private:
    string config_filepath;
        
public:
    // default constructors
    template_config(){};
    template_config(const string configFile, const string croot);

    //copy constructors
    template_config(const template_config &otherConfig);

    // default destructor
    virtual ~template_config(){} 

    void readConfigFile(const string configFile);
    
    void findConfigFile(string filepath)
    {
        // search in config dir

        // search in input dir

        // search in filesystem
    }
};

#endif