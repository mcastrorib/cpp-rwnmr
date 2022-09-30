#ifndef UCT_CONFIG_H_
#define UCT_CONFIG_H_

#include "configFiles_defs.h"

using namespace std;

class uct_config
{
private:
    string config_filepath;
    string DIR_PATH;
    string FILENAME;
    uint FIRST_IDX;
    uint DIGITS;
    string EXTENSION;
    uint SLICES;
    double RESOLUTION;
    uint VOXEL_DIVISION;
    string IMG_FILES_LIST;
    vector<string> IMG_FILES;


public:
    // default constructors
    uct_config(){};
    uct_config(const string configFile, const string croot);

    //copy constructors
    uct_config(const uct_config &otherConfig);

    // default destructor
    virtual ~uct_config()
    {
        // cout << "OMPLoopEnabler object destroyed succesfully" << endl;
    } 

    void readConfigFile(const string configFile);
    
    // -- Read methods
    void readDirPath(string s);
    void readFilename(string s);
    void readFirstIdx(string s);
    void readDigits(string s);
    void readExtension(string s);
    void readSlices(string s); 
    void readResolution(string s);
    void readVoxelDivision(string s);
    void readImgFilesList(string s);
    void readImgFiles();

    // -- Read methods
    void setDirPath(string s){ this->DIR_PATH = s;}
    void setFilename(string s){ this->FILENAME = s;}
    void setFirstIdx(uint s){ this->FIRST_IDX = s; }
    void setDigits(uint s){ this->DIGITS = s; }
    void setExtension(string s){ this->EXTENSION = s; }
    void setSlices(uint s){ this->SLICES = s; }
    void setResolution(double s){ this->RESOLUTION = s; }
    void setVoxelDivision(uint s){ this->VOXEL_DIVISION = s; }
    void setImgFilesList(string s){ this->IMG_FILES_LIST = s; }
    void setImgFiles(vector<string> s){ this->IMG_FILES = s; }


    // -- Read methods
    string getConfigFilepath() {return this->config_filepath; }
    string getDirPath(){ return this->DIR_PATH;}
    string getFilename(){ return this->FILENAME;}
    uint getFirstIdx(){ return this->FIRST_IDX;}
    uint getDigits(){ return this->DIGITS;}
    string getExtension(){ return this->EXTENSION;}
    uint getSlices(){ return this->SLICES;} 
    double getResolution(){ return this->RESOLUTION;}
    uint getVoxelDivision(){ return this->VOXEL_DIVISION;}
    string getImgFilesList(){ return this->IMG_FILES_LIST;}
    vector<string> getImgFiles(){ return this->IMG_FILES;}
    string getImgFile(uint idx){ return this->IMG_FILES[idx];}

    void createImgFileList();
    inline string convertFileIDToString(uint id, uint digits)
    {
        stringstream result;
        result << std::setfill('0') << std::setw(digits) << id;
        return result.str();
    }

    string configRoot;
    void setConfigRoot(string _croot){ this->configRoot = _croot;}
    string getConfigRoot(){return this->configRoot;}
};

#endif