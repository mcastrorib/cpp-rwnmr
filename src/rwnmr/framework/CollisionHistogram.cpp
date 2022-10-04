#include "CollisionHistogram.h"

CollisionHistogram::CollisionHistogram():size(0),
										 scale("linear"),
										 gap(0.0),
										 firstEcho(0),
										 lastEcho(0),
										 isSet(false)
{
	vector<double> amps();
	vector<double> bins();
}

CollisionHistogram::CollisionHistogram(int _size, string _scale):size(_size),
														 		 scale(_scale),
																 gap(0.0),
																 firstEcho(0),
																 lastEcho(0),
																 isSet(false)
{	
	// Initialize stl vectors
	vector<double> amps();
	vector<double> bins();

	if(this->size != 0)
		(*this).createBlankHistogram(this->size, this->scale);

}

CollisionHistogram::CollisionHistogram(const CollisionHistogram &_otherHistogram)
{
	this->size = _otherHistogram.size;
	this->scale = _otherHistogram.scale;
	this->gap = _otherHistogram.gap;
	this->amps = _otherHistogram.amps;
	this->bins = _otherHistogram.bins;
	this->firstEcho = _otherHistogram.firstEcho;
	this->lastEcho = _otherHistogram.lastEcho;
	this->isSet = _otherHistogram.isSet;
}

void CollisionHistogram::createBlankHistogram(int _size, string _scale)
{
	(*this).setScale(_scale);
	double gap = 1.0 / ((double) _size);
	(*this).setGap(gap);

	if((*this).getScale() == "log")
	{
		// reserve additional spot for 0 collisions in log scale
		(*this).setSize(_size + 1); 
	} else
	{
		(*this).setSize(_size);
	}

	this->amps.reserve(this->size);
	this->bins.reserve(this->size);
}

void CollisionHistogram::fillHistogram(vector<Walker> &_walkers, uint _numberOfSteps)
{
	if((*this).getScale() == "log")
	{
		(*this).createBinsLogVector(_walkers, _numberOfSteps);
		(*this).createAmpsLogVector(_walkers, _numberOfSteps);
	} else
	{
		(*this).createBinsLinearVector(_walkers);
		(*this).createAmpsLinearVector(_walkers, _numberOfSteps);
	}

	// assign set flag
	(*this).setIsSet(true);
}

void CollisionHistogram::updateHistogram(vector<Walker> &_walkers, uint _numberOfSteps)
{
	if((*this).getIsSet())
	{
		// clear previous amps 
		if((*this).getAmps().size() > 0) (*this).clearAmps();
		
		// create new amps
		if((*this).getScale() == "log") 
		{
			if((*this).getBins().size() > 0) 
			{
				(*this).clearBins();			
				(*this).reserveBins((*this).getSize());
			}
			(*this).createBinsLogVector(_walkers, _numberOfSteps);
			(*this).createAmpsLogVector(_walkers, _numberOfSteps);
		}
		else (*this).createAmpsLinearVector(_walkers, _numberOfSteps);
	}
}

void CollisionHistogram::createBinsLinearVector(vector<Walker> &_walkers)
{	
	double offset;
	double meanBin = (0.5) * (*this).getGap();
	for(int idx = 0; idx < (*this).getSize(); idx++)
	{	
		offset = idx * (*this).getGap();
		(*this).addBin(offset + meanBin);
	}
}

void CollisionHistogram::createAmpsLinearVector(vector<Walker> &_walkers, uint _numberOfSteps)
{	
	// initialize amps vector entries	
	for(int id = 0; id < this->size; id++)
	{
		(*this).addAmp(0.0);
	}


	// compute histogram
	int histogramIndex;
	double xi_rate;
	double steps = (double) _numberOfSteps;
	for(uint id = 0; id < _walkers.size(); id++)
	{
		xi_rate = _walkers[id].getCollisions() / steps;
		histogramIndex = floor(xi_rate / this->gap);
		if(histogramIndex >= this->size) histogramIndex--;
		(*this).setAmp((*this).getAmp(histogramIndex) + 1.0, histogramIndex);
	}

	// normalize histogram, i.e, histogram values sum 1.0
	double numberOfWalkers = (double) _walkers.size();
	for(int id = 0; id < this->size; id++)
	{
		(*this).setAmp((*this).getAmp(id) / numberOfWalkers, id);
	}	
}

void CollisionHistogram::createBinsLogVector(vector<Walker> &_walkers, uint _numberOfSteps)
{	
	// find min rate to generate bins accordingly
	double steps = (double) _numberOfSteps;
	uint first_idx = 0;
	while(first_idx < _walkers.size() and _walkers[first_idx].getCollisions() == 0)
	{
		first_idx++;	
	}

	if(first_idx < _walkers.size())
	{
		double min_rate = _walkers[first_idx].getCollisions() / steps;
		double xi_rate;
		for(uint id = first_idx; id < _walkers.size(); id++)
		{
			if(_walkers[id].getCollisions() != 0)
			{
				xi_rate = _walkers[id].getCollisions() / steps;
				if(xi_rate < min_rate)
				{
					min_rate = xi_rate;
				}
			}
		}


		// create vector of logspaced values 
		double logmin_rate = log10(min_rate);
		vector<double> logbins = MathFunctions::logspace(round(logmin_rate), 0.0, (*this).getSize() - 1);

		// first entry used for control no-collision cases
		(*this).addBin(round(logmin_rate) - 1.0);

		// other entries based on logspaced vector
		for(uint idx = 1; idx < (*this).getSize(); idx++)
		{
			(*this).addBin(logbins[idx - 1]);
		}

	} else
	{
		// dealing with free diffusion (no collision at all)
		for(uint idx = 0; idx < (*this).getSize(); idx++)
		{
			(*this).addBin(0.0);
		}
	}
}

void CollisionHistogram::createAmpsLogVector(vector<Walker> &_walkers, uint _numberOfSteps)
{	
	// 1st: initialize entries
	for(uint idx = 0; idx < (*this).getSize(); idx++)
	{
		(*this).addAmp(0.0);
	}

	
	// compute histogram
	int histogramIndex;
	double xi_rate;
	double logGap = log10((*this).getBin(2)) - log10((*this).getBin(1));
	double min_val = log10((*this).getBin(1));
	double steps = (double) _numberOfSteps;
	uint leaks = 0;

	for(uint id = 0; id < _walkers.size(); id++)
	{
		if(_walkers[id].getCollisions() == 0)
		{
			(*this).setAmp((*this).getAmp(0) + 1.0, 0);
		} else
		{
			xi_rate = (_walkers[id].getCollisions() / steps);
			histogramIndex = floor( round( (log10(xi_rate) - min_val) / logGap ) );
			histogramIndex += 1;
			if(histogramIndex > 0)
			{
				(*this).setAmp((*this).getAmp(histogramIndex) + 1.0, histogramIndex);
			} else
			{
				(*this).setAmp((*this).getAmp(1) + 1.0, 1);
				this->amps[1] += 1.0;		
			}
		}
	}

	// normalize histogram, i.e, histogram values sum 1.0
	double numberOfWalkers = (double) _walkers.size();
	for(int id = 0; id < (*this).getSize(); id++)
	{
		(*this).setAmp((*this).getAmp(id) / numberOfWalkers, id);
	}
}
