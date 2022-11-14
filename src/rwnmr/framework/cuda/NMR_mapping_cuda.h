#ifndef NMR_MAPPING_CUDA_H_
#define NMR_MAPPING_CUDA_H_

#include "cuda_runtime.h"
#include "../Defs.h"
#include "../Walker.h"
#include "../Model.h"


// Device functions
__device__ direction computeNextDirection_MAP(uint64_t &seed);
__device__ void computeNextPosition_MAP(int &walker_px, int &walker_py, int &walker_pz, direction nextDirection, int &next_x, int &next_y, int &next_z);
__device__ bool checkNextPosition_MAP(int next_x, int next_y, int next_z, const uint64_t *bitBlock, const int bitBlockColumns, const int bitBlockRows);
__device__ int findBlockIndex_MAP(int next_x, int next_y, int next_z, int bitBlockColumns, int bitBlockRows);
__device__ int findBitIndex_MAP(int next_x, int next_y, int next_z);
__device__ bool checkIfBlockBitIsWall_MAP(uint64_t currentBlock, int currentBit);
__device__ uint64_t xorShift64_MAP(struct xorshift64_state *state);
__device__ uint64_t mod6_MAP(uint64_t a);
__device__ int convertLocalToGlobal_MAP(int _localPos, uint _shiftConverter);
__device__ int isPositive_MAP(int x);
__device__ int isZero_MAP(int x);
__device__ int bcMapPeriodic_MAP(int _localPos, uint _shiftConverter, int _dimSize);
__device__ int bcMapMirror_MAP(int _localPos, uint _shiftConverter, int _dimSize);


template<bool mirror>
__global__ void rwMap(int *walker_px,
                      int *walker_py, 
                      int *walker_pz, 
                      uint *collisions, 
                      uint64_t *seed, 
                      const uint64_t *bitBlock, 
                      const uint bitBlockColumns, 
                      const uint bitBlockRows, 
                      const uint numberOfWalkers, 
                      const uint numberOfSteps, 
                      const int imageColumns, 
                      const int imageRows, 
                      const int imageDepth, 
                      const uint shiftConverter)
{
    // identify thread's walker
    int walkerId = threadIdx.x + blockIdx.x * blockDim.x;

    // Local variables for unique read from device global memory
    int localPosX, localPosY, localPosZ;
    int imgPosX, imgPosY, imgPosZ;
    uint64_t localSeed;
    uint localCollisions;

    // thread variables for future movements
    int localNextX, localNextY, localNextZ;
    direction nextDirection = None;
    

    // now begin the "walk" procedure de facto
    if (walkerId < numberOfWalkers)
    {
        // Local variables for unique read from device global memory
        localPosX = walker_px[walkerId];
        localPosY = walker_py[walkerId];
        localPosZ = walker_pz[walkerId];
        localSeed = seed[walkerId];
        localCollisions = collisions[walkerId];

        for (int step = 0; step < numberOfSteps; step++)
        {          
    
            nextDirection = computeNextDirection_MAP(localSeed); 
            computeNextPosition_MAP(localPosX, localPosY, localPosZ, nextDirection, localNextX, localNextY, localNextZ);

            // update img position
            if(mirror)
            {
                imgPosX = bcMapMirror_MAP(localNextX, shiftConverter, imageColumns);
                imgPosY = bcMapMirror_MAP(localNextY, shiftConverter, imageRows);
                imgPosZ = bcMapMirror_MAP(localNextZ, shiftConverter, imageDepth);
            }
            else
            {
                imgPosX = bcMapPeriodic_MAP(localNextX, shiftConverter, imageColumns);
                imgPosY = bcMapPeriodic_MAP(localNextY, shiftConverter, imageRows);
                imgPosZ = bcMapPeriodic_MAP(localNextZ, shiftConverter, imageDepth);
            }

            if (checkNextPosition_MAP(imgPosX, imgPosY, imgPosZ, bitBlock, bitBlockColumns, bitBlockRows))
            {
                // update real position
                localPosX = localNextX;
                localPosY = localNextY;
                localPosZ = localNextZ;                
            }
            else
            {
                // walker hits wall and comes back to the same position
                // collisions count is incremented
                localCollisions += 1;
            }

        }

        // position and seed device global memory update
        // must be done for each kernel
        walker_px[walkerId] = localPosX;
        walker_py[walkerId] = localPosY;
        walker_pz[walkerId] = localPosZ;
        collisions[walkerId] = localCollisions;
        seed[walkerId] = localSeed;
    }
}

#endif