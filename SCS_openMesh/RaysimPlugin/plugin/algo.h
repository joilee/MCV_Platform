#ifndef ALGO_H
#define ALGO_H

#include "geometry/emxFace.h"
#include "geometry/emxModel.h"
#include "geometry/emxKdTreeD.h"
#include "geometry/SphereBeam.h"
#include <util/EField.h>
#include <para/modelPara.h>
#include <para/computePara.h>
#include <para/baseModel.h>
#include <para/visualPara.h>
#include <omp.h>  //OpenMp���б����Ҫ������ͷ�ļ�
#include <QInputDialog>
#include <QMessageBox>
#include <QTextBrowser>
#include "GeneratedFiles/ui_log.h"
#include "LogText.h"
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

//��߿ɼ����յ���Ϣ
struct Edge_LosPointInfo
{
	int ReceiverID;    //��߿ɼ����յ�������
	double dis_edgeToReceiver;   //���յ�����ߵ�ˮƽ����
	Vector3d Receiver_onEdge;   //���յ�������ϵ�ˮƽӳ��
};
//Դ�ɼ������Ϣ
struct Source_LosEdgeInfo
{
	double dis_sourceToEdge;   //Դ������ߵ�ˮƽ����
	Vector3d source_onEdge;   //Դ��������ϵ�ˮƽӳ��
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

class algo
{
public:
	algo();
	~algo();
	void pluginAlgo(ModelPara *modelParameter, ComputePara *cptPara, visPara *vPara);
private:
	double Diff_dis;
	int BeamNumber;
	ModelPara *modelParameter;
	ComputePara *cptPara ;
	visPara *vPara;
	LogText *m_logText;
	
	Scene_para scenePara;//�����Ĳ����������ж����ж����У����ȵȡ�

private:
	void CreateInitialBeam(vector<emxBeam*> &pRootBeams, SphereBeam* SphereTest, Vector3d AP_position); //λ��ԭ�㴦�ĵ�λ��ϸ��count�����ɳ�ʼbeams

	void BeamTracing(emxKdTree* pKdTree, const int &reflectnum, emxBeam* &beam);

	void find_beamroute(emxBeam *pRootBeam, vector<vector<beamNode>> &routes);

	void SetEFieldPoint(Site_Data* m_siteData, Vector3d AP_position, double LocalScene_range);   //���ý��յ�����

	void Point_In_Out( Site_Data* sitedata, vector<Building> &Local_buildings);

	void EfieldPointInPolygon(vector<Vector3d>& polygon, double Xmin, double Ymin, int &start_rowId, int &end_rowId, int &start_columnId, int &end_columnId);  //��ȡ�ڶ�����ڲ����id��Ϣ

	bool contain(vector<Vector3d>& polygon, Vector3d point0);

	void valid_DirPath(emxKdTree* pKdTree, Vector3d AP_position, Site_Data *m_Data);

	void Calc_GO_UTD(TransAntenna &AP, vector<EField*>  &EFieldArray, vector<Vedge> &Edge_list, Antenna_Para * cPara);//���㷴�䡢͸�䡢����·��������յ���ź�ǿ��

	void Calc_Signal(EField *NField, TransAntenna &AP, int &path_id, vector<Vedge> &Edge_list, Antenna_Para *a);

	void Calc_RefEfield(Vector3cd &ctfield, Vector3d into, Vector3d normal, Vector3d next_into, TransAntenna &AP);  //���㷴�䳡

	void Calc_TransEfield(Vector3cd &ctfield, Vector3d into, Vector3d normal, TransAntenna &AP); //����͸�䳡

	void Calc_diffEfield(Vector3cd &ctfield, Vector3d source_pos, Vector3d Diffract_Pos, Vector3d field_pos, Vedge the_edge, TransAntenna &AP, Antenna_Para *a);  //�������䳡

	void Calc_diffSignal1(vector<Vedge> &Edge_list, EField *NField, TransAntenna &AP, int &path_id, Antenna_Para *aP);

	bool getDiffEdgeINfor(Vedge& info, const Vector3d& source_pos, const Vector3d& diffract_pos);

	complex<double>getEpsilon(double freq/*��λ:HZ*/, int materialId);   //��Խ�糣��

	int getMaterial(double freq /*��λ:HZ*/, int materialId);

	void valid_RefTransPath(BaseModel* model,emxKdTree* pKdTree,  Vector3d AP_position, Site_Data* m, const vector<vector<beamNode>> &beamRoutes);

	int  sign_func(double x);//���ź���

	int roundInt(double x);//��������

	complex<double>Fresnel(double xf);

	bool Domp(const vector<beamNode> &a, const vector<beamNode> &b);

	bool intersect(emxRay ray, Vector3d v0, Vector3d v1, Vector3d v2, Vector3d &intersectPoint);

	void valid_OnceDiffPath(vector<Vedge> &Edge_list, emxKdTree* pKdTree, Vector3d AP_position, Site_Data*m); //�ҳ���Ч��һ������·��

	void valid_NVDiffPath(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, Vector3d AP_position, Site_Data* m_siteData, int diff_Num);
	//���·��
	void valid_RefDiffPath(emxKdTree* pKdTree, vector<Vedge> &Edge_list, Vector3d AP_position, Site_Data* m_Data, const vector<vector<beamNode>> &beamRoutes); //�ҳ���Ч�ķ���+���һ������·��

	bool is_diffEdge(beamNode &the_beam, vector<Vedge> &Edge_list, int &beamedge_id, int &edge_id);

	//��ȡ���п��ܵ� <=N�� ���䴫�����������
	void Get_diffVEdge(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<vector<int>>&diff_RoutesEdgeID, vector<vector<int>> & NextDifEdgesID, int diff_Num, int first_VerticalEdgeid);

	//��ǰ��ȡԴ��Ŀɼ������Ϣ���ɼ��������ID��Դ��������ϵ�ˮƽӳ�䡢Դ������ߵ�ˮƽ���룩
	void Get_SourceLosEdgesInfo(Vector3d source_pos, vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, map<int, Source_LosEdgeInfo> &Source_LosEdges);

	//�����ж϶�ÿ�������һ���ܷ����������߼���
	void Get_nextDifEdge(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<vector<int>> & NextDifEdgesID);

	//��ȡÿ����߼��жϵĿɵ�����յ���Ϣ�����յ�������ţ����յ��ڸ�����ϵ�ˮƽӳ�䣬���յ㵽����ߵ�ˮƽ���룩
	void Get_EdgeLosFieldPoint(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<EField*>  &EFieldArray, vector<vector<Edge_LosPointInfo>> & edges_LosFieldPoints);


};



#endif