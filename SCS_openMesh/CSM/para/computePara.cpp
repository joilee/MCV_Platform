#include "computePara.h"

ComputePara::ComputePara()
{
	Sites.clear();
	no_simplane = false;
	phi=90;
	leftUpX=0;
	leftUpY=0;
	rightDownX=0;
	rightDownY=0;


	precision=0;
	altitude=0;

	reflectNumPara=2;
	refractNumPara=1;//透射

	diffractionNumPara=0;
	isDiffractionPara=false;
	RT_sample=0;
	RT_radius=0;
	RT_BeamNum=0;
}

ComputePara::~ComputePara()
{
	 vector<Site>().swap(Sites);
}