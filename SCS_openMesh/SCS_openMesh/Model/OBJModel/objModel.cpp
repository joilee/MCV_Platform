#include "objModel.h"
#include "../../Context/context.h"

objModel::objModel(string objPath)
{
	id = 0;
	name = "";
	ifstream fin(objPath, ios::in);

	string line;
	char type;

	double x, y, z;
	int v1, v2, v3;

	pMax = Vector3d(DBL_MIN, DBL_MIN, DBL_MIN);
	pMin = Vector3d(DBL_MAX, DBL_MAX, DBL_MAX);

	while (getline(fin, line))
	{
		//读取点坐标信息
		if (line.find_first_of('v') == 0)
		{
			istringstream lineStream(line);
			lineStream >> type >> x >> y >> z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			points.push_back(Pot(x, y, z));

			//x,y,z的最大值与最小值
			pMax = Max(pMax, Vector3d(x, y, z));
			pMin = Min(pMin, Vector3d(x, y, z));
		}

		//读取面信息
		if (line.find_first_of('f') == 0)
		{
			istringstream lineStream(line);
			lineStream >> type >> v1 >> v2 >> v3;
			faces.push_back(v1);
			faces.push_back(v2);
			faces.push_back(v3);
		}
	}
	fin.close();
}

void objModel::draw(vector<bool> mode, double alpha)
{

}

void objModel::initDraw()
{
	globalContext *globalCtx = globalContext::GetInstance();
	
	uniform_alpha = 1;
	int defaultID = globalCtx->modelManager->matManager->getDefaultMaterial();
	int index = globalCtx->modelManager->matManager->getVectorIndexFromID(defaultID);
	uniformColor = globalCtx->modelManager->matManager->getColor(index);
	

}

//计算高度
double objModel::getAltitude(double x, double y)
{
	double minZ = DBL_MAX;
	for (int i = 0; i < faces.size() / 3; i = i + 3)
	{
		Pot a = points[faces[i]];
		Pot b = points[faces[i + 1]];
		Pot c = points[faces[i + 2]];
		Pot p = Pot(x, y, 0.0);

		Vector3d ab(b.x - a.x, b.y - a.y, b.z - a.z);
		Vector3d ap(p.x - a.x, p.y - a.y, p.z - a.z);
		Vector3d ac(c.x - a.x, c.y - a.y, c.z - a.z);

		double u = (ap.x*ab.y - ab.x*ap.y) / (ac.x*ab.y - ab.x*ac.y);
		double v = (ap.x*ac.y - ac.x*ap.y) / (ab.x*ac.y - ac.x*ab.y);

		if (u < 0 || v<0 || (u + v)>1)
		{
		}
		else
		{
			float z = (a.z + b.z + c.z) / 3.0;
			if (z < minZ) minZ = z;
		}
	}
	return minZ;
}

//返回坐标最大值
Vector3d objModel::getMaxPoint()
{
	return pMax;
}

//返回坐标最小值
Vector3d objModel::getMinPoint()
{
	return pMin;
}

void objModel::getNormals()
{
	for (int i = 0; i < faces.size() / 3; i = i + 3)
	{
		Pot a = points[faces[i]];
		Pot b = points[faces[i + 1]];
		Pot c = points[faces[i + 2]];

		Vector3d n = VectorCross(Vector3d(b.x - a.x, b.y - a.y, b.z - a.z), Vector3d(c.x - a.x, c.y - a.y, c.z - a.z));
		n = n.normalize();

		if (n.z < 0)
			n = -1.0 * n;
		normals.push_back(n);
	}
}

