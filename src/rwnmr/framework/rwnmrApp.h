#ifndef RWNMRAPP_H_
#define RWNMRAPP_H_

#include "Defs.h"
#include "Model.h"
#include "MapFilter.h"
#include "NMR_cpmg.h"
#include "NMR_multitau.h"
#include "NMR_pfgse.h"

using namespace std;

class rwnmrApp
{
private:   
    string projectRoot;
    string configRoot;
    ArgsParser args;
    Model *model;

public: 
    // default constructors
    rwnmrApp(){};
    rwnmrApp(int argc, char **argv, string _root);
    rwnmrApp(ArgsParser _args, string _root);

    //copy constructors
    rwnmrApp(const rwnmrApp &_otherApp) 
    {
        this->projectRoot = _otherApp.projectRoot;
        this->configRoot = _otherApp.configRoot;
        this->args = _otherApp.args;
        this->model = _otherApp.model;
    }

    // default destructor
    virtual ~rwnmrApp()
    {
        if(model != NULL) delete model;
        model = NULL;
    }

private:
    void setProjectRoot(string _proot) { this->projectRoot = _proot; }

public:
    void setConfigRoot(string _croot) { this->configRoot = _croot; }
    void setArgs(ArgsParser _args){ this->args = _args; }
    void setModel(Model *model){ this->model = model; }
    
    string getProjectRoot() { return this->projectRoot; }
    string getConfigRoot() { return this->configRoot; }
    ArgsParser& getArgs() { return this->args; }
    Model& getModel() { return (*this->model); }
    string getArgsCommand(uint idx) { return this->args.getCommand(idx); }
    string getArgsPath(uint idx) { return this->args.getPath(idx); }
    
    void buildEssentials();
    void exec();    

    void CPMG(uint command_idx);
    void PFGSE(uint command_idx);
    void GA(uint command_idx);
    void MultiTau(uint command_idx);

};

#endif