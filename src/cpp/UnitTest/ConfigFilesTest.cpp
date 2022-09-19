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

	cout << "Ok." << endl;
	return;
}

void ConfigFilesTest::TestCPMGConfigFile()
{	
	cout << "executing cpmg_config test...";

	cout << "Ok." << endl;
	return;
}

void ConfigFilesTest::TestPFGSEConfigFile()
{
	cout << "executing pfgse_config test...";

	cout << "Ok." << endl;
	return;

}

void ConfigFilesTest::TestMULTITAUConfigFile()
{
	cout << "executing multitau_config test...";

	cout << "Ok." << endl;
	return;
}