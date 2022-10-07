// #include "ConfigFilesTest.h"

// void ConfigFilesTest::run()
// {
// 	(*this).TestRWNMRConfigFile();
// 	(*this).TestUCTConfigFile();
// 	(*this).TestPFGSEConfigFile();
// 	(*this).TestMULTITAUConfigFile();
// 	return;
// }

// void ConfigFilesTest::TestRWNMRConfigFile()
// {
// 	cout << "executing rwnmr_config test...";
// 	string rwnmr_config_path = "trwnmr.config";
// 	rwnmr_config config;
// 	config.readConfigFile((*this).getInputDir() + rwnmr_config_path);
// 	Assert::assertEquals(config.getName(), (string)"NMR_TestSimulation", "rwnmr: Name");
// 	Assert::assertEquals(config.getD0(), (double) 2.5, "rwnmr: D0 is incorrect");
// 	Assert::assertEquals(config.getGiromagneticRatio(), (double) 42.576, "rwnmr: GiromagneticRatio");
// 	Assert::assertEquals(config.getGiromagneticUnit(), (string)"mhertz", "rwnmr: GiromagneticUnit");
// 	Assert::assertEquals(config.getBulkTime(), (double) 2800.0, "rwnmr: BulkTime");
// 	Assert::assertEquals(config.getRhoType(), (string)"uniform", "rwnmr: RhoType");
// 	Assert::assertEquals(config.getRho(), (vector<double>) {0.0}, "rwnmr: Rho");
// 	Assert::assertEquals(config.getWalkers(), (uint) 1000, "rwnmr: Walkers");
// 	Assert::assertEquals(config.getWalkerSamples(), (uint) 1, "rwnmr: WalkerSamples");
// 	Assert::assertEquals(config.getWalkersPlacement(), (string)"random", "rwnmr: WalkersPlacement");
// 	Assert::assertEquals(config.getPlacementDeviation(), (uint) 1, "rwnmr: PlacementDeviation");
// 	Assert::assertEquals(config.getStepsPerEcho(), (uint) 4, "rwnmr: StepsPerEcho");
// 	Assert::assertEquals(config.getSeed(), (uint64_t) 0, "rwnmr: Seed");
// 	Assert::assertEquals(config.getBC(), (string)"mirror", "rwnmr: BC");
// 	Assert::assertEquals(config.getSaveImgInfo(), (bool) false, "rwnmr: SaveImgInfo");
// 	Assert::assertEquals(config.getSaveBinImg(), (bool) false, "rwnmr: SaveBinImg");
// 	Assert::assertEquals(config.getSaveWalkers(), (bool) false, "rwnmr: SaveWalkers");
// 	Assert::assertEquals(config.getHistograms(), (uint) 1, "rwnmr: Histograms");
// 	Assert::assertEquals(config.getHistogramSize(), (uint) 256, "rwnmr: HistSize");
// 	Assert::assertEquals(config.getHistogramScale(), (string)"linear", "rwnmr: HistScale");
// 	Assert::assertEquals(config.getOpenMPUsage(), (bool) true, "rwnmr: OpenMPUsage");
// 	Assert::assertEquals(config.getOpenMPThreads(), (uint) 4, "rwnmr: OpenMPThreads");
// 	Assert::assertEquals(config.getGPUUsage(), (bool) true, "rwnmr: GPUUsage");
// 	Assert::assertEquals(config.getBlocks(), (uint) 4096, "rwnmr: Blocks");
// 	Assert::assertEquals(config.getThreadsPerBlock(), (uint) 1024, "rwnmr: ThreadsPerBlock");
// 	Assert::assertEquals(config.getReduceInGPU(), (bool) true, "rwnmr: ReduceInGPU");
// 	Assert::assertEquals(config.getMaxRWSteps(), (uint) 65536, "rwnmr: MaxRWSteps");
// 	Assert::assertEquals(config.getBitBlockBatchesSize(), (uint) 100000, "rwnmr: BITBLOCKS_BATCHES_SIZE");
// 	Assert::assertEquals(config.getBitBlockPropertiesSize(), (uint) 7, "rwnmr: BITBLOCK_PROP_SIZE");
// 	Assert::assertEquals(config.getNMRT2Size(), (uint) 128, "rwnmr: NMR_T2_SIZE");
// 	Assert::assertEquals(config.getStartTag(), (uint) 1000, "rwnmr: NMR_START_TAG");
// 	Assert::assertEquals(config.getBitBlockTag(), (uint) 2000, "rwnmr: NMR_BITBLOCK_TAG");
// 	Assert::assertEquals(config.getBatchTag(), (uint) 3000, "rwnmr: NMR_BATCH_TAG");
// 	Assert::assertEquals(config.getT2Tag(), (uint) 4000, "rwnmr: NMR_T2_TAG");
// 	Assert::assertEquals(config.getEndTag(), (uint) 5000, "rwnmr: NMR_END_TAG");
	
// 	cout << "Ok." << endl;
// 	return;
// }

// void ConfigFilesTest::TestUCTConfigFile()
// {
// 	cout << "executing uct_config test...";

// 	string config_path = "tuct.config";
// 	uct_config config;
// 	config.readConfigFile((*this).getInputDir() + config_path);
// 	Assert::assertEquals(config.getDirPath(), (string)"./input/images/sphere_2.5voxels/", "uct: DirPath");
// 	Assert::assertEquals(config.getFilename(), (string)"sphere_r=2.5voxels_", "uct: Filename");
// 	Assert::assertEquals(config.getFirstIdx(), (uint) 0, "uct: FirstIdx");
// 	Assert::assertEquals(config.getDigits(), (uint) 1, "uct: Digits");
// 	Assert::assertEquals(config.getExtension(), (string)".png", "uct: Extension");
// 	Assert::assertEquals(config.getSlices(), (uint) 7, "uct: Slices");
// 	Assert::assertEquals(config.getResolution(), (double) 1.0, "uct: Resolution");
// 	Assert::assertEquals(config.getVoxelDivision(), (uint) 0, "uct: VoxelDivision");
	
// 	cout << "Ok." << endl;
// 	return;
// }

// void ConfigFilesTest::TestPFGSEConfigFile()
// {
// 	cout << "executing pfgse_config test...";

// 	string config_path = "tpfgse.config";
// 	pfgse_config config;
// 	config.readConfigFile((*this).getInputDir() + config_path);
// 	Assert::assertEquals(config.getPulseWidth(), (double) 0.1, "pfgse: PulseWidth");
// 	Assert::assertEquals((vector<double>) {
// 					config.getMaxGradient().getX(), 
// 					config.getMaxGradient().getY(), 
// 					config.getMaxGradient().getZ()}, 
// 					(vector<double>) {0.0, 0.0, 100.0}, 
// 					"pfgse: MaxGradient");
// 	Assert::assertEquals(config.getGradientSamples(), (uint) 256, "pfgse: GradientSamples");
// 	Assert::assertEquals(config.getApplyBulk(), (bool) false, "pfgse: ApplyBulk");
// 	Assert::assertEquals(config.getAllowWalkerSampling(), (bool) true, "pfgse: AllowWalkerSampling");
// 	Assert::assertEquals(config.getApplyAbsorption(), (bool) false, "pfgse: ApplyAbsorption");
// 	Assert::assertEquals(config.getTimeSequence(), (string)"manual", "pfgse: TimeSequence");
// 	Assert::assertEquals(config.getTimeSamples(), (uint) 4, "pfgse: TimeSamples");
// 	Assert::assertEquals(config.getTimeValues(), (vector<double>) {0.01,0.1,1.0,10.0}, "pfgse: TimeValues");
// 	Assert::assertEquals(config.getTimeMin(), (double) 10.0, "pfgse: TimeMin");
// 	Assert::assertEquals(config.getTimeMax(), (double) 800.0, "pfgse: TimeMax");
// 	Assert::assertEquals(config.getInspectionLength(), (double) 10.0, "pfgse: InspectionLength");
// 	Assert::assertEquals(config.getNoiseAmp(), (double) 0.0001, "pfgse: NoiseAmp");
// 	Assert::assertEquals(config.getTargetSNR(), (double) -1.0, "pfgse: TargetSNR");
// 	Assert::assertEquals(config.getThresholdType(), (string)"lhs", "pfgse: ThresholdType");
// 	Assert::assertEquals(config.getThresholdValue(), (double) 0.9, "pfgse: ThresholdValue");
// 	Assert::assertEquals(config.getThresholdWindow(), (uint) 5, "pfgse: ThresholdWindow");
// 	Assert::assertEquals(config.getSaveMode(), (bool) true, "pfgse: SaveMode");
// 	Assert::assertEquals(config.getSavePFGSE(), (bool) false, "pfgse: SavePFGSE");
// 	Assert::assertEquals(config.getSaveWalkers(), (bool) false, "pfgse: SaveWalkers");
// 	Assert::assertEquals(config.getSaveHistogram(), (bool) false, "pfgse: SaveHistogram");
// 	Assert::assertEquals(config.getSaveHistogramList(), (bool) false, "pfgse: SaveHistogramList");	

// 	cout << "Ok." << endl;
// 	return;

// }

// void ConfigFilesTest::TestMULTITAUConfigFile()
// {
// 	cout << "executing multitau_config test...";

// 	string config_path = "tmultitau.config";
// 	multitau_config config;
// 	config.readConfigFile((*this).getInputDir() + config_path);
// 	Assert::assertEquals(config.getTauScale(), (string)"manual", "multitau: TauScale");
// 	Assert::assertEquals(config.getTauValues(), (vector<double>) {0.1,0.2,1.0,2.0,4.0}, "multitau: TauValues");
// 	Assert::assertEquals(config.getTauMin(), (double) 0.01, "multitau: TauMin");
// 	Assert::assertEquals(config.getTauMax(), (double) 10.0, "multitau: TauMax");
// 	Assert::assertEquals(config.getTauPoints(), (int) 5, "multitau: TauPoints");
// 	Assert::assertEquals(config.getSaveMode(), (bool) true, "multitau: SaveMode");
// 	Assert::assertEquals(config.getSaveDecay(), (bool) false, "multitau: SaveDecay");
// 	Assert::assertEquals(config.getSaveWalkers(), (bool) false, "multitau: SaveWalkers");
// 	Assert::assertEquals(config.getSaveHistogram(), (bool) false, "multitau: SaveHistogram");
// 	Assert::assertEquals(config.getSaveHistogramList(), (bool) false, "multitau: SaveHistogramList");	

// 	cout << "Ok." << endl;
// 	return;
// }