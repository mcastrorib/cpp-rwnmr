#include "NMR_mapping_cuda.h"

// function to call GPU kernel to execute
// walker's "map" method in Graphics Processing Unit
void Model::mapSimulation_CUDA_3D_histograms(bool reset)
{   
    string bc = (*this).getBoundaryCondition();

    cout << "- starting 3DRW-Mapping simulation (in GPU) [bc:" << bc << "]...";
        
    // reset walkers
    if(reset)
    {
        if(this->rwNMR_config.getOpenMPUsage())
        {
            // set omp variables for parallel loop throughout walker list
            const int num_cpu_threads = omp_get_max_threads();
            const int loop_size = this->getWalkers()->size();
            int loop_start, loop_finish;

            #pragma omp parallel shared(walkers) private(loop_start, loop_finish) 
            {
                const int thread_id = omp_get_thread_num();
                ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                loop_start = looper.getStart();
                loop_finish = looper.getFinish(); 

                for (uint id = loop_start; id < loop_finish; id++)
                {
                    (*this->getWalkers())[id].resetPosition();
                    (*this->getWalkers())[id].resetSeed();
                    (*this->getWalkers())[id].resetCollisions();
                    (*this->getWalkers())[id].resetTCollisions();
                }
            }
        } else
        {
            for (uint id = 0; id < this->getWalkers()->size(); id++)
            {
                (*this->getWalkers())[id].resetPosition();
                (*this->getWalkers())[id].resetSeed();
                (*this->getWalkers())[id].resetCollisions();
                (*this->getWalkers())[id].resetTCollisions();
            }
        }
    }

    // CUDA event recorder to measure computation time in device
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    // integer values
    uint bitBlockColumns = this->bitBlock->getBlockColumns();
    uint bitBlockRows = this->bitBlock->getBlockRows();
    uint numberOfBitBlocks = this->bitBlock->getNumberOfBlocks();
    uint numberOfWalkers = this->numberOfWalkers;
    int map_columns = this->bitBlock->getImageColumns();
    int map_rows = this->bitBlock->getImageRows();
    int map_depth = this->bitBlock->getImageDepth();
    uint shiftConverter = log2(this->voxelDivision);

    // define parameters for CUDA kernel launch: blockDim, gridDim etc
    uint threadsPerBlock = this->rwNMR_config.getThreadsPerBlock();
    uint blocksPerKernel = this->rwNMR_config.getBlocks();
    uint walkersPerKernel = threadsPerBlock * blocksPerKernel;
    if (numberOfWalkers < walkersPerKernel)
    {
        blocksPerKernel = (int)ceil((double)(numberOfWalkers) / (double)(threadsPerBlock));
        walkersPerKernel = threadsPerBlock * blocksPerKernel;
    }
    uint numberOfWalkerPacks = (numberOfWalkers / walkersPerKernel) + 1;
    uint lastWalkerPackSize = numberOfWalkers % walkersPerKernel;

    // bitBlock3D host to device copy
    // assign pointer to bitBlock datastructure
    uint64_t *bitBlock;
    bitBlock = this->bitBlock->getBlocks();

    // copy host bitblock data to temporary host arrays
    uint64_t *d_bitBlock;
    cudaMalloc((void **)&d_bitBlock, numberOfBitBlocks * sizeof(uint64_t));
    cudaMemcpy(d_bitBlock, bitBlock, numberOfBitBlocks * sizeof(uint64_t), cudaMemcpyHostToDevice);

    // Device memory allocation
    // pointers used in array conversion and their host memory allocation
    int *walker_px = MemAllocator::mallocIntArray(walkersPerKernel);
    int *walker_py = MemAllocator::mallocIntArray(walkersPerKernel);
    int *walker_pz = MemAllocator::mallocIntArray(walkersPerKernel);
    uint *collisions = MemAllocator::mallocUIntArray(walkersPerKernel);
    uint64_t *seed = MemAllocator::mallocUInt64Array(walkersPerKernel);

    // Device memory allocation
    // Declaration of device data arrays
    int *d_walker_px;
    int *d_walker_py;
    int *d_walker_pz;
    uint *d_collisions;
    uint64_t *d_seed;

    // alloc memory in device for data arrays
    cudaMalloc((void **)&d_walker_px, walkersPerKernel * sizeof(int));
    cudaMalloc((void **)&d_walker_py, walkersPerKernel * sizeof(int));
    cudaMalloc((void **)&d_walker_pz, walkersPerKernel * sizeof(int));
    cudaMalloc((void **)&d_collisions, walkersPerKernel * sizeof(uint));
    cudaMalloc((void **)&d_seed, walkersPerKernel * sizeof(uint64_t));

    // initialize histograms
    (*this).initHistogramList();

    // loop throughout histogram list
    for(int hst_ID = 0; hst_ID < this->histogramList.size(); hst_ID++)
    {
        // set steps for each histogram
        uint eBegin = this->histogramList[hst_ID].getFirstEcho();
        uint eEnd = this->histogramList[hst_ID].getLastEcho();
        uint steps = this->stepsPerEcho * (eEnd - eBegin);

        // create a steps bucket
        uint stepsLimit = this->rwNMR_config.getMaxRWSteps();
        uint stepsSize = steps/stepsLimit;
        vector<uint> stepsList;
        for(uint idx = 0; idx < stepsSize; idx++)
        {
            stepsList.push_back(stepsLimit);
        }
        // charge rebalance
        if((steps % stepsLimit) > 0)
        {
            stepsSize++;
            stepsList.push_back(steps%stepsLimit);
        } 

        for (uint packId = 0; packId < (numberOfWalkerPacks - 1); packId++)
        {
            // set offset in walkers vector
            uint packOffset = packId * walkersPerKernel;
    
            // Host data copy
            // copy original walkers' data to temporary host arrays
            if(this->rwNMR_config.getOpenMPUsage())
            {
                // set omp variables for parallel loop throughout walker list
                const int num_cpu_threads = omp_get_max_threads();
                const int loop_size = walkersPerKernel;
                int loop_start, loop_finish;

                #pragma omp parallel shared(packOffset, walker_px, walker_py, walker_pz, collisions, seed, walkers) private(loop_start, loop_finish) 
                {
                    const int thread_id = omp_get_thread_num();
                    ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                    loop_start = looper.getStart();
                    loop_finish = looper.getFinish(); 

                    for (uint id = loop_start; id < loop_finish; id++)
                    {
                        walker_px[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionX();
                        walker_py[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionY();
                        walker_pz[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionZ();
                        collisions[id] = 0;
                        seed[id] = (*this->getWalkers())[id + packOffset].getCurrentSeed();
                    }
                }
            } else
            {
                for (uint id = 0; id < walkersPerKernel; id++)
                {
                    walker_px[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionX();
                    walker_py[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionY();
                    walker_pz[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionZ();
                    collisions[id] = 0;
                    seed[id] = (*this->getWalkers())[id + packOffset].getCurrentSeed();
                }
            }
    
            // Device data copy
            // copy host data to device
            cudaMemcpy(d_walker_px, walker_px, walkersPerKernel * sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_walker_py, walker_py, walkersPerKernel * sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_walker_pz, walker_pz, walkersPerKernel * sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_collisions, collisions, walkersPerKernel * sizeof(uint), cudaMemcpyHostToDevice);
            cudaMemcpy(d_seed, seed, walkersPerKernel * sizeof(uint64_t), cudaMemcpyHostToDevice);
    
            //////////////////////////////////////////////////////////////////////
            // Launch kernel for GPU computation
            // kernel "map" launch
            for(uint sIdx = 0; sIdx < stepsList.size(); sIdx++)
            {
                if(bc == "mirror")
                {
                    rwMap<true><<<blocksPerKernel, threadsPerBlock>>>(d_walker_px,
                                                                      d_walker_py,
                                                                      d_walker_pz,
                                                                      d_collisions,
                                                                      d_seed,
                                                                      d_bitBlock,
                                                                      bitBlockColumns,
                                                                      bitBlockRows,
                                                                      walkersPerKernel,
                                                                      stepsList[sIdx],
                                                                      map_columns,
                                                                      map_rows,
                                                                      map_depth,
                                                                      shiftConverter);
                }
                else 
                {
                    rwMap<false><<<blocksPerKernel, threadsPerBlock>>>(d_walker_px,
                                                                       d_walker_py,
                                                                       d_walker_pz,
                                                                       d_collisions,
                                                                       d_seed,
                                                                       d_bitBlock,
                                                                       bitBlockColumns,
                                                                       bitBlockRows,
                                                                       walkersPerKernel,
                                                                       stepsList[sIdx],
                                                                       map_columns,
                                                                       map_rows,
                                                                       map_depth,
                                                                       shiftConverter);
                
                } 
                
                cudaDeviceSynchronize();
            }

            // Host data copy
            // copy device data to host
            cudaMemcpy(collisions, d_collisions, walkersPerKernel * sizeof(uint), cudaMemcpyDeviceToHost);            
            cudaMemcpy(walker_px, d_walker_px, walkersPerKernel * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_py, d_walker_py, walkersPerKernel * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_pz, d_walker_pz, walkersPerKernel * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(seed, d_seed, walkersPerKernel * sizeof(uint64_t), cudaMemcpyDeviceToHost);
            
            // copy collisions host data to class members
            if(this->rwNMR_config.getOpenMPUsage())
            {
                // set omp variables for parallel loop throughout walker list
                const int num_cpu_threads = omp_get_max_threads();
                const int loop_size = walkersPerKernel;
                int loop_start, loop_finish;

                #pragma omp parallel shared(packOffset, walker_px, walker_py, walker_pz, collisions, seed, walkers) private(loop_start, loop_finish) 
                {
                    const int thread_id = omp_get_thread_num();
                    ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                    loop_start = looper.getStart();
                    loop_finish = looper.getFinish(); 

                    for (uint id = loop_start; id < loop_finish; id++)
                    {
                        (*this->getWalkers())[id + packOffset].setCollisions(collisions[id]);
                        (*this->getWalkers())[id + packOffset].setCurrentPositionX(walker_px[id]);
                        (*this->getWalkers())[id + packOffset].setCurrentPositionY(walker_py[id]);
                        (*this->getWalkers())[id + packOffset].setCurrentPositionZ(walker_pz[id]); 
                        (*this->getWalkers())[id + packOffset].setCurrentSeed(seed[id]);
                    }
                }
            } else
            {
                
                for (uint id = 0; id < walkersPerKernel; id++)
                {
                    (*this->getWalkers())[id + packOffset].setCollisions(collisions[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentPositionX(walker_px[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentPositionY(walker_py[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentPositionZ(walker_pz[id]); 
                    (*this->getWalkers())[id + packOffset].setCurrentSeed(seed[id]);
                }
            }
    
            
        }
    
        if (lastWalkerPackSize > 0)
        { 
            // last pack is done explicitly
            // set offset in walkers vector
            uint packOffset = (numberOfWalkerPacks - 1) * walkersPerKernel;
    
            // Host data copy
            // copy original walkers' data to temporary host arrays
            if(this->rwNMR_config.getOpenMPUsage())
            {
                // set omp variables for parallel loop throughout walker list
                const int num_cpu_threads = omp_get_max_threads();
                const int loop_size = lastWalkerPackSize;
                int loop_start, loop_finish;

                #pragma omp parallel shared(packOffset, walker_px, walker_py, walker_pz, collisions, seed, walkers) private(loop_start, loop_finish) 
                {
                    const int thread_id = omp_get_thread_num();
                    ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                    loop_start = looper.getStart();
                    loop_finish = looper.getFinish(); 

                    for (uint id = loop_start; id < loop_finish; id++)
                    {
                        walker_px[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionX();
                        walker_py[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionY();
                        walker_pz[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionZ();
                        collisions[id] = 0;
                        seed[id] = (*this->getWalkers())[id + packOffset].getCurrentSeed();
                    }
                }
            } else
            {
                for (uint id = 0; id < lastWalkerPackSize; id++)
                {
                    walker_px[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionX();
                    walker_py[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionY();
                    walker_pz[id] = (*this->getWalkers())[id + packOffset].getCurrentPositionZ();
                    collisions[id] = 0;
                    seed[id] = (*this->getWalkers())[id + packOffset].getCurrentSeed();
                }
            }
    
            // Device data copy
            // copy host data to device
            cudaMemcpy(d_walker_px, walker_px, lastWalkerPackSize * sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_walker_py, walker_py, lastWalkerPackSize * sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_walker_pz, walker_pz, lastWalkerPackSize * sizeof(int), cudaMemcpyHostToDevice);
            cudaMemcpy(d_collisions, collisions, lastWalkerPackSize * sizeof(uint), cudaMemcpyHostToDevice);
            cudaMemcpy(d_seed, seed, lastWalkerPackSize * sizeof(uint64_t), cudaMemcpyHostToDevice);
    
            //////////////////////////////////////////////////////////////////////
            // Launch kernel for GPU computation
            // kernel "map" launch
            for(uint sIdx = 0; sIdx < stepsList.size(); sIdx++)
            {
                if(bc == "mirror")
                {
                    rwMap<true><<<blocksPerKernel, threadsPerBlock>>>(d_walker_px,
                                                                      d_walker_py,
                                                                      d_walker_pz,
                                                                      d_collisions,
                                                                      d_seed,
                                                                      d_bitBlock,
                                                                      bitBlockColumns,
                                                                      bitBlockRows,
                                                                      lastWalkerPackSize,
                                                                      stepsList[sIdx],
                                                                      map_columns,
                                                                      map_rows,
                                                                      map_depth,
                                                                      shiftConverter);
                }
                else 
                {
                    rwMap<false><<<blocksPerKernel, threadsPerBlock>>>(d_walker_px,
                                                                       d_walker_py,
                                                                       d_walker_pz, 
                                                                       d_collisions, 
                                                                       d_seed, 
                                                                       d_bitBlock, 
                                                                       bitBlockColumns, 
                                                                       bitBlockRows, 
                                                                       lastWalkerPackSize, 
                                                                       stepsList[sIdx], 
                                                                       map_columns, 
                                                                       map_rows, 
                                                                       map_depth, 
                                                                       shiftConverter);
                }
                cudaDeviceSynchronize();
            }
    
            // Host data copy
            // copy device data to host
            cudaMemcpy(collisions, d_collisions, lastWalkerPackSize * sizeof(uint), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_px, d_walker_px, lastWalkerPackSize * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_py, d_walker_py, lastWalkerPackSize * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_pz, d_walker_pz, lastWalkerPackSize * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(seed, d_seed, lastWalkerPackSize * sizeof(uint64_t), cudaMemcpyDeviceToHost);
            
    
            // copy collisions host data to class members
            if(this->rwNMR_config.getOpenMPUsage())
            {
                // set omp variables for parallel loop throughout walker list
                const int num_cpu_threads = omp_get_max_threads();
                const int loop_size = lastWalkerPackSize;
                int loop_start, loop_finish;

                #pragma omp parallel shared(packOffset, walker_px, walker_py, walker_pz, collisions, seed, walkers) private(loop_start, loop_finish) 
                {
                    const int thread_id = omp_get_thread_num();
                    ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                    loop_start = looper.getStart();
                    loop_finish = looper.getFinish(); 

                    for (uint id = loop_start; id < loop_finish; id++)
                    {
                        (*this->getWalkers())[id + packOffset].setCollisions(collisions[id]);
                        (*this->getWalkers())[id + packOffset].setCurrentPositionX(walker_px[id]);
                        (*this->getWalkers())[id + packOffset].setCurrentPositionY(walker_py[id]);
                        (*this->getWalkers())[id + packOffset].setCurrentPositionZ(walker_pz[id]); 
                        (*this->getWalkers())[id + packOffset].setCurrentSeed(seed[id]);
                    }
                }
            } else
            {
                
                for (uint id = 0; id < lastWalkerPackSize; id++)
                {
                    (*this->getWalkers())[id + packOffset].setCollisions(collisions[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentPositionX(walker_px[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentPositionY(walker_py[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentPositionZ(walker_pz[id]);
                    (*this->getWalkers())[id + packOffset].setCurrentSeed(seed[id]);
                }
            }
        }

        // create histogram
        (*this).createHistogram(hst_ID, steps);

        // reset collision count, but keep summation in alternative count
        if(this->rwNMR_config.getOpenMPUsage())
        {
            // set omp variables for parallel loop throughout walker list
            const int num_cpu_threads = omp_get_max_threads();
            const int loop_size = this->numberOfWalkers;
            int loop_start, loop_finish;

            #pragma omp parallel shared(walkers) private(loop_start, loop_finish) 
            {
                const int thread_id = omp_get_thread_num();
                ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                loop_start = looper.getStart();
                loop_finish = looper.getFinish(); 

                for (uint id = loop_start; id < loop_finish; id++)
                {
                    (*this->getWalkers())[id].setTCollisions((*this->getWalkers())[id].getTCollisions() + (*this->getWalkers())[id].getCollisions());
                    (*this->getWalkers())[id].resetCollisions();
                }
            }
        } else
        {
            for (uint id = 0; id < this->numberOfWalkers; id++)
            {
                (*this->getWalkers())[id].setTCollisions((*this->getWalkers())[id].getTCollisions() + (*this->getWalkers())[id].getCollisions());
                (*this->getWalkers())[id].resetCollisions();
            }
        }
    }
    // histogram loop is finished

    // recover walkers collisions from total sum and create a global histogram
    if(this->rwNMR_config.getOpenMPUsage())
    {
        // set omp variables for parallel loop throughout walker list
        const int num_cpu_threads = omp_get_max_threads();
        const int loop_size = this->numberOfWalkers;
        int loop_start, loop_finish;

        #pragma omp parallel shared(walkers) private(loop_start, loop_finish) 
        {
            const int thread_id = omp_get_thread_num();
            ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
            loop_start = looper.getStart();
            loop_finish = looper.getFinish(); 

            for (uint id = loop_start; id < loop_finish; id++)
            {
                (*this->getWalkers())[id].setCollisions((*this->getWalkers())[id].getTCollisions());
            }
        }

    } else
    {
        for (uint id = 0; id < this->numberOfWalkers; id++)
        {
            (*this->getWalkers())[id].setCollisions((*this->getWalkers())[id].getTCollisions());   
        }
    }

    // create collision histogram
    (*this).createHistogram();

    // free pointers in host
    free(walker_px);
    free(walker_py);
    free(walker_pz);
    free(collisions);
    free(seed);

    // and direct them to NULL
    walker_px = NULL;
    walker_py = NULL;
    walker_pz = NULL;
    collisions = NULL;
    seed = NULL;

    // also direct the bitBlock pointer created in this context
    // (original data is kept safe)
    bitBlock = NULL;

    // free device global memory
    cudaFree(d_walker_px);
    cudaFree(d_walker_py);
    cudaFree(d_walker_pz);
    cudaFree(d_collisions);
    cudaFree(d_seed);
    cudaFree(d_bitBlock);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop); 
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    cout << "Done.\nelapsed time: " << elapsedTime * 1.0e-3 << " seconds" << endl;    
}

/*

Device functions

*/

__device__ direction computeNextDirection_MAP(uint64_t &seed)
{
    // generate random number using xorshift algorithm
    xorshift64_state xor_state;
    xor_state.a = seed;
    seed = xorShift64_MAP(&xor_state);
    uint64_t rand = seed;

    // set direction based on the random number
    direction nextDirection = (direction)(mod6_MAP(rand) + 1);
    return nextDirection;
}

__device__ uint64_t xorShift64_MAP(struct xorshift64_state *state)
{
    uint64_t x = state->a;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return state->a = x;
}

__device__ uint64_t mod6_MAP(uint64_t a)
{
    while (a > 11)
    {
        int s = 0; /* accumulator for the sum of the digits */
        while (a != 0)
        {
            s = s + (a & 7);
            a = (a >> 2) & -2;
        }
        a = s;
    }
    /* note, at this point: a < 12 */
    if (a > 5)
        a = a - 6;
    return a;
}

__device__ void computeNextPosition_MAP(int &walker_px,
                                       int &walker_py,
                                       int &walker_pz,
                                       direction nextDirection,
                                       int &next_x,
                                       int &next_y,
                                       int &next_z)
{
    next_x = walker_px;
    next_y = walker_py;
    next_z = walker_pz;

    switch (nextDirection)
    {
    case North:
        next_y = next_y - 1;
        break;

    case West:
        next_x = next_x - 1;
        break;

    case South:
        next_y = next_y + 1;
        break;

    case East:
        next_x = next_x + 1;
        break;

    case Up:
        next_z = next_z + 1;
        break;

    case Down:
        next_z = next_z - 1;
        break;
    }
}

__device__ bool checkNextPosition_MAP(int next_x,
                                     int next_y,
                                     int next_z,
                                     const uint64_t *bitBlock,
                                     const int bitBlockColumns,
                                     const int bitBlockRows)
{
    int blockIndex = findBlockIndex_MAP(next_x, next_y, next_z, bitBlockColumns, bitBlockRows);
    int nextBit = findBitIndex_MAP(next_x, next_y, next_z);
    uint64_t nextBlock = bitBlock[blockIndex];

    return (!checkIfBlockBitIsWall_MAP(nextBlock, nextBit));
};

__device__ int findBlockIndex_MAP(int next_x, int next_y, int next_z, int bitBlockColumns, int bitBlockRows)
{
    // "x >> 2" is like "x / 4" in bitwise operation
    int block_x = next_x >> 2;
    int block_y = next_y >> 2;
    int block_z = next_z >> 2;
    int blockIndex = block_x + block_y * bitBlockColumns + block_z * (bitBlockColumns * bitBlockRows);

    return blockIndex;
}

__device__ int findBitIndex_MAP(int next_x, int next_y, int next_z)
{
    // "x & (n - 1)" is lise "x % n" in bitwise operation
    int bit_x = next_x & (COLUMNSPERBLOCK3D - 1);
    int bit_y = next_y & (ROWSPERBLOCK3D - 1);
    int bit_z = next_z & (DEPTHPERBLOCK3D - 1);
    // "x << 3" is like "x * 8" in bitwise operation
    int bitIndex = bit_x + (bit_y << 2) + ((bit_z << 2) << 2);

    return bitIndex;
}

__device__ bool checkIfBlockBitIsWall_MAP(uint64_t nextBlock, int nextBit)
{
    return ((nextBlock >> nextBit) & 1ull);
}

__device__ int convertLocalToGlobal_MAP(int _localPos, uint _shiftConverter)
{
    return (_localPos >> _shiftConverter);
}

__device__ int bcMapPeriodic_MAP(int _localPos, uint _shiftConverter, int _dimSize)
{
    int globalPos;
    globalPos = ( convertLocalToGlobal_MAP(_localPos, _shiftConverter) ) % _dimSize;
    globalPos += (((int) (globalPos < 0)) * _dimSize);
    return globalPos;
}

__device__ int bcMapMirror_MAP(int _localPos, uint _shiftConverter, int _dimSize)
{
    int globalPos = convertLocalToGlobal_MAP(_localPos, _shiftConverter);
    int imgPos = (globalPos % _dimSize); 
    imgPos += (((int) (imgPos < 0)) * _dimSize);
    int mirror = ((isPositive_MAP(globalPos)*globalPos) + (((int) !(globalPos > 0)) * (-globalPos-1+_dimSize)))/_dimSize; 
    mirror = (mirror & 1);
    return (((mirror + 1) & 1) * imgPos) + (mirror * (_dimSize - 1 - imgPos));
}

__device__ int isPositive_MAP(int x) 
{
   return !((x&(1<<31)) | !x);
}

__device__ int isZero_MAP(int x) 
{
   return !x;
}
