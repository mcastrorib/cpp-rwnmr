#include "UctConfig.h"

using namespace std;

// default constructors
UctConfig::UctConfig(const string configFile, const string croot) : BaseConfig(croot, configFile)
{
	(*this).setImgFilesList("Empty");
	vector<string> IMG_FILES();

	string defaultFile = (*this).getProjectRoot() + UCT_CONFIG_DEFAULT;
    if(configFile != (defaultFile)) (*this).readConfigFile(configFile);
    else (*this).readConfigFile(defaultFile);	
	if((*this).getImgFilesList() == "Empty") (*this).createImgFileList();	
	(*this).readImgFiles();
}

//copy constructors
UctConfig::UctConfig(const UctConfig &otherConfig)
{
	this->DIR_PATH = otherConfig.DIR_PATH;
    this->FILENAME = otherConfig.FILENAME;
    this->FIRST_IDX = otherConfig.FIRST_IDX;
    this->DIGITS = otherConfig.DIGITS;
    this->EXTENSION = otherConfig.EXTENSION;
    this->SLICES = otherConfig.SLICES;
    this->RESOLUTION = otherConfig.RESOLUTION;
    this->VOXEL_DIVISION = otherConfig.VOXEL_DIVISION;
	this->IMG_FILES_LIST = otherConfig.IMG_FILES_LIST;
	this->IMG_FILES = otherConfig.IMG_FILES;
}

vector<string> UctConfig::checkConfig()
{
    vector<string> missingParameters;
    bool validState = true;

    vector<string> extensions = {".png", ".tif"};
    validState &= (*this).checkItem(std::find(extensions.begin(), extensions.end(), (*this).getExtension()) != extensions.end(), 
                      (string)"EXTENSION", missingParameters);
    
    validState &= (*this).checkItem((*this).getResolution() > 0.0, (string)"RESOLUTION", missingParameters);
    
    (*this).setReady(validState);   
    return missingParameters;
}

// read config file
void UctConfig::readConfigFile(const string configFile)
{
    ifstream fileObject;
    fileObject.open(configFile, ios::in);
    if (fileObject.fail())
    {
        cout << "Could not open uct config file from disc." << endl;
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

			if(token == "DIR_PATH")	(*this).readDirPath(content);
			else if(token == "FILENAME") (*this).readFilename(content);
			else if(token == "FIRST_IDX") (*this).readFirstIdx(content);
			else if(token == "DIGITS") (*this).readDigits(content);
			else if(token == "EXTENSION") (*this).readExtension(content);
			else if(token == "SLICES") (*this).readSlices(content);
			else if(token == "RESOLUTION") (*this).readResolution(content);
			else if(token == "VOXEL_DIVISION") (*this).readVoxelDivision(content);
			else if(token == "IMG_FILES_LIST") (*this).readImgFilesList(content);			
		}
    } 

    fileObject.close();
}

void UctConfig::readDirPath(string s)
{
    if(s.length() > 2 and s.substr(0,2) == "./") (*this).setDirPath((*this).getProjectRoot() + s);
	else (*this).setDirPath(s);
}

void UctConfig::readFilename(string s)
{
	this->FILENAME = s;
}

void UctConfig::readFirstIdx(string s)
{
	this->FIRST_IDX = std::stoi(s);
}

void UctConfig::readDigits(string s)
{
	this->DIGITS = std::stoi(s);
}

void UctConfig::readExtension(string s)
{
	this->EXTENSION = s;
}

void UctConfig::readSlices(string s)
{
	this->SLICES = std::stoi(s);
}

void UctConfig::readResolution(string s)
{
	this->RESOLUTION = std::stod(s);
}

void UctConfig::readVoxelDivision(string s)
{
	this->VOXEL_DIVISION = std::stoi(s);
}

void UctConfig::readImgFilesList(string s)
{
	this->IMG_FILES_LIST = s;
}

void UctConfig::readImgFiles()
{
	const string filepath = (*this).getImgFilesList();
	
    ifstream fileObject;
    fileObject.open(filepath, ios::in);
    if (fileObject.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

	// reserve memory for image file list
	if(this->IMG_FILES.size() > 0) this->IMG_FILES.clear();
	this->IMG_FILES.reserve((*this).getSlices());

    string line;
	uint slice = 0;
    while(fileObject)
    {
    	getline(fileObject, line);
		// cout << line << endl;
    	if(slice < (*this).getSlices()) this->IMG_FILES.push_back(line);
		slice++;
    } 

    fileObject.close();
}

void UctConfig::createImgFileList()
{
    string dirpath = (*this).getProjectRoot() + CONFIG_ROOT;
	string filepath = dirpath + ".ImagesList.txt";

    ofstream fileObject;
    fileObject.open(filepath, ios::out);
    if (fileObject.fail())
    {
        cout << "Could not open images list file from disc." << endl;
        exit(1);
    }

	// constant strings
    string dirname = (*this).getDirPath();
    string fname = (*this).getFilename();
    uint firstId = (*this).getFirstIdx();
    uint digits = (*this).getDigits();
	uint slices = (*this).getSlices();
    string extension = (*this).getExtension();
	ImagePath paths(dirname, fname, firstId, digits, slices, extension);

    // variable strings
    string currentId;
    string currentImagePath;    

    for (uint slice = 0; slice < slices; slice++)
    {
        fileObject << paths.getImagePathFromList(slice) << endl;
    }	  

	(*this).readImgFilesList(filepath);    
}


