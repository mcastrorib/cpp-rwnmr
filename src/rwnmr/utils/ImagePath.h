#ifndef IMAGE_PATH_H
#define IMAGE_PATH_H

// include C++ standard libraries
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <cstdlib>

using namespace std;

class ImagePath
{
private:
    vector<string> imagesPathList;
    string path;
    string filename;
    uint fileID;
    uint lastID;
    uint digits;
    uint images;
    string extension;
    string completePath;

public:
    ImagePath():lastID(0){}
    ImagePath(string _path, string _name, uint _fileID, uint _digits, uint _images, string _extension);
    ImagePath(const ImagePath &other)
    {
        this->imagesPathList = other.imagesPathList;
        this->path = other.path;
        this->filename = other.filename;
        this->fileID = other.fileID;
        this->digits = other.digits;
        this->images = other.images;
        this->extension = other.extension;
        this->completePath = other.completePath;
    }

    virtual ~ImagePath(){}

    vector<string> getImagesPathList(){ return this->imagesPathList; }
    string getImagePathFromList(uint idx){ return this->imagesPathList[idx]; }
    string getPath(){ return this->path; };
    string getFilename(){ return this->filename; };
    uint getFileID(){ return this->fileID; };
    uint getLastID(){ return this->lastID; }
    uint getDigits(){ return this->digits; };
    uint getImages(){ return this->images; };
    string getExtension(){ return this->extension; };
    string getCompletePath(){ return this->completePath; };

    void setPath(string newInput){this->path = newInput;}
    void setFilename(string newInput){this->filename = newInput;}
    void setFileID(uint newInput){this->fileID = newInput;}
    void setLastID(uint newInput){this->lastID = newInput;}
    void setFileID(string newInput){this->fileID = std::stoi(newInput);}
    void setDigits(uint newInput){this->digits = newInput;}
    void setDigits(string newInput){this->digits = std::stoi(newInput);}
    void setImages(uint newInput){this->images = newInput;}
    void setImages(string newInput){this->images = std::stoi(newInput);}
    void setImageExtension(string newInput){this->extension = newInput;}
    void setExtension(string newInput){this->extension = newInput;}
    void setCompletePath(string newInput){this->completePath = newInput;}
    
    void createImagesPathList();
    void updateLastID();
    void updateCompletePath();
    void updateNumberOfDigits();

    inline string convertFileIDToString(uint id, uint digits)
    {
        stringstream result;
        result << std::setfill('0') << std::setw(digits) << id;
        return result.str();
    }
};

#endif