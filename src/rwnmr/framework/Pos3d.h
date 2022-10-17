#ifndef POS3D_H_
#define POS3D_H_

#include "Defs.h"

class Pos3d
{
private:
    int x;
    int y;
    int z;

public:

    // methods
    Pos3d() : x(0), y(0), z(0) {}
    Pos3d(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    Pos3d(const Pos3d &other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    inline bool isPore(vector<Mat> &_binaryMap)
    {
        uchar *mapPixel = _binaryMap[this->z].ptr<uchar>(this->y);

        if (mapPixel[this->x] == PORE_COLOR)
            return true;
        else
            return false;
    };

    inline bool isPore(Mat &_binaryMap)
    {
        uchar *mapPixel = _binaryMap.ptr<uchar>(this->y);

        if (mapPixel[this->x] == PORE_COLOR)
            return true;
        else
            return false;
    };

    inline void printInfo()
    {
        cout << "{" << this->x << ", " << this->y << ", " << this->z << "}" << endl;
    };

    int getX(){ return this->x; }
    int getY(){ return this->y; }
    int getZ(){ return this->z; }
    void setX(int _x){ this->x=_x; }
    void setY(int _y){ this->y=_y; }
    void setZ(int _z){ this->z=_z; }

};

#endif