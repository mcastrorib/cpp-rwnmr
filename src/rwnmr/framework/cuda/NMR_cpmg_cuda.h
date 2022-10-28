#ifndef NMR_CPMG_CUDA_H_
#define NMR_CPMG_CUDA_H_

#include "cuda_runtime.h"
#include "../Defs.h"
#include "../Walker.h"
#include "../Model.h"
#include "../NMR_cpmg.h"

typedef struct 
{
    int x;
    int y;
    int z;
} dPoint;

typedef struct 
{
    dPoint p000;
    dPoint p100;
    dPoint p010;
    dPoint p110;
    dPoint p001;
    dPoint p101;
    dPoint p011;
    dPoint p111;
} Neighborhood;

typedef struct 
{
    double c000; long i000;
    double c100; long i100;
    double c010; long i010;
    double c110; long i110;
    double c001; long i001;
    double c101; long i101;
    double c011; long i011;
    double c111; long i111;
} InterpCube;


// Device functions
__device__ direction computeNextDirection_CPMG(uint64_t &seed);
__device__ void computeNextPosition_CPMG(int &walker_px, int &walker_py, int &walker_pz, direction nextDirection, int &next_x, int &next_y, int &next_z);
__device__ bool checkNextPosition_CPMG(int next_x, int next_y, int next_z, const uint64_t *bitBlock, const int bitBlockColumns, const int bitBlockRows);
__device__ int findBlockIndex_CPMG(int next_x, int next_y, int next_z, int bitBlockColumns, int bitBlockRows);
__device__ int findBitIndex_CPMG(int next_x, int next_y, int next_z);
__device__ bool checkIfBlockBitIsWall_CPMG(uint64_t currentBlock, int currentBit);
__device__ uint64_t xorShift64_CPMG(struct xorshift64_state *state);
__device__ uint64_t mod6_CPMG(uint64_t a);
__device__ int convertLocalToGlobal_CPMG(int _localPos, uint _shiftConverter);
__device__ int isPositive_cpmg(int x);
__device__ int isZero_cpmg(int x);
__device__ int bcMapPeriodic_CPMG(int _localPos, uint _shiftConverter, int _dimSize);
__device__ int bcMapPeriodic_CPMG_old(int _localPos, uint _shiftConverter, int _dimSize);
__device__ int bcMapMirror_CPMG(int _localPos, uint _shiftConverter, int _dimSize);
__device__ int bcMapMirror_CPMG_old(int _localPos, uint _shiftConverter, int _dimSize);
__device__ long getFieldIndex(int _x, int _y, int _z, int _rowScale, int _depthScale);
__device__ void getNeighbors_periodic(Neighborhood &n, int x, int y, int z, int stride, uint shiftConverter, int cols, int rows, int depth);
__device__ void getNeighbors_mirror(Neighborhood &n, int x, int y, int z, uint shiftConverter, int cols, int rows, int depth);
__device__ void getInterpCube(InterpCube &ic, Neighborhood &n, const double *field, int rowScale, int depthScale);
__device__ double interpValues(InterpCube &ic, int x, int y, int z, int stride); 
__device__ double triLinInterp_periodic(const double *field, int x, int y, int z, int stride, int shiftConverter, int cols, int rows, int depth, Neighborhood &n, InterpCube &ic);
__device__ double triLinInterp_mirror(const double *field, int x, int y, int z, int stride, int shiftConverter, int cols, int rows, int depth, Neighborhood &n, InterpCube &ic);

// Kernel declarations
__global__ void CPMG_walk_test(void);

template <bool mirror>
__global__ void bcMap_test(int shiftConverter, int dimSize)
{    
    // identify thread's walker
    int walkerId = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = pow(2,shiftConverter);
    if(walkerId == 0)
    {
        if(mirror) printf("Mirror BC::\n");
        else printf("Periodic BC::\n");
        printf("sc: %d,\t dim: %d\n", shiftConverter, dimSize);
        for(int i = -3*dimSize*stride; i < 3*dimSize*stride; i++)
        {
           if(mirror) printf("pos: %d -> img: %d (a) or %d (b)\n", i, bcMapMirror_CPMG_old(i,shiftConverter,dimSize), bcMapMirror_CPMG(i,shiftConverter,dimSize));
           else printf("pos: %d -> img: %d (a) or %d (b)\n", i, bcMapPeriodic_CPMG_old(i,shiftConverter,dimSize), bcMapPeriodic_CPMG(i,shiftConverter,dimSize));
        }
    }
}

template <bool mirror, bool nimp>
__global__ void bcMap_testExecTime(int *pos, int size, int shiftConverter, int dimSize)
{    
    // identify thread's walker
    int threaId = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = pow(2,shiftConverter);
    int p;
    if(threaId < size)
    {
        for(int i = -3*dimSize*stride; i < 3*dimSize*stride; i++)
        {
            if(mirror && nimp) 
            { 
                p = bcMapMirror_CPMG(i,shiftConverter,dimSize);
            }
            else if(mirror && !nimp) 
            {
                p = bcMapMirror_CPMG_old(i,shiftConverter,dimSize);
            }
            else if(!mirror && nimp) 
            {
                p = bcMapPeriodic_CPMG(i,shiftConverter,dimSize);
            } 
            else
            {
                p = bcMapPeriodic_CPMG_old(i,shiftConverter,dimSize);
            }
        }
    
        if(mirror && nimp) 
        { 
            p = bcMapMirror_CPMG(pos[threaId],shiftConverter,dimSize);
        }
        else if(mirror && !nimp) 
        {
            p = bcMapMirror_CPMG_old(pos[threaId],shiftConverter,dimSize);
        }
        else if(!mirror && nimp) 
        {
            p = bcMapPeriodic_CPMG(pos[threaId],shiftConverter,dimSize);
        } 
        else
        {
            p = bcMapPeriodic_CPMG_old(pos[threaId],shiftConverter,dimSize);
        }

        pos[threaId] = p;
    }
}

__global__ void triLinInterp(int *walker_px,
                             int *walker_py,
                             int *walker_pz,
                             const double *field,
                             const uint64_t *bitBlock,
                             const uint bitBlockColumns,
                             const uint bitBlockRows,
                             const uint numberOfWalkers,
                             const int imageColumns,
                             const int imageRows,
                             const int imageDepth,
                             const uint shift_convert);

template<bool mirror>
__global__ void CPMG_walk(int *walker_px,
                          int *walker_py, 
                          int *walker_pz, 
                          double *decreaseFactor, 
                          double *energy, 
                          uint64_t *seed, 
                          const uint64_t *bitBlock, 
                          const uint bitBlockColumns, 
                          const uint bitBlockRows, 
                          const uint numberOfWalkers, 
                          const uint energyArraySize, 
                          const uint echoesPerKernel, 
                          const uint stepsPerEcho, 
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
    double localDFactor;
    uint64_t localSeed;

    // thread variables for future movements
    int localNextX, localNextY, localNextZ;
    direction nextDirection = None;

    // 1st energy array offset
    // in first echo, walker's energy is stored in last echo of previous kernel launch
    uint energy_OFFSET = (echoesPerKernel - 1) * energyArraySize;
    double energyLvl;

    // now begin the "walk" procedure de facto
    if (walkerId < numberOfWalkers)
    {
        // Local variables for unique read from device global memory
        localPosX = walker_px[walkerId];
        localPosY = walker_py[walkerId];
        localPosZ = walker_pz[walkerId];
        localDFactor = decreaseFactor[walkerId];
        localSeed = seed[walkerId];
        energyLvl = energy[walkerId + energy_OFFSET];

        for (int echo = 0; echo < echoesPerKernel; echo++)
        {
            // update the offset
            energy_OFFSET = echo * energyArraySize;

            for (int step = 0; step < stepsPerEcho; step++)
            {          
        
                nextDirection = computeNextDirection_CPMG(localSeed); 
                computeNextPosition_CPMG(localPosX, localPosY, localPosZ, nextDirection, localNextX, localNextY, localNextZ);

                // update img position
                if(mirror)
                {
                    imgPosX = bcMapMirror_CPMG(localNextX, shiftConverter, imageColumns);
                    imgPosY = bcMapMirror_CPMG(localNextY, shiftConverter, imageRows);
                    imgPosZ = bcMapMirror_CPMG(localNextZ, shiftConverter, imageDepth);
                }
                else
                {
                    imgPosX = bcMapPeriodic_CPMG(localNextX, shiftConverter, imageColumns);
                    imgPosY = bcMapPeriodic_CPMG(localNextY, shiftConverter, imageRows);
                    imgPosZ = bcMapPeriodic_CPMG(localNextZ, shiftConverter, imageDepth);
                }

                if (checkNextPosition_CPMG(imgPosX, imgPosY, imgPosZ, bitBlock, bitBlockColumns, bitBlockRows))
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
                    energyLvl = energyLvl * localDFactor;
                }

            }

            // walker's energy device global memory update
            // must be done for each echo
            energy[walkerId + energy_OFFSET] = energyLvl;
        }

        // position and seed device global memory update
        // must be done for each kernel
        walker_px[walkerId] = localPosX;
        walker_py[walkerId] = localPosY;
        walker_pz[walkerId] = localPosZ;
        seed[walkerId] = localSeed;
    }
}


template<bool mirror>
__global__ void CPMG_walk_field(int *walker_px,
                                int *walker_py,
                                int *walker_pz,
                                double *decreaseFactor,
                                double *energy,
                                double *pAlive,
                                double *phase,
                                uint64_t *seed,
                                const uint64_t *bitBlock,
                                const uint bitBlockColumns,
                                const uint bitBlockRows,
                                const uint numberOfWalkers,
                                const uint energyArraySize,
                                const uint echoesPerKernel,
                                const uint stepsPerEcho,
                                const int imageColumns,
                                const int imageRows,
                                const int imageDepth,
                                const uint shiftConverter,
                                const double gamma,
                                const double timeInterval,
                                const double *field)
{
    // identify thread's walker
    int walkerId = threadIdx.x + blockIdx.x * blockDim.x;

    // Local variables for unique read from device global memory
    int localPosX, localPosY, localPosZ;
    int imgPosX, imgPosY, imgPosZ;
    double localDFactor;
    uint64_t localSeed;

    // thread variables for future movements
    int localNextX, localNextY, localNextZ;
    int imgNextX, imgNextY, imgNextZ;
    direction nextDirection = None;

    // 1st energy array offset
    // in first echo, walker's energy is stored in last echo of previous kernel launch
    uint energy_OFFSET = (echoesPerKernel - 1) * energyArraySize;
    double energyLvl;

    // thread variables for phase computation
    double local_pAlive;
    double local_phase;
    double gammatau = 0.5*gamma*timeInterval;
    uint stepsPerInversion = stepsPerEcho / 2;
    double fieldVal;
    int rowScale = imageColumns;
    int depthScale = imageColumns * imageRows;
    int stride = pow(2,shiftConverter);
    Neighborhood n;
    InterpCube ic;    

    // now begin the "walk" procedure de facto
    if (walkerId < numberOfWalkers)
    {
        // Local variables for unique read from device global memory
        localPosX = walker_px[walkerId];
        localPosY = walker_py[walkerId];
        localPosZ = walker_pz[walkerId];
        localDFactor = decreaseFactor[walkerId];
        localSeed = seed[walkerId];
        local_pAlive = pAlive[walkerId];
        local_phase = phase[walkerId];
        energyLvl = energy[walkerId + energy_OFFSET];

        // get starting img position
        if(mirror)
        {
            imgPosX = bcMapMirror_CPMG(localPosX, shiftConverter, imageColumns);
            imgPosY = bcMapMirror_CPMG(localPosY, shiftConverter, imageRows);
            imgPosZ = bcMapMirror_CPMG(localPosZ, shiftConverter, imageDepth);
        }
        else
        {
            imgPosX = bcMapPeriodic_CPMG(localPosX, shiftConverter, imageColumns);
            imgPosY = bcMapPeriodic_CPMG(localPosY, shiftConverter, imageRows);
            imgPosZ = bcMapPeriodic_CPMG(localPosZ, shiftConverter, imageDepth);
        }

        // get field idx at starting point
        fieldVal = field[getFieldIndex(imgPosX, imgPosY, imgPosZ, rowScale, depthScale)];
         
        // Start random walks            
        for (int echo = 0; echo < echoesPerKernel; echo++)
        {
            // update the offset
            energy_OFFSET = echo * energyArraySize;

            for(uint inv = 0; inv < 2; inv++)
            {
                // phase inversion
                local_phase = -local_phase;

                for (int step = 0; step < stepsPerInversion; step++)
                {
                    // update phase at starting point
                    local_phase += gammatau * fieldVal;
                    
                    // compute next direction            
                    nextDirection = computeNextDirection_CPMG(localSeed); 
                    computeNextPosition_CPMG(localPosX, localPosY, localPosZ, nextDirection, localNextX, localNextY, localNextZ);

                    // update img next position 
                    if(mirror)
                    {
                        imgNextX = bcMapMirror_CPMG(localNextX, shiftConverter, imageColumns);
                        imgNextY = bcMapMirror_CPMG(localNextY, shiftConverter, imageRows);
                        imgNextZ = bcMapMirror_CPMG(localNextZ, shiftConverter, imageDepth);
                    }                   
                    else
                    {
                        imgNextX = bcMapPeriodic_CPMG(localNextX, shiftConverter, imageColumns);
                        imgNextY = bcMapPeriodic_CPMG(localNextY, shiftConverter, imageRows);
                        imgNextZ = bcMapPeriodic_CPMG(localNextZ, shiftConverter, imageDepth);
                    }
                    

                    if (checkNextPosition_CPMG(imgNextX, imgNextY, imgNextZ, bitBlock, bitBlockColumns, bitBlockRows))
                    {
                        // update real position
                        localPosX = localNextX;
                        localPosY = localNextY;
                        localPosZ = localNextZ;
                        
                        // update img position
                        imgPosX = imgNextX;
                        imgPosY = imgNextY;
                        imgPosZ = imgNextZ;

                        // update field at position
                        fieldVal = field[getFieldIndex(imgPosX, imgPosY, imgPosZ, rowScale, depthScale)];                
                    }
                    else
                    {
                        // walker hits wall and comes back to the same position
                        // collisions count is incremented
                        local_pAlive *= localDFactor;
                    }

                    // update phase at finishing point
                    local_phase += gammatau * fieldVal;
                }

            }

            // walker's energy device global memory update
            // must be done for each echo
            energy[walkerId + energy_OFFSET] = local_pAlive * cos(local_phase);
        }

        // position and seed device global memory update
        // must be done for each kernel
        walker_px[walkerId] = localPosX;
        walker_py[walkerId] = localPosY;
        walker_pz[walkerId] = localPosZ;
        seed[walkerId] = localSeed;
        pAlive[walkerId] = local_pAlive;
        phase[walkerId] = local_phase;
    }
}

template <bool mirror>
__global__ void CPMG_walk_field_interp( int *walker_px,
                                        int *walker_py,
                                        int *walker_pz,
                                        double *decreaseFactor,
                                        double *energy,
                                        double *pAlive,
                                        double *phase,
                                        uint64_t *seed,
                                        const uint64_t *bitBlock,
                                        const uint bitBlockColumns,
                                        const uint bitBlockRows,
                                        const uint numberOfWalkers,
                                        const uint energyArraySize,
                                        const uint echoesPerKernel,
                                        const uint stepsPerEcho,
                                        const int imageColumns,
                                        const int imageRows,
                                        const int imageDepth,
                                        const uint shiftConverter,
                                        const double gamma,
                                        const double timeInterval,
                                        const double *field)
{
    // identify thread's walker
    int walkerId = threadIdx.x + blockIdx.x * blockDim.x;

    // Local variables for unique read from device global memory
    int localPosX, localPosY, localPosZ;
    int imgPosX, imgPosY, imgPosZ;
    double localDFactor;
    uint64_t localSeed;

    // thread variables for future movements
    int localNextX, localNextY, localNextZ;
    int imgNextX, imgNextY, imgNextZ;
    direction nextDirection = None;

    // 1st energy array offset
    // in first echo, walker's energy is stored in last echo of previous kernel launch
    uint energy_OFFSET = (echoesPerKernel - 1) * energyArraySize;
    double energyLvl;

    // thread variables for phase computation
    double local_pAlive;
    double local_phase;
    double gammatau = 0.5*gamma*timeInterval;
    uint stepsPerInversion = stepsPerEcho / 2;
    double fieldVal;
    int rowScale = imageColumns;
    int depthScale = imageColumns * imageRows;
    int stride = pow(2,shiftConverter);
    Neighborhood n;
    InterpCube ic;    

    // now begin the "walk" procedure de facto
    if (walkerId < numberOfWalkers)
    {
        // Local variables for unique read from device global memory
        localPosX = walker_px[walkerId];
        localPosY = walker_py[walkerId];
        localPosZ = walker_pz[walkerId];
        localDFactor = decreaseFactor[walkerId];
        localSeed = seed[walkerId];
        local_pAlive = pAlive[walkerId];
        local_phase = phase[walkerId];
        energyLvl = energy[walkerId + energy_OFFSET];

        // get starting img position
        if(mirror)
        {
            imgPosX = bcMapMirror_CPMG(localPosX, shiftConverter, imageColumns);
            imgPosY = bcMapMirror_CPMG(localPosY, shiftConverter, imageRows);
            imgPosZ = bcMapMirror_CPMG(localPosZ, shiftConverter, imageDepth);
        }
        else
        {
            imgPosX = bcMapPeriodic_CPMG(localPosX, shiftConverter, imageColumns);
            imgPosY = bcMapPeriodic_CPMG(localPosY, shiftConverter, imageRows);
            imgPosZ = bcMapPeriodic_CPMG(localPosZ, shiftConverter, imageDepth);
        }

        // get field idx at starting point
        if(mirror) 
        {
            fieldVal = triLinInterp_mirror(field, localPosX, localPosY, localPosZ, stride, shiftConverter, imageColumns, imageRows, imageDepth, n, ic);
            if(walkerId == 0) printf("\nf: %f", fieldVal);
        } 
        else 
        {
            fieldVal = triLinInterp_periodic(field, localPosX, localPosY, localPosZ, stride, shiftConverter, imageColumns, imageRows, imageDepth, n, ic);
            if(walkerId == 0) printf("\nf: %f", fieldVal);
        }
         
        // Start random walks            
        for (int echo = 0; echo < echoesPerKernel; echo++)
        {
            // update the offset
            energy_OFFSET = echo * energyArraySize;

            for(uint inv = 0; inv < 2; inv++)
            {
                // phase inversion
                local_phase = -local_phase;

                for (int step = 0; step < stepsPerInversion; step++)
                {
                    // update phase at starting point
                    local_phase += gammatau * fieldVal;
                    
                    // compute next direction            
                    nextDirection = computeNextDirection_CPMG(localSeed); 
                    computeNextPosition_CPMG(localPosX, localPosY, localPosZ, nextDirection, localNextX, localNextY, localNextZ);

                    // update img next position 
                    if(mirror)
                    {
                        imgNextX = bcMapMirror_CPMG(localNextX, shiftConverter, imageColumns);
                        imgNextY = bcMapMirror_CPMG(localNextY, shiftConverter, imageRows);
                        imgNextZ = bcMapMirror_CPMG(localNextZ, shiftConverter, imageDepth);
                    }                   
                    else
                    {
                        imgNextX = bcMapPeriodic_CPMG(localNextX, shiftConverter, imageColumns);
                        imgNextY = bcMapPeriodic_CPMG(localNextY, shiftConverter, imageRows);
                        imgNextZ = bcMapPeriodic_CPMG(localNextZ, shiftConverter, imageDepth);
                    }
                    

                    if (checkNextPosition_CPMG(imgNextX, imgNextY, imgNextZ, bitBlock, bitBlockColumns, bitBlockRows))
                    {
                        // update real position
                        localPosX = localNextX;
                        localPosY = localNextY;
                        localPosZ = localNextZ;
                        
                        // update img position
                        imgPosX = imgNextX;
                        imgPosY = imgNextY;
                        imgPosZ = imgNextZ;

                        // update field at position
                        // if(mirror) 
                        // {
                        //     fieldVal = triLinInterp_mirror(field, localPosX, localPosY, localPosZ, stride, shiftConverter, imageColumns, imageRows, imageDepth, n, ic);
                        // } 
                        // else 
                        // {
                        //     fieldVal = triLinInterp_periodic(field, localPosX, localPosY, localPosZ, stride, shiftConverter, imageColumns, imageRows, imageDepth, n, ic);
                        // }                   
                    }
                    else
                    {
                        // walker hits wall and comes back to the same position
                        // collisions count is incremented
                        local_pAlive *= localDFactor;
                    }

                    // update phase at finishing point
                    local_phase += gammatau * fieldVal;
                }

            }

            // walker's energy device global memory update
            // must be done for each echo
            energy[walkerId + energy_OFFSET] = local_pAlive * cos(local_phase);
        }

        // position and seed device global memory update
        // must be done for each kernel
        walker_px[walkerId] = localPosX;
        walker_py[walkerId] = localPosY;
        walker_pz[walkerId] = localPosZ;
        seed[walkerId] = localSeed;
        pAlive[walkerId] = local_pAlive;
        phase[walkerId] = local_phase;
    }
}



// GPU kernel for reducing energy array into a global energy vector
__global__ void CPMG_energyReduce(double *energy,
                                  double *collector,
                                  const uint energyArraySize,
                                  const uint collectorSize,
                                  const uint echoesPerKernel)
{
    extern __shared__ double sharedData[];

    // each thread loads two elements from global to shared mem
    uint threadId = threadIdx.x;
    uint globalId = threadIdx.x + blockIdx.x * (blockDim.x * 2);
    uint OFFSET;

    for (int echo = 0; echo < echoesPerKernel; echo++)
    {
        OFFSET = echo * energyArraySize;

        sharedData[threadId] = energy[globalId + OFFSET] + energy[globalId + blockDim.x + OFFSET];
        __syncthreads();

        // do reduction in shared mem
        for (uint stride = blockDim.x / 2; stride > 0; stride >>= 1)
        {
            if (threadId < stride)
            {
                sharedData[threadId] += sharedData[threadId + stride];
            }
            __syncthreads();
        }

        // write result for this block to global mem
        if (threadId == 0)
        {
            collector[blockIdx.x + (echo * collectorSize)] = sharedData[0];
        }
        __syncthreads();
    }
}

__global__ void triLinInterp(int *walker_px,
                             int *walker_py,
                             int *walker_pz,
                             const double *field,
                             const uint64_t *bitBlock,
                             const uint bitBlockColumns,
                             const uint bitBlockRows,
                             const uint numberOfWalkers,
                             const int imageColumns,
                             const int imageRows,
                             const int imageDepth,
                             const uint shiftConverter)
{
    // identify thread's walker
    int walkerId = threadIdx.x + blockIdx.x * blockDim.x;
    Neighborhood n; 
    InterpCube ic;
    double value;
    double value2;
    int imgPosX;
    int imgPosY;
    int imgPosZ;
    int stride = pow(2,shiftConverter);
    int rowScale = imageColumns;
    int depthScale = imageColumns * imageRows;
    
    if (walkerId < numberOfWalkers)
    {        
        imgPosX = bcMapPeriodic_CPMG(walker_px[walkerId], shiftConverter, imageColumns);
        imgPosY = bcMapPeriodic_CPMG(walker_py[walkerId], shiftConverter, imageRows);
        imgPosZ = bcMapPeriodic_CPMG(walker_pz[walkerId], shiftConverter, imageDepth);
        getNeighbors_periodic(n, walker_px[walkerId], walker_py[walkerId], walker_pz[walkerId], 
                              stride, shiftConverter, imageColumns, imageRows, imageDepth);
        getInterpCube(ic, n, field, rowScale, depthScale);
        value = interpValues(ic, walker_px[walkerId], walker_py[walkerId], walker_pz[walkerId], stride);
        value2 = triLinInterp_periodic(field, walker_px[walkerId], walker_py[walkerId], walker_pz[walkerId], 
                                       stride, shiftConverter, imageColumns, imageRows, imageDepth, n, ic);
        
        printf("Converter: %d, stride: %d, rs: %d, ds: %d", shiftConverter, stride, rowScale, depthScale);    
        printf("\nPeriodic BC:");
        printf("\nHi, I'm walker #%d at {%d -> %d, %d -> %d, %d -> %d} \n", walkerId, 
                                                                            walker_px[walkerId], imgPosX, 
                                                                            walker_py[walkerId], imgPosY,
                                                                            walker_pz[walkerId], imgPosZ);
        double xd = (0.5 + ((double) ((walker_px[walkerId]-(stride/2)) & (stride-1))))/stride;
        double yd = (0.5 + ((double) ((walker_py[walkerId]-(stride/2)) & (stride-1))))/stride;
        double zd = (0.5 + ((double) ((walker_pz[walkerId]-(stride/2)) & (stride-1))))/stride;
                                                                                
        printf("interp data => (%f,%f,%f) %f\t%f\n",xd,yd,zd,value,value2);
        printf("\nneighbor 000 at {%d, %d, %d}", n.p000.x, n.p000.y, n.p000.z); printf("=> d[%ld]=%f (%f)\n", ic.i000, ic.c000, field[ic.i000]);
        printf("\nneighbor 100 at {%d, %d, %d}", n.p100.x, n.p100.y, n.p100.z); printf("=> d[%ld]=%f (%f)\n", ic.i100, ic.c100, field[ic.i100]);
        printf("\nneighbor 010 at {%d, %d, %d}", n.p010.x, n.p010.y, n.p010.z); printf("=> d[%ld]=%f (%f)\n", ic.i010, ic.c010, field[ic.i010]);
        printf("\nneighbor 110 at {%d, %d, %d}", n.p110.x, n.p110.y, n.p110.z); printf("=> d[%ld]=%f (%f)\n", ic.i110, ic.c110, field[ic.i110]);
        printf("\nneighbor 001 at {%d, %d, %d}", n.p001.x, n.p001.y, n.p001.z); printf("=> d[%ld]=%f (%f)\n", ic.i001, ic.c001, field[ic.i001]);
        printf("\nneighbor 101 at {%d, %d, %d}", n.p101.x, n.p101.y, n.p101.z); printf("=> d[%ld]=%f (%f)\n", ic.i101, ic.c101, field[ic.i101]);
        printf("\nneighbor 011 at {%d, %d, %d}", n.p011.x, n.p011.y, n.p011.z); printf("=> d[%ld]=%f (%f)\n", ic.i011, ic.c011, field[ic.i011]); 
        printf("\nneighbor 111 at {%d, %d, %d}", n.p111.x, n.p111.y, n.p111.z); printf("=> d[%ld]=%f (%f)\n", ic.i111, ic.c111, field[ic.i111]);

        // printf("\n");   
        // for(int i = -10; i < 10; i++)
        // {
        //     double pos = (double) ((i-(stride/2)) & (stride-1));
        //     double div = (0.5+pos)/stride;
        //     printf("pos %d => %f ==> %f\n", i, pos, div);
        // }


        double comp = (double) (shiftConverter != 0);
        printf("\ncomp = %f\n", comp);
    }
}

// Host functions
void CPMG_reduce_omp(double *temp_collector, double *array, int numberOfEchoes, uint arraySizePerEcho);

#endif
