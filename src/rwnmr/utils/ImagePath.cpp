#include "ImagePath.h"

ImagePath::ImagePath(string _path, 
                     string _name, 
                     uint _fileID, 
                     uint _digits,
                     uint _images, 
                     string _extension) : path(_path),
                                          filename(_name),
                                          fileID(_fileID),
                                          lastID(0),
                                          digits(_digits),
                                          images(_images),
                                          extension(_extension) 
{
    vector<string> imagesPathList();
    (*this).updateLastID();
    (*this).updateCompletePath();  
    (*this).createImagesPathList();  
}

void ImagePath::updateLastID()
{
    (*this).setLastID((*this).getFileID() + (*this).getImages());
}

void ImagePath::updateCompletePath()
{
    // (*this).updateNumberOfDigits();
    (*this).setCompletePath(this->path + 
                            this->filename + 
                            (*this).convertFileIDToString(this->fileID, this->digits) + 
                            this->extension);
}

void ImagePath::updateNumberOfDigits()
{
    int finalID = (*this).getFileID() + (*this).getImages();
    int result = finalID / 10;
    int count = 1;

    while (result > 0)
    {
        count++;
        result = result / 10;
    }

    (*this).setDigits(count);
}

void ImagePath::createImagesPathList()
{
    if(this->imagesPathList.size() > 0)
        this->imagesPathList.clear();
    
    if((*this).getLastID() == 0) (*this).updateLastID();

    for(uint s = (*this).getFileID(); s < (*this).getLastID(); s++)
    {
        this->imagesPathList.push_back(this->path + 
                                       this->filename + 
                                       (*this).convertFileIDToString(s, this->digits) + 
                                       this->extension);
    }
}