// include class header file
#include "rwnmrApp.h"

rwnmrApp::rwnmrApp(int argc, char **argv, string _root) : projectRoot(_root), args(argc, argv), model(NULL)
{
    (*this).setConfigRoot((*this).getProjectRoot() + CONFIG_ROOT);   
}

rwnmrApp::rwnmrApp(ArgsParser _args, string _root) : projectRoot(_root), args(_args), model(NULL)
{
    (*this).setConfigRoot((*this).getProjectRoot() + CONFIG_ROOT); 
}

void rwnmrApp::buildEssentials()
{
    cout << "--- Building RWNMR essentials" << endl;

    // -- Read NMR essentials config files  
    // -- rwnmr & uct image config
    string RwnmrConfig_path;
    if((*this).getArgsPath(0) != "default") RwnmrConfig_path = (*this).getArgsPath(0);
    else RwnmrConfig_path = RWNMR_CONFIG_DEFAULT;
    
    string UctConfig_path;
    if((*this).getArgsPath(1) != "default") UctConfig_path = (*this).getArgsPath(1);
    else UctConfig_path = UCT_CONFIG_DEFAULT;

    RwnmrConfig rwNMR_Config((*this).getProjectRoot() + RwnmrConfig_path, (*this).getProjectRoot());     
    UctConfig uCT_Config((*this).getProjectRoot() + UctConfig_path, (*this).getProjectRoot()); 
    // // -----

    // -- Create NMR_Simulation object
    // this->model = new Model(rwNMR_Config, uCT_Config, (*this).getProjectRoot());
    (*this).setModel(new Model(rwNMR_Config, uCT_Config, (*this).getProjectRoot()));
    // Read digital rock image
    cout << "-- Loading uCT-image" << endl;
    (*this).getModel().readImage();

    // Create and set up random walkers
    cout << endl << "-- Setting random walkers" << endl;
    (*this).getModel().initWalkers();

    // Save image info
    cout << endl << "-- Saving uCT-image info" << endl;
    (*this).getModel().save();
    cout << endl; (*this).getModel().info();
    // -----    
}

void rwnmrApp::exec()
{
    (*this).buildEssentials();

    uint commands = (*this).getArgs().getCommands().size();
    uint current = 2;
    while(current < commands)
    {
        if((*this).getArgsCommand(current) == "cpmg") (*this).CPMG(current);
        else if((*this).getArgsCommand(current) == "pfgse") (*this).PFGSE(current);
        else if((*this).getArgsCommand(current) == "ga") (*this).GA(current);
        else if((*this).getArgsCommand(current) == "multitau") (*this).MultiTau(current);

        current++;
    }
}

void rwnmrApp::CPMG(uint command_idx)
{
    cout << "-- CPMG to be executed:" << endl;
    // -- Read CPMG routine config files
    string CpmgConfig_path;
    if((*this).getArgsPath(command_idx) != "default") CpmgConfig_path = (*this).getArgsPath(command_idx);
    else CpmgConfig_path = CPMG_CONFIG_DEFAULT;
    CpmgConfig cpmg_Config((*this).getProjectRoot() + CpmgConfig_path, (*this).getProjectRoot());
    // --

    // -- Create cpmg object
    NMR_cpmg cpmg((*this).getModel(), cpmg_Config);
    cpmg.run();
    cout << endl << "- cpmg executed succesfully" << endl << endl;
    // -----
}

void rwnmrApp::PFGSE(uint command_idx)
{
    cout << "-- PFGSE to be executed:" << endl;
    // -- Read PFGSE routine config files
    string PfgseConfig_path;
    if((*this).getArgsPath(command_idx) != "default") PfgseConfig_path = (*this).getArgsPath(command_idx);
    else PfgseConfig_path = PFGSE_CONFIG_DEFAULT;
    PfgseConfig pfgse_Config((*this).getProjectRoot() + PfgseConfig_path, (*this).getProjectRoot());
    // --
    
    NMR_PFGSE pfgse((*this).getModel(), pfgse_Config);
    pfgse.run();
    cout << "- pfgse sequence executed succesfully" << endl << endl;
    // -----
}

void rwnmrApp::GA(uint command_idx)
{
    cout << "-- GA is under construction." << endl;
}

void rwnmrApp::MultiTau(uint command_idx)
{
    cout << "-- MultiTau to be executed:" << endl;

    // -- Read MultiTau routine config files
    string MultitauConfig_path;
    if((*this).getArgsPath(command_idx) != "default") MultitauConfig_path = (*this).getArgsPath(command_idx);
    else MultitauConfig_path = MULTITAU_CONFIG_DEFAULT;
    MultitauConfig multitau_Config((*this).getProjectRoot() + MultitauConfig_path, (*this).getProjectRoot());
    // --

    // -- Read CPMG routine config files
    string CpmgConfig_path;
    if((*this).getArgsPath(command_idx + 1) != "default") CpmgConfig_path = (*this).getArgsPath(command_idx + 1);
    else CpmgConfig_path = CPMG_CONFIG_DEFAULT;
    CpmgConfig cpmg_Config((*this).getProjectRoot() + CpmgConfig_path, (*this).getProjectRoot());
    // --
    
    NMR_multitau multitau((*this).getModel(), multitau_Config, cpmg_Config);
    multitau.run();
    cout << "- multitau sequence executed succesfully" << endl << endl;
    // -----
}
