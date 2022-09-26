#ifndef NMR_DEFS_H
#define NMR_DEFS_H

// include string stream manipulation functions
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <random>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <limits>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

// include OpenCV core functions
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>

// include OpenMP for multicore implementation
#include <omp.h>

// include other project libs
#include "../Utils/Utils.h"
#include "../Math/Math.h"
#include "../Math/RNG/mRNG.h"
#include "../Math/RNG/xorshift.h"
#include "../Math/RNG/randomIndex.h"
#include "../Math/Laplace/tikhonov.h"
#include "../Math/Laplace/include/nmrinv_core.h"
#include "../ConfigFiles/ConfigFiles.h"

using namespace std;
using namespace cv;

#define MPI_NMR_T2_SIZE 128
#define TWO_PI (6.2831853071795864769252867665590057683943)

#ifndef DIRECTION_ENUM
#define DIRECTION_ENUM
typedef enum Direction
{
    None = 0,
    North = 1,
    West = 2,
    South = 3,
    East = 4,
    Up = 5,
    Down = 6
} direction;
#endif

#define NORTH 1
#define WEST 2
#define SOUTH 3
#define EAST 4
#define UP 5
#define DOWN 6

#define LOG_XIRATE false
#define WALKER_INITIAL_ENERGY 1.0
#define WALKER_DEFAULT_RHO 1.0

#endif