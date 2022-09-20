#include "ConfigFilesTest.h"

void ConfigFilesTest::run()
{
	(*this).TestRWNMRConfigFile();
	(*this).TestUCTConfigFile();
	(*this).TestCPMGConfigFile();
	(*this).TestPFGSEConfigFile();
	(*this).TestMULTITAUConfigFile();
	return;
}

void ConfigFilesTest::TestRWNMRConfigFile()
{
	cout << "executing rwnmr_config test...";
	string rwnmr_config_path = "trwnmr.config";
	rwnmr_config config;
	config.readConfigFile((*this).getInputDir() + rwnmr_config_path);
	Assert::check(config.getName(), (string)"NMR_TestSimulation", "rwnmr: Name");
	Assert::check(config.getD0(), (double) 2.5, "rwnmr: D0 is incorrect");
	Assert::check(config.getGiromagneticRatio(), (double) 42.576, "rwnmr: GiromagneticRatio");
	Assert::check(config.getGiromagneticUnit(), (string)"mhertz", "rwnmr: GiromagneticUnit");
	Assert::check(config.getBulkTime(), (double) 2800.0, "rwnmr: BulkTime");
	Assert::check(config.getRhoType(), (string)"uniform", "rwnmr: RhoType");
	Assert::check(config.getRho(), (vector<double>) {0.0}, "rwnmr: Rho");
	Assert::check(config.getWalkers(), (uint) 1000, "rwnmr: Walkers");
	Assert::check(config.getWalkerSamples(), (uint) 1, "rwnmr: WalkerSamples");
	Assert::check(config.getWalkersPlacement(), (string)"random", "rwnmr: WalkersPlacement");
	Assert::check(config.getPlacementDeviation(), (uint) 1, "rwnmr: PlacementDeviation");
	Assert::check(config.getStepsPerEcho(), (uint) 4, "rwnmr: StepsPerEcho");
	Assert::check(config.getSeed(), (uint64_t) 0, "rwnmr: Seed");
	Assert::check(config.getBC(), (string)"mirror", "rwnmr: BC");
	Assert::check(config.getSaveImgInfo(), (bool) false, "rwnmr: SaveImgInfo");
	Assert::check(config.getSaveBinImg(), (bool) false, "rwnmr: SaveBinImg");
	Assert::check(config.getSaveWalkers(), (bool) false, "rwnmr: SaveWalkers");
	Assert::check(config.getHistograms(), (uint) 1, "rwnmr: Histograms");
	Assert::check(config.getHistogramSize(), (uint) 256, "rwnmr: HistSize");
	Assert::check(config.getHistogramScale(), (string)"linear", "rwnmr: HistScale");
	Assert::check(config.getOpenMPUsage(), (bool) true, "rwnmr: OpenMPUsage");
	Assert::check(config.getOpenMPThreads(), (uint) 4, "rwnmr: OpenMPThreads");
	Assert::check(config.getGPUUsage(), (bool) true, "rwnmr: GPUUsage");
	Assert::check(config.getBlocks(), (uint) 4096, "rwnmr: Blocks");
	Assert::check(config.getThreadsPerBlock(), (uint) 1024, "rwnmr: ThreadsPerBlock");
	Assert::check(config.getReduceInGPU(), (bool) true, "rwnmr: ReduceInGPU");
	Assert::check(config.getMaxRWSteps(), (uint) 65536, "rwnmr: MaxRWSteps");
	Assert::check(config.getBitBlockBatchesSize(), (uint) 100000, "rwnmr: BITBLOCKS_BATCHES_SIZE");
	Assert::check(config.getBitBlockPropertiesSize(), (uint) 7, "rwnmr: BITBLOCK_PROP_SIZE");
	Assert::check(config.getNMRT2Size(), (uint) 128, "rwnmr: NMR_T2_SIZE");
	Assert::check(config.getStartTag(), (uint) 1000, "rwnmr: NMR_START_TAG");
	Assert::check(config.getBitBlockTag(), (uint) 2000, "rwnmr: NMR_BITBLOCK_TAG");
	Assert::check(config.getBatchTag(), (uint) 3000, "rwnmr: NMR_BATCH_TAG");
	Assert::check(config.getT2Tag(), (uint) 4000, "rwnmr: NMR_T2_TAG");
	Assert::check(config.getEndTag(), (uint) 5000, "rwnmr: NMR_END_TAG");
	
	cout << "Ok." << endl;
	return;
}

void ConfigFilesTest::TestUCTConfigFile()
{
	cout << "executing uct_config test...";

	string config_path = "tuct.config";
	uct_config config;
	config.readConfigFile((*this).getInputDir() + config_path);
	Assert::check(config.getDirPath(), (string)"./input/images/sphere_2.5voxels/", "uct: DirPath");
	Assert::check(config.getFilename(), (string)"sphere_r=2.5voxels_", "uct: Filename");
	Assert::check(config.getFirstIdx(), (uint) 0, "uct: FirstIdx");
	Assert::check(config.getDigits(), (uint) 1, "uct: Digits");
	Assert::check(config.getExtension(), (string)".png", "uct: Extension");
	Assert::check(config.getSlices(), (uint) 7, "uct: Slices");
	Assert::check(config.getResolution(), (double) 1.0, "uct: Resolution");
	Assert::check(config.getVoxelDivision(), (uint) 0, "uct: VoxelDivision");
	
	cout << "Ok." << endl;
	return;
}

void ConfigFilesTest::TestCPMGConfigFile()
{	
	cout << "executing cpmg_config test...";

	string config_path = "tcpmg.config";
	cpmg_config config;
	config.readConfigFile((*this).getInputDir() + config_path);
	Assert::check(config.getMethod(), (string)"image-based", "cpmg: Method");
	Assert::check(config.getObservationTime(), (double) 3000.0, "cpmg: ObservationTime");
	Assert::check(config.getApplyBulk(), (bool) false, "cpmg: ApplyBulk");
	Assert::check(config.getTimeVerbose(), (bool) false, "cpmg: TimeVerbose");
	Assert::check(config.getResidualField(), (string)"uniform", "cpmg: ResidualField");
	Assert::check(config.getGradientValue(), (double) 1.0, "cpmg: GradientValue");
	Assert::check(config.getGradientDirection(), (int) 2, "cpmg: GradientDirection");
	Assert::check(config.getMinT2(), (double) 0.1, "cpmg: MinT2");
	Assert::check(config.getMaxT2(), (double) 10000.0, "cpmg: MaxT2");
	Assert::check(config.getUseT2Logspace(), (bool) true, "cpmg: UseT2Logspace");
	Assert::check(config.getNumT2Bins(), (int) 256, "cpmg: NumT2Bins");
	Assert::check(config.getMinLambda(), (double) -4.0, "cpmg: MinLambda");
	Assert::check(config.getMaxLambda(), (double) 2.0, "cpmg: MaxLambda");
	Assert::check(config.getNumLambdas(), (int) 512, "cpmg: NumLambdas");
	Assert::check(config.getPruneNum(), (int) 512, "cpmg: PruneNum");
	Assert::check(config.getNoiseAmp(), (double) 0.0, "cpmg: NoiseAmp");
	Assert::check(config.getSaveMode(), (bool) true, "cpmg: SaveMode");
	Assert::check(config.getSaveDecay(), (bool) false, "cpmg: SaveDecay");
	Assert::check(config.getSaveT2(), (bool) false, "cpmg: SaveT2");
	Assert::check(config.getSaveWalkers(), (bool) false, "cpmg: SaveWalkers");
	Assert::check(config.getSaveHistogram(), (bool) false, "cpmg: SaveHistogram");
	Assert::check(config.getSaveHistogramList(), (bool) false, "cpmg: SaveHistogramList");

	cout << "Ok." << endl;
	return;
}

void ConfigFilesTest::TestPFGSEConfigFile()
{
	cout << "executing pfgse_config test...";

	string config_path = "tpfgse.config";
	pfgse_config config;
	config.readConfigFile((*this).getInputDir() + config_path);
	Assert::check(config.getPulseWidth(), (double) 0.1, "pfgse: PulseWidth");
	Assert::check((vector<double>) {
					config.getMaxGradient().getX(), 
					config.getMaxGradient().getY(), 
					config.getMaxGradient().getZ()}, 
					(vector<double>) {0.0, 0.0, 100.0}, 
					"pfgse: MaxGradient");
	Assert::check(config.getGradientSamples(), (uint) 256, "pfgse: GradientSamples");
	Assert::check(config.getApplyBulk(), (bool) false, "pfgse: ApplyBulk");
	Assert::check(config.getAllowWalkerSampling(), (bool) true, "pfgse: AllowWalkerSampling");
	Assert::check(config.getApplyAbsorption(), (bool) false, "pfgse: ApplyAbsorption");
	Assert::check(config.getTimeSequence(), (string)"manual", "pfgse: TimeSequence");
	Assert::check(config.getTimeSamples(), (uint) 4, "pfgse: TimeSamples");
	Assert::check(config.getTimeValues(), (vector<double>) {0.01,0.1,1.0,10.0}, "pfgse: TimeValues");
	Assert::check(config.getTimeMin(), (double) 10.0, "pfgse: TimeMin");
	Assert::check(config.getTimeMax(), (double) 800.0, "pfgse: TimeMax");
	Assert::check(config.getInspectionLength(), (double) 10.0, "pfgse: InspectionLength");
	Assert::check(config.getNoiseAmp(), (double) 0.0001, "pfgse: NoiseAmp");
	Assert::check(config.getTargetSNR(), (double) -1.0, "pfgse: TargetSNR");
	Assert::check(config.getThresholdType(), (string)"lhs", "pfgse: ThresholdType");
	Assert::check(config.getThresholdValue(), (double) 0.9, "pfgse: ThresholdValue");
	Assert::check(config.getThresholdWindow(), (uint) 5, "pfgse: ThresholdWindow");
	Assert::check(config.getSaveMode(), (bool) true, "pfgse: SaveMode");
	Assert::check(config.getSavePFGSE(), (bool) false, "pfgse: SavePFGSE");
	Assert::check(config.getSaveWalkers(), (bool) false, "pfgse: SaveWalkers");
	Assert::check(config.getSaveHistogram(), (bool) false, "pfgse: SaveHistogram");
	Assert::check(config.getSaveHistogramList(), (bool) false, "pfgse: SaveHistogramList");	

	cout << "Ok." << endl;
	return;

}

void ConfigFilesTest::TestMULTITAUConfigFile()
{
	cout << "executing multitau_config test...";

	cout << "Ok." << endl;
	return;
}