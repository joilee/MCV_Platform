#ifndef COMPUTE_PARA
#define COMPUTE_PARA
#include "../Antenna/antenna.h"
#include "../Antenna/receiver.h"
class ComputePara
{
public:
	ComputePara();
	~ComputePara();

	double phi;//正北方向在场景坐标系中的phi角度
	double leftUpX,leftUpY;
	double rightDownX,rightDownY;

	double precision,altitude;

	double  reflectNumPara;
	double  refractNumPara;
	double  diffractionNumPara;
	bool isDiffractionPara;
	int transIndex;
	double RT_sample;//采样球密度
	double RT_radius;//采样球半径
	double RT_BeamNum;//beam数量


	bool no_simplane;


	//
	vector<Site> Sites;  //多个小区

	vector<no_simplaneReceiver> No_SimPlanePoint; //非仿真面设置的接收点信息

private:

};




#endif