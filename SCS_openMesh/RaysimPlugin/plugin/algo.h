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
#include <omp.h>  //OpenMp并行编程需要包含的头文件
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

//棱边可见接收点信息
struct Edge_LosPointInfo
{
	int ReceiverID;    //棱边可见接收点索引号
	double dis_edgeToReceiver;   //接收点与棱边的水平距离
	Vector3d Receiver_onEdge;   //接收点在棱边上的水平映射
};
//源可见棱边信息
struct Source_LosEdgeInfo
{
	double dis_sourceToEdge;   //源点与棱边的水平距离
	Vector3d source_onEdge;   //源点在棱边上的水平映射
};

class beamNode{ //路径中的beam节点类
public:
	int beam_type; //beam的类别，值为0是reflectedbeam，值为1是transmittedbeam, 值为2是childbeam
	Vector3d origin;		// Beam origin 三(四)角锥光束顶点
	Vector3d BeamVertex[4]; //三(四)角锥光束锥底面顶点
	Vector3d beamNormal;
	int faceID; //beam相交的面片ID号
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
	
	Scene_para scenePara;//场景的参数，比如有多少行多少列，精度等。

private:
	void CreateInitialBeam(vector<emxBeam*> &pRootBeams, SphereBeam* SphereTest, Vector3d AP_position); //位于原点处的单位球，细分count次生成初始beams

	void BeamTracing(emxKdTree* pKdTree, const int &reflectnum, emxBeam* &beam);

	void find_beamroute(emxBeam *pRootBeam, vector<vector<beamNode>> &routes);

	void SetEFieldPoint(Site_Data* m_siteData, Vector3d AP_position, double LocalScene_range);   //设置接收点坐标

	void Point_In_Out( Site_Data* sitedata, vector<Building> &Local_buildings);

	void EfieldPointInPolygon(vector<Vector3d>& polygon, double Xmin, double Ymin, int &start_rowId, int &end_rowId, int &start_columnId, int &end_columnId);  //获取在多边形内部点的id信息

	bool contain(vector<Vector3d>& polygon, Vector3d point0);

	void valid_DirPath(emxKdTree* pKdTree, Vector3d AP_position, Site_Data *m_Data);

	void Calc_GO_UTD(TransAntenna &AP, vector<EField*>  &EFieldArray, vector<Vedge> &Edge_list, Antenna_Para * cPara);//计算反射、透射、绕射路径到达接收点的信号强度

	void Calc_Signal(EField *NField, TransAntenna &AP, int &path_id, vector<Vedge> &Edge_list, Antenna_Para *a);

	void Calc_RefEfield(Vector3cd &ctfield, Vector3d into, Vector3d normal, Vector3d next_into, TransAntenna &AP);  //计算反射场

	void Calc_TransEfield(Vector3cd &ctfield, Vector3d into, Vector3d normal, TransAntenna &AP); //计算透射场

	void Calc_diffEfield(Vector3cd &ctfield, Vector3d source_pos, Vector3d Diffract_Pos, Vector3d field_pos, Vedge the_edge, TransAntenna &AP, Antenna_Para *a);  //计算绕射场

	void Calc_diffSignal1(vector<Vedge> &Edge_list, EField *NField, TransAntenna &AP, int &path_id, Antenna_Para *aP);

	bool getDiffEdgeINfor(Vedge& info, const Vector3d& source_pos, const Vector3d& diffract_pos);

	complex<double>getEpsilon(double freq/*单位:HZ*/, int materialId);   //相对介电常数

	int getMaterial(double freq /*单位:HZ*/, int materialId);

	void valid_RefTransPath(BaseModel* model,emxKdTree* pKdTree,  Vector3d AP_position, Site_Data* m, const vector<vector<beamNode>> &beamRoutes);

	int  sign_func(double x);//符号函数

	int roundInt(double x);//四舍五入

	complex<double>Fresnel(double xf);

	bool Domp(const vector<beamNode> &a, const vector<beamNode> &b);

	bool intersect(emxRay ray, Vector3d v0, Vector3d v1, Vector3d v2, Vector3d &intersectPoint);

	void valid_OnceDiffPath(vector<Vedge> &Edge_list, emxKdTree* pKdTree, Vector3d AP_position, Site_Data*m); //找出有效的一次绕射路径

	void valid_NVDiffPath(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, Vector3d AP_position, Site_Data* m_siteData, int diff_Num);
	//组合路径
	void valid_RefDiffPath(emxKdTree* pKdTree, vector<Vedge> &Edge_list, Vector3d AP_position, Site_Data* m_Data, const vector<vector<beamNode>> &beamRoutes); //找出有效的反射+最后一次绕射路径

	bool is_diffEdge(beamNode &the_beam, vector<Vedge> &Edge_list, int &beamedge_id, int &edge_id);

	//获取所有可能的 <=N次 绕射传播的棱边序列
	void Get_diffVEdge(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<vector<int>>&diff_RoutesEdgeID, vector<vector<int>> & NextDifEdgesID, int diff_Num, int first_VerticalEdgeid);

	//提前获取源点的可见棱边信息（可见棱边索引ID、源点在棱边上的水平映射、源点与棱边的水平距离）
	void Get_SourceLosEdgesInfo(Vector3d source_pos, vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, map<int, Source_LosEdgeInfo> &Source_LosEdges);

	//粗略判断对每条棱边下一可能发生绕射的棱边集合
	void Get_nextDifEdge(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<vector<int>> & NextDifEdgesID);

	//获取每条棱边简单判断的可到达接收点信息（接收点索引编号，接收点在该棱边上的水平映射，接收点到该棱边的水平距离）
	void Get_EdgeLosFieldPoint(vector<int>&Vertical_Edge_ID, vector<Vedge> &Edge_list, emxKdTree* pKdTree, vector<EField*>  &EFieldArray, vector<vector<Edge_LosPointInfo>> & edges_LosFieldPoints);


};



#endif