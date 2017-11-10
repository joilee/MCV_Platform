#ifndef ALGO_H
#define ALGO_H

#include "geometry/emxKdTreeD.h"
#include "geometry/SphereBeam.h"
#include "util/EField.h"
#include <para/modelPara.h>
#include <para/computePara.h>
#include <omp.h>  //OpenMp���б����Ҫ������ͷ�ļ�


struct measured_field
{
	Vector3d position;
	double RSRP;
};
struct PCI_measured_field
{
	int PCI;
	vector<measured_field> M_Field;
};
struct site_measured_field
{
	Vector3d AP_position;
	vector<PCI_measured_field> PCIs_Fields;
};
class beamNode{ //·���е�beam�ڵ���
public:
	int beam_type; //beam�����ֵΪ0��reflectedbeam��ֵΪ1��transmittedbeam, ֵΪ2��childbeam
	Vector3d origin;		// Beam origin ��(��)��׶��������
	Vector3d BeamVertex[4]; //��(��)��׶����׶���涥��
	Vector3d beamNormal;
	int faceID; //beam�ཻ����ƬID��
	int		 cornerNum;		// Number of beam corners
};


struct Scene_para 
{
	int scene_length;
	int scene_width;
	double Xstep,Ystep;
};

struct Antenna_Para
{
	double TP_gain;
	double frequency;
	double light_speed;
	double epsilon0;
	double lamda;
	double k; 
	double w;
	double EveryRayPowerStrength;
	double NorthAngle;
};
void CreateInitialBeam(vector<emxBeam*> &pRootBeams, SphereBeam* SphereTest, Vector3d AP_position,int BeamNumber); //λ��ԭ�㴦�ĵ�λ��ϸ��count�����ɳ�ʼbeams

void BeamTracing(emxKdTree* pKdTree, const int &reflectnum, emxBeam* &beam);

void find_beamroute(emxBeam *pRootBeam, vector<vector<beamNode>> &routes);

void SetEFieldPoint(vector<EField>  &EFieldArray, Vector3d AP_position, double LocalScene_range,double Zheight,double Precetion ,Scene_para &sp,ModelPara *mP);   //���ý��յ�����

void Point_In_Out(vector<EField>  &EFieldArray,vector<Building> &Local_buildings,double Zheight,Scene_para &sp);

void EfieldPointInPolygon(vector<Vector3d>& polygon,double Xmin,double Ymin,int &start_rowId,int &end_rowId,int &start_columnId,int &end_columnId,Scene_para sp);  //��ȡ�ڶ�����ڲ����id��Ϣ

bool contain(vector<Vector3d>& polygon, Vector3d point0);

void valid_DirPath(emxKdTree* pKdTree, Vector3d AP_position, vector<EField>  &EFieldArray);

void Calc_GO_UTD( TransAntenna &AP, vector<EField>  &EFieldArray,vector<Vedge> &Edge_list,Antenna_Para & cPara,ComputePara * c_para,ModelPara *mp);//���㷴�䡢͸�䡢����·��������յ���ź�ǿ��

void Calc_DirPathSignal(EField &NField ,TransAntenna &AP, int &path_id,Antenna_Para a);

void Calc_diffSignal1(vector<Vedge> &Edge_list,EField &NField,TransAntenna &AP, int &path_id,Antenna_Para aP,ModelPara *mp);

bool getDiffEdgeINfor(Vedge& info, const Vector3d& source_pos, const Vector3d& diffract_pos);

complex<double>getEpsilon(double freq/*��λ:HZ*/, int materialId,ModelPara *mp);   //��Խ�糣��

int getMaterial(double freq /*��λ:HZ*/,int materialId,ModelPara *mp);

void Calc_RefTransSignal(EField &NField,TransAntenna &AP, int &path_id,Antenna_Para ap,ModelPara *mp,ComputePara *cp); //���㷴͸��·���ź�ǿ��

int  sign_func(double x)  ;//���ź���

int roundInt(double x) ;//��������

complex<double>Fresnel( double xf );

bool Domp(const vector<beamNode> &a, const vector<beamNode> &b);
#endif