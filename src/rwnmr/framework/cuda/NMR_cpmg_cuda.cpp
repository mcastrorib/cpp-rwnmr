#include "NMR_cpmg_cuda.h"

__global__ void CPMG_walk_test(void)
{    
    
}

// function to call GPU kernel to execute
// walker's "walk" method in Graphics Processing Unit
void NMR_cpmg::image_simulation_cuda()
{
    // define kernel launch flags
    const bool applyField = (this->internalField != NULL);
    string bc = this->model.getBoundaryCondition();
    const bool bcMirrorFlag = (bc == "mirror");
    const bool interpFlag = (*this).getCpmgConfig().getInterpolateField();
    cout << "- starting RW-CPMG simulation (in GPU) [bc:" << bc;
    if(applyField and (*this).getCpmgConfig().getInterpolateField()) cout << "+3Linterp";
    cout << "]...";
    
    bool time_verbose = this->CPMG_config.getTimeVerbose();
    double reset_time = 0.0;
    double copy_time = 0.0;
    double kernel_time = 0.0;
    double buffer_time = 0.0;
    double reduce_time = 0.0;
    
    double tick = omp_get_wtime();
    if(this->model.getRwnmrConfig().getOpenMPUsage())
    {
        // set omp variables for parallel loop throughout walker list
        const int num_cpu_threads = omp_get_max_threads();
        const int loop_size = this->model.getWalkers()->size();
        int loop_start, loop_finish;

        #pragma omp parallel private(loop_start, loop_finish) 
        {
            const int thread_id = omp_get_thread_num();
            ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
            loop_start = looper.getStart();
            loop_finish = looper.getFinish(); 

            for (uint id = loop_start; id < loop_finish; id++)
            {
                (*this->model.getWalkers())[id].resetPosition();
                (*this->model.getWalkers())[id].resetSeed();
                (*this->model.getWalkers())[id].resetEnergy();
            }
        }
    } else
    {
        // reset walker's initial state 
        for (uint id = 0; id < this->model.getWalkers()->size(); id++)
        {
            (*this->model.getWalkers())[id].resetPosition();
            (*this->model.getWalkers())[id].resetSeed();
            (*this->model.getWalkers())[id].resetEnergy();
        }
    }

    // reset vector to store energy decay
    (*this).resetSignal();
    this->signalAmps.reserve(this->model.getNumberOfEchoes() + 1); // '+1' to accomodate time 0.0

    // get initial energy global state
    double energySum = ((double) this->model.getWalkers()->size()) * (*this->model.getWalkers())[0].getEnergy();
    this->signalAmps.push_back(energySum);

    reset_time += omp_get_wtime() - tick;


    // CUDA event recorder to measure computation time in device
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    // integer values for sizing issues
    uint numberOfWalkers = this->model.getNumberOfWalkers();
    uint bitBlockColumns = this->model.getBitBlock()->getBlockColumns();
    uint bitBlockRows = this->model.getBitBlock()->getBlockRows();
    uint numberOfBitBlocks = this->model.getBitBlock()->getNumberOfBlocks();
    int imageColumns = this->model.getBitBlock()->getImageColumns();
    int imageRows = this->model.getBitBlock()->getImageRows();
    int imageDepth = this->model.getBitBlock()->getImageDepth();
    uint shiftConverter = log2(this->model.getVoxelDivision());

    uint numberOfEchoes = this->model.getNumberOfEchoes();
    uint stepsPerEcho = this->model.getStepsPerEcho();
    uint echoesPerKernel = this->model.getRwnmrConfig().getEchoesPerKernel();
    uint kernelCalls = (uint) ceil(numberOfEchoes / (double) echoesPerKernel);


    // Internal field parameters
    double *field = (*this).getInternalFieldData();
    long fieldSize = (*this).getInternalFieldSize();
    double timeInterval = 1.0e-3 * this->model.getTimeInterval(); 
    double gamma = 1.0e+06 * this->model.getGiromagneticRatio();
    
    // define parameters for CUDA kernel launch: blockDim, gridDim etc
    uint threadsPerBlock = this->model.getRwnmrConfig().getThreadsPerBlock();
    uint blocksPerKernel = this->model.getRwnmrConfig().getBlocks();
    uint walkersPerKernel = threadsPerBlock * blocksPerKernel;

    // treat case when only one kernel is needed
    if (numberOfWalkers < walkersPerKernel)
    {
        blocksPerKernel = (uint)ceil((double)(numberOfWalkers) / (double)(threadsPerBlock));

        // blocks per kernel should be multiple of 2
        if (blocksPerKernel % 2 == 1)
        {
            blocksPerKernel += 1;
        }

        walkersPerKernel = threadsPerBlock * blocksPerKernel;
    }

    // Walker packs == groups of walkers in the same kernel
    // all threads in a pack represent a walker in the NMR simulation
    // But, in the last pack, some threads may be idle
    uint numberOfWalkerPacks = (numberOfWalkers / walkersPerKernel) + 1;
    uint lastWalkerPackSize = numberOfWalkers % walkersPerKernel;
    uint lastWalkerPackTail = walkersPerKernel - lastWalkerPackSize;
    uint energyArraySize = walkersPerKernel;
    uint energyCollectorSize = (blocksPerKernel / 2);

    // Copy bitBlock3D data from host to device (only once)
    // assign pointer to bitBlock datastructure
    uint64_t *bitBlock;
    bitBlock = this->model.getBitBlock()->getBlocks();
    uint64_t *d_bitBlock;
    cudaMalloc((void **)&d_bitBlock, numberOfBitBlocks * sizeof(uint64_t));
    cudaMemcpy(d_bitBlock, bitBlock, numberOfBitBlocks * sizeof(uint64_t), cudaMemcpyHostToDevice);

    // Host and Device memory data allocation
    // pointers used in host array conversion
    int *walker_px = MemAllocator::mallocIntArray(walkersPerKernel);
    int *walker_py = MemAllocator::mallocIntArray(walkersPerKernel);
    int *walker_pz = MemAllocator::mallocIntArray(walkersPerKernel);
    double *penalty = MemAllocator::mallocDoubleArray(walkersPerKernel);
    double *pAlive = MemAllocator::mallocDoubleArray(walkersPerKernel);
    double *phase = MemAllocator::mallocDoubleArray(walkersPerKernel);
    double *energy = MemAllocator::mallocDoubleArray(echoesPerKernel * energyArraySize);
    double *energyCollector = MemAllocator::mallocDoubleArray(echoesPerKernel * energyCollectorSize);
    uint64_t *seed = MemAllocator::mallocUInt64Array(walkersPerKernel);
    
    // temporary array to collect energy contributions for each echo in a kernel
    double *temp_globalEnergy = MemAllocator::mallocDoubleArray((uint)echoesPerKernel);
    double *h_globalEnergy = MemAllocator::mallocDoubleArray(kernelCalls * echoesPerKernel);

    tick = omp_get_wtime();
    for (uint echo = 0; echo < numberOfEchoes; echo++)
    {
        h_globalEnergy[echo] = 0.0;
    }
    buffer_time += omp_get_wtime() - tick;

    // Declaration of pointers to device data arrays
    int *d_walker_px;
    int *d_walker_py;
    int *d_walker_pz;
    double *d_penalty;
    double *d_pAlive;
    double *d_phase;
    double *d_field;
    double *d_energy;
    double *d_energyCollector;
    uint64_t *d_seed;

    // Memory allocation in device for data arrays
    cudaMalloc((void **)&d_walker_px, walkersPerKernel * sizeof(int));
    cudaMalloc((void **)&d_walker_py, walkersPerKernel * sizeof(int));
    cudaMalloc((void **)&d_walker_pz, walkersPerKernel * sizeof(int));
    cudaMalloc((void **)&d_penalty, walkersPerKernel * sizeof(double));
    if(applyField) cudaMalloc((void **)&d_pAlive, walkersPerKernel * sizeof(double));
    if(applyField) cudaMalloc((void **)&d_phase, walkersPerKernel * sizeof(double));
    cudaMalloc((void **)&d_energy, echoesPerKernel * energyArraySize * sizeof(double));
    cudaMalloc((void **)&d_energyCollector, echoesPerKernel * energyCollectorSize * sizeof(double));
    cudaMalloc((void **)&d_seed, walkersPerKernel * sizeof(uint64_t));
    
    if(applyField)
    {
        cudaMalloc((void **)&d_field, fieldSize * sizeof(double));
        cudaMemcpy(d_field, field, fieldSize * sizeof(double), cudaMemcpyHostToDevice);
    }

    tick = omp_get_wtime();
    for (uint i = 0; i < energyArraySize * echoesPerKernel; i++)
    {
        energy[i] = 0.0;
    }
    buffer_time += omp_get_wtime() - tick;

    for (uint packId = 0; packId < (numberOfWalkerPacks - 1); packId++)
    {
        // set offset in walkers vector
        uint packOffset = packId * walkersPerKernel;

        // Host data copy
        // copy original walkers' data to temporary host arrays
        tick = omp_get_wtime();
        if(this->model.getRwnmrConfig().getOpenMPUsage())
        {
            // set omp variables for parallel loop throughout walker list
            const int num_cpu_threads = omp_get_max_threads();
            const int loop_size = walkersPerKernel;
            int loop_start, loop_finish;

            #pragma omp parallel shared(packOffset, walker_px, walker_py, walker_pz, penalty, energy, seed) private(loop_start, loop_finish) 
            {
                const int thread_id = omp_get_thread_num();
                ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                loop_start = looper.getStart();
                loop_finish = looper.getFinish(); 

                for (uint i = loop_start; i < loop_finish; i++)
                {
                    walker_px[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionX();
                    walker_py[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionY();
                    walker_pz[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionZ();
                    penalty[i] = (*this->model.getWalkers())[i + packOffset].getDecreaseFactor();
                    pAlive[i] = 1.0;
                    phase[i] = 0.0;
                    energy[i + ((echoesPerKernel - 1) * energyArraySize)] = (*this->model.getWalkers())[i + packOffset].getEnergy();
                    seed[i] = (*this->model.getWalkers())[i + packOffset].getInitialSeed();
                }
            }
        } else
        {            
            for (uint i = 0; i < walkersPerKernel; i++)
            {
                walker_px[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionX();
                walker_py[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionY();
                walker_pz[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionZ();
                penalty[i] = (*this->model.getWalkers())[i + packOffset].getDecreaseFactor();
                pAlive[i] = 1.0;    
                phase[i] = 0.0;
                energy[i + ((echoesPerKernel - 1) * energyArraySize)] = (*this->model.getWalkers())[i + packOffset].getEnergy();
                seed[i] = (*this->model.getWalkers())[i + packOffset].getInitialSeed();
            }
        }  
        buffer_time += omp_get_wtime() - tick;      

        // Device data copy
        // copy host data to device
        tick = omp_get_wtime();
        cudaMemcpy(d_walker_px, walker_px, walkersPerKernel * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_walker_py, walker_py, walkersPerKernel * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_walker_pz, walker_pz, walkersPerKernel * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_penalty, penalty, walkersPerKernel * sizeof(double), cudaMemcpyHostToDevice);
        if(applyField) cudaMemcpy(d_pAlive, pAlive, walkersPerKernel * sizeof(double), cudaMemcpyHostToDevice);
        if(applyField) cudaMemcpy(d_phase, phase, walkersPerKernel * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_energy, energy, echoesPerKernel * energyArraySize * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_seed, seed, walkersPerKernel * sizeof(uint64_t), cudaMemcpyHostToDevice);
        copy_time += omp_get_wtime() - tick;

        // Launch kernel for GPU computation
        for (uint kernelId = 0; kernelId < kernelCalls; kernelId++)
        {
            // define echo offset
            uint echoOffset = kernelId * echoesPerKernel;
            uint echoes = echoesPerKernel;

            /* 
                Call adequate RW kernel depending on the chosen boundary treatment
            */
            tick = omp_get_wtime();
            if(applyField)
            {
                if(bcMirrorFlag and interpFlag)
                {
                    CPMG_walk_field_interp<true><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        walkersPerKernel, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
                else if(bcMirrorFlag and !interpFlag)
                {
                    CPMG_walk_field<true><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        walkersPerKernel, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
                else if(!bcMirrorFlag and interpFlag)
                {
                    CPMG_walk_field_interp<false><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        walkersPerKernel, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
                else 
                {
                    CPMG_walk_field<false><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        walkersPerKernel, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
            } 
            else
            {
                if(bcMirrorFlag)
                {
                    CPMG_walk<true><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px,
                        d_walker_py,
                        d_walker_pz,
                        d_penalty,
                        d_energy,
                        d_seed,
                        d_bitBlock,
                        bitBlockColumns,
                        bitBlockRows,
                        walkersPerKernel,
                        energyArraySize,
                        echoes,
                        stepsPerEcho,
                        imageColumns,
                        imageRows,
                        imageDepth,
                        shiftConverter
                    );
                }
                else
                {
                    CPMG_walk<false><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px,
                        d_walker_py,
                        d_walker_pz,
                        d_penalty,
                        d_energy,
                        d_seed,
                        d_bitBlock,
                        bitBlockColumns,
                        bitBlockRows,
                        walkersPerKernel,
                        energyArraySize,
                        echoes,
                        stepsPerEcho,
                        imageColumns,
                        imageRows,
                        imageDepth,
                        shiftConverter
                    );
                }  
            }

            cudaDeviceSynchronize();
            kernel_time += omp_get_wtime() - tick;

            
            // launch globalEnergy "reduce" kernel
            tick = omp_get_wtime();
            CPMG_energyReduce<<<blocksPerKernel / 2,
                                threadsPerBlock,
                                threadsPerBlock * sizeof(double)>>>(d_energy,
                                                                    d_energyCollector,
                                                                    energyArraySize,
                                                                    energyCollectorSize,
                                                                    echoesPerKernel);
            cudaDeviceSynchronize();
            reduce_time += omp_get_wtime() - tick;

            // copy data from gatherer array
            tick = omp_get_wtime();
            cudaMemcpy(energyCollector,
                       d_energyCollector,
                       echoesPerKernel * energyCollectorSize * sizeof(double),
                       cudaMemcpyDeviceToHost);
            copy_time += omp_get_wtime() - tick;

            //last reduce is done in CPU parallel-style using openMP
            tick = omp_get_wtime();
            CPMG_reduce_omp(temp_globalEnergy, energyCollector, echoesPerKernel, blocksPerKernel / 2);
            reduce_time += omp_get_wtime() - tick;

            // copy data from temporary array to NMR_Simulation2D "globalEnergy" vector class member
            tick = omp_get_wtime();
            for (uint echo = 0; echo < echoesPerKernel; echo++)
            {
                h_globalEnergy[echo + echoOffset] += temp_globalEnergy[echo];
            }
            buffer_time += omp_get_wtime() - tick;

            // recover last positions
            tick = omp_get_wtime();
            cudaMemcpy(walker_px, d_walker_px, walkersPerKernel * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_py, d_walker_py, walkersPerKernel * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_pz, d_walker_pz, walkersPerKernel * sizeof(int), cudaMemcpyDeviceToHost);      
            copy_time = omp_get_wtime() - tick;

            tick = omp_get_wtime();
            if(this->model.getRwnmrConfig().getOpenMPUsage())
            {
                // set omp variables for parallel loop throughout walker list
                const int num_cpu_threads = omp_get_max_threads();
                const int loop_size = walkersPerKernel;
                int loop_start, loop_finish;

                #pragma omp parallel shared(walker_px, walker_py, walker_pz, packOffset) private(loop_start, loop_finish) 
                {
                    const int thread_id = omp_get_thread_num();
                    ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                    loop_start = looper.getStart();
                    loop_finish = looper.getFinish(); 

                    for (uint id = loop_start; id < loop_finish; id++)
                    {
                        (*this->model.getWalkers())[id + packOffset].setCurrentPositionX(walker_px[id]);
                        (*this->model.getWalkers())[id + packOffset].setCurrentPositionY(walker_py[id]);
                        (*this->model.getWalkers())[id + packOffset].setCurrentPositionZ(walker_pz[id]);
                    }
                }
            } else
            {
                for (uint i = 0; i < walkersPerKernel; i++)
                {
                    (*this->model.getWalkers())[i + packOffset].setCurrentPositionX(walker_px[i]);
                    (*this->model.getWalkers())[i + packOffset].setCurrentPositionY(walker_py[i]);
                    (*this->model.getWalkers())[i + packOffset].setCurrentPositionZ(walker_pz[i]); 
                }
            }
            buffer_time += omp_get_wtime() - tick;  
        }
    }

    if (lastWalkerPackSize > 0)
    {
        // last Walker pack is done explicitly
        // set offset in walkers vector
        uint packOffset = (numberOfWalkerPacks - 1) * walkersPerKernel;

        // Host data copy
        // copy original walkers' data to temporary host arrays
        tick = omp_get_wtime();
        if(this->model.getRwnmrConfig().getOpenMPUsage())
        {
            // set omp variables for parallel loop throughout walker list
            const int num_cpu_threads = omp_get_max_threads();
            const int loop_size = lastWalkerPackSize;
            int loop_start, loop_finish;

            #pragma omp parallel shared(packOffset, walker_px, walker_py, walker_pz, penalty, energy, seed) private(loop_start, loop_finish) 
            {
                const int thread_id = omp_get_thread_num();
                ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                loop_start = looper.getStart();
                loop_finish = looper.getFinish(); 

                for (uint i = loop_start; i < loop_finish; i++)
                {
                    walker_px[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionX();
                    walker_py[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionY();
                    walker_pz[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionZ();
                    penalty[i] = (*this->model.getWalkers())[i + packOffset].getDecreaseFactor();
                    pAlive[i] = 1.0;
                    phase[i] = 0.0;
                    energy[i + ((echoesPerKernel - 1) * energyArraySize)] = (*this->model.getWalkers())[i + packOffset].getEnergy();
                    seed[i] = (*this->model.getWalkers())[i + packOffset].getInitialSeed();
                }
            }
        } else
        {            
            for (uint i = 0; i < lastWalkerPackSize; i++)
            {
                walker_px[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionX();
                walker_py[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionY();
                walker_pz[i] = (*this->model.getWalkers())[i + packOffset].getInitialPositionZ();
                penalty[i] = (*this->model.getWalkers())[i + packOffset].getDecreaseFactor();
                pAlive[i] = 1.0;
                phase[i] = 0.0;
                energy[i + ((echoesPerKernel - 1) * energyArraySize)] = (*this->model.getWalkers())[i + packOffset].getEnergy();
                seed[i] = (*this->model.getWalkers())[i + packOffset].getInitialSeed();
            }
        }   

        // complete energy array data
        for (uint echo = 0; echo < echoesPerKernel; echo++)
        {
            for (uint i = 0; i < lastWalkerPackTail; i++)
            {
                {
                    energy[i + lastWalkerPackSize + (echo * energyArraySize)] = 0.0;
                }
            }
        }
        buffer_time += omp_get_wtime() - tick;
        

        // Device data copy
        // copy host data to device
        tick = omp_get_wtime();
        cudaMemcpy(d_walker_px, walker_px, lastWalkerPackSize * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_walker_py, walker_py, lastWalkerPackSize * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_walker_pz, walker_pz, lastWalkerPackSize * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_penalty, penalty, lastWalkerPackSize * sizeof(double), cudaMemcpyHostToDevice);
        if(applyField) cudaMemcpy(d_pAlive, pAlive, lastWalkerPackSize * sizeof(double), cudaMemcpyHostToDevice);
        if(applyField) cudaMemcpy(d_phase, phase, lastWalkerPackSize * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_energy, energy, echoesPerKernel * energyArraySize * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_seed, seed, lastWalkerPackSize * sizeof(uint64_t), cudaMemcpyHostToDevice);
        copy_time += omp_get_wtime() - tick;
        

        // Launch kernel for GPU computation
        for (uint kernelId = 0; kernelId < kernelCalls; kernelId++)
        {
            // define echo offset
            uint echoOffset = kernelId * echoesPerKernel;
            uint echoes = echoesPerKernel;

            /* 
                Call adequate RW kernel depending on the chosen boundary treatment
            */
            tick = omp_get_wtime();
            if(applyField)
            {
                if(bcMirrorFlag and interpFlag)
                {
                    CPMG_walk_field_interp<true><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        lastWalkerPackSize, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
                else if(bcMirrorFlag and !interpFlag)
                {
                    CPMG_walk_field<true><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        lastWalkerPackSize, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
                else if(!bcMirrorFlag and interpFlag)
                {
                    CPMG_walk_field_interp<false><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        lastWalkerPackSize, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
                else 
                {
                    CPMG_walk_field<false><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px, 
                        d_walker_py, 
                        d_walker_pz, 
                        d_penalty, 
                        d_energy,
                        d_pAlive,
                        d_phase, 
                        d_seed, 
                        d_bitBlock, 
                        bitBlockColumns, 
                        bitBlockRows, 
                        lastWalkerPackSize, 
                        energyArraySize, 
                        echoes, 
                        stepsPerEcho, 
                        imageColumns, 
                        imageRows, 
                        imageDepth, 
                        shiftConverter,
                        gamma,
                        timeInterval,
                        d_field
                    );
                }
            } 
            else
            {
                if(bcMirrorFlag)
                {
                    CPMG_walk<true><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px,
                        d_walker_py,
                        d_walker_pz,
                        d_penalty,
                        d_energy,
                        d_seed,
                        d_bitBlock,
                        bitBlockColumns,
                        bitBlockRows,
                        lastWalkerPackSize,
                        energyArraySize,
                        echoes,
                        stepsPerEcho,
                        imageColumns,
                        imageRows,
                        imageDepth,
                        shiftConverter
                    );
                }
                else
                {
                    CPMG_walk<false><<<blocksPerKernel, threadsPerBlock>>>(
                        d_walker_px,
                        d_walker_py,
                        d_walker_pz,
                        d_penalty,
                        d_energy,
                        d_seed,
                        d_bitBlock,
                        bitBlockColumns,
                        bitBlockRows,
                        lastWalkerPackSize,
                        energyArraySize,
                        echoes,
                        stepsPerEcho,
                        imageColumns,
                        imageRows,
                        imageDepth,
                        shiftConverter
                    );
                }
            }
        
            cudaDeviceSynchronize();
            kernel_time += omp_get_wtime() - tick;

            // launch globalEnergy "reduce" kernel
            tick = omp_get_wtime();
            CPMG_energyReduce<<<blocksPerKernel / 2,
                                threadsPerBlock,
                                threadsPerBlock * sizeof(double)>>>(d_energy,
                                                                    d_energyCollector,
                                                                    energyArraySize,
                                                                    energyCollectorSize,
                                                                    echoesPerKernel);
            cudaDeviceSynchronize();
            reduce_time += omp_get_wtime() - tick;

            // copy data from gatherer array
            tick = omp_get_wtime();
            cudaMemcpy(energyCollector,
                       d_energyCollector,
                       echoesPerKernel * energyCollectorSize * sizeof(double),
                       cudaMemcpyDeviceToHost);
            copy_time += omp_get_wtime() - tick;

            //last reduce is done in CPU parallel-style using openMP
            tick = omp_get_wtime();
            CPMG_reduce_omp(temp_globalEnergy, energyCollector, echoesPerKernel, blocksPerKernel / 2);
            reduce_time += omp_get_wtime() - tick;

            // copy data from temporary array
            tick = omp_get_wtime();
            for (uint echo = 0; echo < echoesPerKernel; echo++)
            {
                h_globalEnergy[echo + echoOffset] += temp_globalEnergy[echo];
            }
            buffer_time += omp_get_wtime() - tick;

            // recover last positions
            tick = omp_get_wtime();
            cudaMemcpy(walker_px, d_walker_px, lastWalkerPackSize * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_py, d_walker_py, lastWalkerPackSize * sizeof(int), cudaMemcpyDeviceToHost);
            cudaMemcpy(walker_pz, d_walker_pz, lastWalkerPackSize * sizeof(int), cudaMemcpyDeviceToHost);      
            copy_time += omp_get_wtime() - tick;

            tick = omp_get_wtime();
            if(this->model.getRwnmrConfig().getOpenMPUsage())
            {
                // set omp variables for parallel loop throughout walker list
                const int num_cpu_threads = omp_get_max_threads();
                const int loop_size = lastWalkerPackSize;
                int loop_start, loop_finish;

                #pragma omp parallel shared(walker_px, walker_py, walker_pz, packOffset) private(loop_start, loop_finish) 
                {
                    const int thread_id = omp_get_thread_num();
                    ThreadsBalancer looper(thread_id, num_cpu_threads, loop_size);
                    loop_start = looper.getStart();
                    loop_finish = looper.getFinish(); 

                    for (uint id = loop_start; id < loop_finish; id++)
                    {
                        (*this->model.getWalkers())[id + packOffset].setCurrentPositionX(walker_px[id]);
                        (*this->model.getWalkers())[id + packOffset].setCurrentPositionY(walker_py[id]);
                        (*this->model.getWalkers())[id + packOffset].setCurrentPositionZ(walker_pz[id]);
                    }
                }
            } else
            {
                for (uint i = 0; i < lastWalkerPackSize; i++)
                {
                    (*this->model.getWalkers())[i + packOffset].setCurrentPositionX(walker_px[i]);
                    (*this->model.getWalkers())[i + packOffset].setCurrentPositionY(walker_py[i]);
                    (*this->model.getWalkers())[i + packOffset].setCurrentPositionZ(walker_pz[i]);            
                }
            }
            buffer_time += omp_get_wtime() - tick;  
        }
    }

    // insert to object energy values computed in gpu
    tick = omp_get_wtime();
    for (uint echo = 0; echo < numberOfEchoes; echo++)
    {
        this->signalAmps.push_back(h_globalEnergy[echo]);
    }
    buffer_time += omp_get_wtime() - tick;

    // free pointers in host
    free(walker_px);
    free(walker_py);
    free(walker_pz);
    free(penalty);
    free(pAlive);
    free(phase);
    free(h_globalEnergy);
    free(energy);
    free(energyCollector);
    free(temp_globalEnergy);
    free(seed);

    // and direct them to NULL
    walker_px = NULL;
    walker_py = NULL;
    walker_pz = NULL;
    penalty = NULL;
    pAlive = NULL;
    phase = NULL;
    h_globalEnergy = NULL;
    energy = NULL;
    energyCollector = NULL;
    temp_globalEnergy = NULL;
    seed = NULL;

    // also direct the bitBlock pointer created in this context
    // (original data is kept safe)
    bitBlock = NULL;
    field = NULL;

    // free device global memory
    cudaFree(d_walker_px);
    cudaFree(d_walker_py);
    cudaFree(d_walker_pz);
    cudaFree(d_penalty);
    if(applyField) cudaFree(d_pAlive);
    if(applyField) cudaFree(d_phase);
    if(applyField) cudaFree(d_field);
    cudaFree(d_energy);
    cudaFree(d_energyCollector);
    cudaFree(d_seed);
    cudaFree(d_bitBlock);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    cout << "Done.\nCpu/Gpu elapsed time: " << elapsedTime * 1.0e-3 << " s" << endl;
    cudaDeviceReset();

    if(time_verbose)
    {
        cout << "--- Time analysis ---" << endl;
        cout << "cpu data reset:    \t" << reset_time << " s" << endl;
        cout << "cpu data buffer:   \t" << buffer_time << " s" << endl;
        cout << "gpu data copy:     \t" << copy_time << " s" << endl;
        cout << "gpu kernel launch: \t" << kernel_time << " s" << endl;
        cout << "gpu reduce launch: \t" << reduce_time << " s" << endl;
        cout << "---------------------" << endl;
    }
}


/////////////////////////////////////////////////////////////////////
//////////////////////// HOST FUNCTIONS ///////////////////////////
/////////////////////////////////////////////////////////////////////
void CPMG_reduce_omp(double *temp_collector, double *array, int numberOfEchoes, uint arraySizePerEcho)
{
    // declaring shared variables
    uint offset;
    double arraySum;
    uint id;

    for (int echo = 0; echo < numberOfEchoes; echo++)
    {
        arraySum = 0.0;
        offset = (echo * arraySizePerEcho);

// #pragma omp parallel for reduction(+ \
//                                    : arraySum) private(id) shared(array, offset)
        for (id = 0; id < arraySizePerEcho; id++)
        {
            arraySum += array[id + offset];
        }

        temp_collector[echo] = arraySum;
    }
}

/////////////////////////////////////////////////////////////////////
//////////////////////// DEVICE FUNCTIONS ///////////////////////////
/////////////////////////////////////////////////////////////////////

__device__ direction computeNextDirection_CPMG(uint64_t &seed)
{
    // generate random number using xorshift algorithm
    xorshift64_state xor_state;
    xor_state.a = seed;
    seed = xorShift64_CPMG(&xor_state);
    uint64_t rand = seed;

    // set direction based on the random number
    direction nextDirection = (direction)(mod6_CPMG(rand) + 1);
    return nextDirection;
}

__device__ uint64_t xorShift64_CPMG(struct xorshift64_state *state)
{
    uint64_t x = state->a;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return state->a = x;
}

__device__ uint64_t mod6_CPMG(uint64_t a)
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

__device__ void computeNextPosition_CPMG(int &walker_px,
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

__device__ bool checkNextPosition_CPMG(int next_x,
                                     int next_y,
                                     int next_z,
                                     const uint64_t *bitBlock,
                                     const int bitBlockColumns,
                                     const int bitBlockRows)
{
    int blockIndex = findBlockIndex_CPMG(next_x, next_y, next_z, bitBlockColumns, bitBlockRows);
    int nextBit = findBitIndex_CPMG(next_x, next_y, next_z);
    uint64_t nextBlock = bitBlock[blockIndex];

    return (!checkIfBlockBitIsWall_CPMG(nextBlock, nextBit));
};

__device__ int findBlockIndex_CPMG(int next_x, int next_y, int next_z, int bitBlockColumns, int bitBlockRows)
{
    // "x >> 2" is like "x / 4" in bitwise operation
    int block_x = next_x >> 2;
    int block_y = next_y >> 2;
    int block_z = next_z >> 2;
    int blockIndex = block_x + block_y * bitBlockColumns + block_z * (bitBlockColumns * bitBlockRows);

    return blockIndex;
}

__device__ int findBitIndex_CPMG(int next_x, int next_y, int next_z)
{
    // "x & (n - 1)" is lise "x % n" in bitwise operation
    int bit_x = next_x & (COLUMNSPERBLOCK3D - 1);
    int bit_y = next_y & (ROWSPERBLOCK3D - 1);
    int bit_z = next_z & (DEPTHPERBLOCK3D - 1);
    // "x << 3" is like "x * 8" in bitwise operation
    int bitIndex = bit_x + (bit_y << 2) + ((bit_z << 2) << 2);

    return bitIndex;
}

__device__ bool checkIfBlockBitIsWall_CPMG(uint64_t nextBlock, int nextBit)
{
    return ((nextBlock >> nextBit) & 1ull);
}

__device__ int convertLocalToGlobal_CPMG(int _localPos, uint _shiftConverter)
{
    return (_localPos >> _shiftConverter);
}

__device__ int bcMapPeriodic_CPMG_old(int _localPos, uint _shiftConverter, int _dimSize)
{
    int globalPos;

    globalPos = convertLocalToGlobal_CPMG(_localPos, _shiftConverter);
    globalPos = globalPos % _dimSize;
    if(globalPos < 0) return globalPos + _dimSize;
    else return globalPos;
}

__device__ int bcMapPeriodic_CPMG(int _localPos, uint _shiftConverter, int _dimSize)
{
    int globalPos;
    globalPos = ( convertLocalToGlobal_CPMG(_localPos, _shiftConverter) ) % _dimSize;
    globalPos += (((int) (globalPos < 0)) * _dimSize);
    return globalPos;
}

__device__ int bcMapMirror_CPMG_old(int _localPos, uint _shiftConverter, int _dimSize)
{
    int globalPos;
    int imgPos;
    int mirror;
    int antimirror;

    globalPos = convertLocalToGlobal_CPMG(_localPos, _shiftConverter);
    imgPos = globalPos % _dimSize;    
    if(imgPos < 0) imgPos += _dimSize;
    if(globalPos > 0) mirror = (globalPos / _dimSize) % 2;
    else mirror = ((-globalPos - 1 + _dimSize) / _dimSize) % 2; 
    antimirror = (mirror + 1) % 2;
    return (antimirror * imgPos) + (mirror * (_dimSize - 1 - imgPos));
}

__device__ int bcMapMirror_CPMG(int _localPos, uint _shiftConverter, int _dimSize)
{
    int globalPos = convertLocalToGlobal_CPMG(_localPos, _shiftConverter);
    int imgPos = (globalPos % _dimSize); 
    imgPos += (((int) (imgPos < 0)) * _dimSize);
    int mirror = ((isPositive_cpmg(globalPos)*globalPos) + (((int) !(globalPos > 0)) * (-globalPos-1+_dimSize)))/_dimSize; 
    mirror = (mirror & 1);
    return (((mirror + 1) & 1) * imgPos) + (mirror * (_dimSize - 1 - imgPos));
}

__device__ int isPositive_cpmg(int x) 
{
   return !((x&(1<<31)) | !x);
}

__device__ int isZero_cpmg(int x) 
{
   return !x;
}

__device__ long getFieldIndex(int _x, int _y, int _z, int _rowScale, int _depthScale)
{ 
    return (_x + (_y * _rowScale) + (_z * _depthScale)); 
}

__device__ void getNeighbors_periodic(Neighborhood &n, int x, int y, int z, int stride, uint shiftConverter, int cols, int rows, int depth)
{
    int x0 = (x >> shiftConverter) - (int) ( ((x & (stride-1)) < (stride>>1)) ); // se x for menor do que x0 tem que diminuir 1
    int x1 = (x >> shiftConverter) + (int) (!((x & (stride-1)) < (stride>>1)) ); // se x for maior do que x0 tem que aumentar 1
    int y0 = (y >> shiftConverter) - (int) ( ((y & (stride-1)) < (stride>>1)) ); 
    int y1 = (y >> shiftConverter) + (int) (!((y & (stride-1)) < (stride>>1)) );
    int z0 = (z >> shiftConverter) - (int) ( ((z & (stride-1)) < (stride>>1)) ); 
    int z1 = (z >> shiftConverter) + (int) (!((z & (stride-1)) < (stride>>1)) );

    n.p000.x = bcMapPeriodic_CPMG(x0,0,cols); 
    n.p000.y = bcMapPeriodic_CPMG(y0,0,rows); 
    n.p000.z = bcMapPeriodic_CPMG(z0,0,depth); 
    
    n.p100.x = bcMapPeriodic_CPMG(x1,0,cols); 
    n.p100.y = n.p000.y;
    n.p100.z = n.p000.z; 
        
    n.p010.x = n.p000.x;
    n.p010.y = bcMapPeriodic_CPMG(y1,0,rows); 
    n.p010.z = n.p000.z; 
        
    n.p110.x = n.p100.x; 
    n.p110.y = n.p010.y; 
    n.p110.z = n.p000.z; 
        
    n.p001.x = n.p000.x; 
    n.p001.y = n.p000.y; 
    n.p001.z = bcMapPeriodic_CPMG(z1,0,depth);     
    
    n.p101.x = n.p100.x; 
    n.p101.y = n.p000.y; 
    n.p101.z = n.p001.z;    
    
    n.p011.x = n.p000.x; 
    n.p011.y = n.p010.y; 
    n.p011.z = n.p001.z;     
    
    n.p111.x = n.p100.x; 
    n.p111.y = n.p010.y; 
    n.p111.z = n.p001.z; 
}

__device__ void getNeighbors_mirror(Neighborhood &n, int x, int y, int z, int stride, uint shiftConverter, int cols, int rows, int depth)
{   
    int x0 = (x >> shiftConverter) - (int) ( ((x & (stride-1)) < (stride>>1)) ); // if x < x0, x0 = xc - 1
    int x1 = (x >> shiftConverter) + (int) (!((x & (stride-1)) < (stride>>1)) ); // if x > x0, x1 = xc + 1 
    int y0 = (y >> shiftConverter) - (int) ( ((y & (stride-1)) < (stride>>1)) ); // if y < y0, y0 = yc - 1
    int y1 = (y >> shiftConverter) + (int) (!((y & (stride-1)) < (stride>>1)) ); // if y > y0, y1 = yc + 1 
    int z0 = (z >> shiftConverter) - (int) ( ((z & (stride-1)) < (stride>>1)) ); // if z < z0, z0 = zc - 1
    int z1 = (z >> shiftConverter) + (int) (!((z & (stride-1)) < (stride>>1)) ); // if z > z0, z1 = zc + 1 
    
    n.p000.x = bcMapMirror_CPMG(x0,0,cols); 
    n.p000.y = bcMapMirror_CPMG(y0,0,rows); 
    n.p000.z = bcMapMirror_CPMG(z0,0,depth); 
    
    n.p100.x = bcMapMirror_CPMG(x1,0,cols); 
    n.p100.y = n.p000.y;
    n.p100.z = n.p000.z; 
        
    n.p010.x = n.p000.x;
    n.p010.y = bcMapMirror_CPMG(y1,0,rows); 
    n.p010.z = n.p000.z; 
        
    n.p110.x = n.p100.x; 
    n.p110.y = n.p010.y; 
    n.p110.z = n.p000.z; 
        
    n.p001.x = n.p000.x; 
    n.p001.y = n.p000.y; 
    n.p001.z = bcMapMirror_CPMG(z1,0,depth);     
    
    n.p101.x = n.p100.x; 
    n.p101.y = n.p000.y; 
    n.p101.z = n.p001.z;    
    
    n.p011.x = n.p000.x; 
    n.p011.y = n.p010.y; 
    n.p011.z = n.p001.z;     
    
    n.p111.x = n.p100.x; 
    n.p111.y = n.p010.y; 
    n.p111.z = n.p001.z; 
}

__device__ void getInterpCube(InterpCube &ic, Neighborhood &n, const double *field, int rowScale, int depthScale)
{    
    ic.i000 = getFieldIndex(n.p000.x,n.p000.y,n.p000.z,rowScale,depthScale);
    ic.c000 = field[ic.i000];

    ic.i100 = getFieldIndex(n.p100.x,n.p100.y,n.p100.z,rowScale,depthScale);
    ic.c100 = field[ic.i100];

    ic.i010 = getFieldIndex(n.p010.x,n.p010.y,n.p010.z,rowScale,depthScale);
    ic.c010 = field[ic.i010];

    ic.i110 = getFieldIndex(n.p110.x,n.p110.y,n.p110.z,rowScale,depthScale);
    ic.c110 = field[ic.i110];

    ic.i001 = getFieldIndex(n.p001.x,n.p001.y,n.p001.z,rowScale,depthScale);
    ic.c001 = field[ic.i001];

    ic.i101 = getFieldIndex(n.p101.x,n.p101.y,n.p101.z,rowScale,depthScale);
    ic.c101 = field[ic.i101];

    ic.i011 = getFieldIndex(n.p011.x,n.p011.y,n.p011.z,rowScale,depthScale);
    ic.c011 = field[ic.i011];

    ic.i111 = getFieldIndex(n.p111.x,n.p111.y,n.p111.z,rowScale,depthScale);
    ic.c111 = field[ic.i111];
}

__device__ double interpValues(InterpCube &ic, int x, int y, int z, int stride)
{
    // compensation for sc=0 cases:
    double comp = (double) (stride != 1);
    double xd = comp*(((double) ((x-(stride/2)) & (stride-1))) + 0.5)/stride; 
    double c00 = ic.c000*(1.0 - xd) + ic.c100*xd;
    double c01 = ic.c001*(1.0 - xd) + ic.c101*xd;
    double c10 = ic.c010*(1.0 - xd) + ic.c110*xd;
    double c11 = ic.c011*(1.0 - xd) + ic.c111*xd;
    
    double yd = comp*(((double) ((y-(stride/2)) & (stride-1))) + 0.5)/stride;
    double c0 = c00*(1.0-yd) + c10*yd;
    double c1 = c01*(1.0-yd) + c11*yd;
    
    double zd = comp*(((double) ((z-(stride/2)) & (stride-1))) + 0.5)/stride;
    double val = c0*(1.0-zd) + c1*zd;
    return val;
}

__device__ double triLinInterp_periodic(const double *field, int x, int y, int z, int stride, int shiftConverter, int cols, int rows, int depth, Neighborhood &n, InterpCube &ic)
{
    getNeighbors_periodic(n, x, y, z, stride, shiftConverter, cols, rows, depth);
    getInterpCube(ic, n, field, cols, cols*rows);
    return interpValues(ic, x, y, z, stride);    
}

__device__ double triLinInterp_mirror(const double *field, int x, int y, int z, int stride, int shiftConverter, int cols, int rows, int depth, Neighborhood &n, InterpCube &ic)
{
    getNeighbors_mirror(n, x, y, z, stride, shiftConverter, cols, rows, depth);
    getInterpCube(ic, n, field, cols, cols*rows);
    return interpValues(ic, x, y, z, stride);    
}

/*
    wrappers for kernel testing
*/

void NMR_cpmg::trilinearInterpolation()
{
    // CUDA event recorder to measure computation time in device
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    // integer values for sizing issues
    uint numberOfWalkers = this->model.getNumberOfWalkers();
    uint bitBlockColumns = this->model.getBitBlock()->getBlockColumns();
    uint bitBlockRows = this->model.getBitBlock()->getBlockRows();
    uint numberOfBitBlocks = this->model.getBitBlock()->getNumberOfBlocks();
    int imageColumns = this->model.getBitBlock()->getImageColumns();
    int imageRows = this->model.getBitBlock()->getImageRows();
    int imageDepth = this->model.getBitBlock()->getImageDepth();
    uint shiftConverter = log2(this->model.getVoxelDivision());
    
    // THIS NEEDS TO BE REVISED LATER!!!
    double *field = (*this).getInternalFieldData();
    long fieldSize = (*this).getInternalFieldSize();
    
    // Copy bitBlock3D data from host to device (only once)
    // assign pointer to bitBlock datastructure
    uint64_t *bitBlock;
    bitBlock = this->model.getBitBlock()->getBlocks();
    uint64_t *d_bitBlock;
    
    // Host and Device memory data allocation
    // pointers used in host array conversion
    int *walker_px = MemAllocator::mallocIntArray(numberOfWalkers);
    int *walker_py = MemAllocator::mallocIntArray(numberOfWalkers);
    int *walker_pz = MemAllocator::mallocIntArray(numberOfWalkers);
    
    for (uint i = 0; i < numberOfWalkers; i++)
    {
        walker_px[i] = (*this->model.getWalkers())[i].getInitialPositionX();
        walker_py[i] = (*this->model.getWalkers())[i].getInitialPositionY();
        walker_pz[i] = (*this->model.getWalkers())[i].getInitialPositionZ();
    }

    // Declaration of pointers to device data arrays
    int *d_walker_px;
    int *d_walker_py;
    int *d_walker_pz;
    double *d_field;

    // Memory allocation in device for data arrays
    cudaMalloc((void **)&d_bitBlock, numberOfBitBlocks * sizeof(uint64_t));
    cudaMalloc((void **)&d_walker_px, numberOfWalkers * sizeof(int));
    cudaMalloc((void **)&d_walker_py, numberOfWalkers * sizeof(int));
    cudaMalloc((void **)&d_walker_pz, numberOfWalkers * sizeof(int));
    cudaMalloc((void **)&d_field, fieldSize * sizeof(double));
    
    // Copy data from host to device
    cudaMemcpy(d_bitBlock, bitBlock, numberOfBitBlocks * sizeof(uint64_t), cudaMemcpyHostToDevice);
    cudaMemcpy(d_walker_px, walker_px, numberOfWalkers * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_walker_py, walker_py, numberOfWalkers * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_walker_pz, walker_pz, numberOfWalkers * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_field, field, fieldSize * sizeof(double), cudaMemcpyHostToDevice);

    /*
        Kernel calls
    */
    uint threadsPerBlock = this->model.getRwnmrConfig().getThreadsPerBlock();
    uint blocksPerKernel = this->model.getRwnmrConfig().getBlocks();
    triLinInterp<<<1, 1>>>(d_walker_px, 
                           d_walker_py, 
                           d_walker_pz, 
                           d_field, 
                           d_bitBlock, 
                           bitBlockColumns, 
                           bitBlockRows, 
                           numberOfWalkers, 
                           imageColumns, 
                           imageRows, 
                           imageDepth, 
                           shiftConverter);

    cudaDeviceSynchronize();

    /*
        End of routine
    */
    // free pointers in host
    free(walker_px);
    free(walker_py);
    free(walker_pz);
    
    // and direct them to NULL
    walker_px = NULL;
    walker_py = NULL;
    walker_pz = NULL;
    
    // also direct the bitBlock pointer created in this context
    // (original data is kept safe)
    bitBlock = NULL;
    field = NULL;

    // free device global memory
    cudaFree(d_bitBlock);
    cudaFree(d_walker_px);
    cudaFree(d_walker_py);
    cudaFree(d_walker_pz);
    cudaFree(d_field);
    
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    cout << "Done.\nCpu/Gpu elapsed time: " << elapsedTime * 1.0e-3 << " s" << endl;
    cudaDeviceReset();
}

void NMR_cpmg::bcMaps(int shiftConverter, int dimSize)
{
    /*
        Kernel calls
    */
    if(this->model.getRwnmrConfig().getBC() == "mirror")
        bcMap_test<true><<<1, 1>>>(shiftConverter, dimSize);
    else
        bcMap_test<false><<<1, 1>>>(shiftConverter, dimSize);
    
    cudaDeviceSynchronize();   
    cudaDeviceReset();
}

void NMR_cpmg::bcMapsExectime(int shiftConverter, int dimSize)
{    
    int bpk = 1024;
    int tpb = 1024;
    int size = tpb*bpk;
    bool printCondition = false;

    int *h_pos = MemAllocator::mallocIntArray(size);
    int *h_pos1 = MemAllocator::mallocIntArray(size);
    int *h_pos2 = MemAllocator::mallocIntArray(size);
    for(int i = 0; i < size; i++) h_pos[i] = i - (size/2);
    for(int i = 0; i < size; i++) h_pos1[i] = i - (size/2);
    for(int i = 0; i < size; i++) h_pos2[i] = i - (size/2);

    int *d_pos1;
    int *d_pos2;
    cudaMalloc((void **)&d_pos1, size * sizeof(int));
    cudaMemcpy(d_pos1, h_pos1, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&d_pos2, size * sizeof(int));
    cudaMemcpy(d_pos2, h_pos2, size * sizeof(int), cudaMemcpyHostToDevice);
    /*
        Kernel calls old implementation
    */
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    if(this->model.getRwnmrConfig().getBC() == "mirror")
        bcMap_testExecTime<true,false><<<bpk,tpb>>>(d_pos1, size, shiftConverter, dimSize);
    else
        bcMap_testExecTime<false,false><<<bpk,tpb>>>(d_pos1, size, shiftConverter, dimSize);    
    cudaDeviceSynchronize();
    cudaMemcpy(h_pos1, d_pos1, size * sizeof(int), cudaMemcpyDeviceToHost);
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    cout << "Old implementation: gpu elapsed time: " << elapsedTime * 1.0e-3 << " s" << endl;   
    
    /*
        Kernel calls new implementation
    */
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    if(this->model.getRwnmrConfig().getBC() == "mirror")
        bcMap_testExecTime<true,true><<<bpk,tpb>>>(d_pos2, size, shiftConverter, dimSize);
    else
        bcMap_testExecTime<false,true><<<bpk,tpb>>>(d_pos2, size, shiftConverter, dimSize);
    cudaDeviceSynchronize();
    cudaMemcpy(h_pos2, d_pos2, size * sizeof(int), cudaMemcpyDeviceToHost);
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);    
    cudaEventElapsedTime(&elapsedTime, start, stop);
    cout << "New implementation: gpu elapsed time: " << elapsedTime * 1.0e-3 << " s" << endl;   
    
    for(int i = 0; i < size; i++)
        if(printCondition) cout << "pos: " << h_pos[i] << "\told: " << h_pos1[i] << "\tnew: " << h_pos2[i] << endl;

    free(h_pos); h_pos=NULL;
    free(h_pos1); h_pos1=NULL;
    free(h_pos2); h_pos2=NULL;
    cudaFree(d_pos1);
    cudaFree(d_pos2);
    cudaDeviceReset();
}
