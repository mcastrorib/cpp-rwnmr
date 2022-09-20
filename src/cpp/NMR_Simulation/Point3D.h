#ifndef POINT3D_H_
#define POINT3D_H_

#include <iostream>
#include <vector>
#include <math.h>
#include "Walker_defs.h"

// include OpenCV core functions
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class Point3D
{
public:

    // methods
    Point3D() : x(0), y(0), z(0) {}
    Point3D(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    Point3D(const Point3D &other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    inline bool isPore(vector<Mat> &_binaryMap)
    {
        uchar *mapPixel = _binaryMap[this->z].ptr<uchar>(this->y);

        if (mapPixel[this->x] == 0)
            return true;
        else
            return false;
    };

    inline bool isPore(Mat &_binaryMap)
    {
        uchar *mapPixel = _binaryMap.ptr<uchar>(this->y);

        if (mapPixel[this->x] == 0)
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

private:
    int x;
    int y;
    int z;

};

#endif