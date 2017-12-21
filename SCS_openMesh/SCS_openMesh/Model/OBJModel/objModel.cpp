#include "objModel.h"
#include "../../Context/context.h"

objModel::objModel(string objPath)
{
	id = 0;
	fileType = ModelType::OBJ;
	int lPos = objPath.find_last_of('\\');
	int rPos = objPath.find('.');
	name = objPath.substr(lPos+1,rPos-1);

	readObj(objPath);
	uniformColor = Color(0.0f, 0.0f, 0.0f);
	getNormals();
	initDraw();
}


void objModel::readObj(string objPath)
{
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

			points.push_back(Vector3d(x, y, z));

			//x,y,z的最大值与最小值
			pMax = Max(pMax, Vector3d(x, y, z));
			pMin = Min(pMin, Vector3d(x, y, z));
		}

		//读取面信息
		if (line.find_first_of('f') == 0)
		{
			istringstream lineStream(line);
			lineStream >> type >> v1 >> v2 >> v3;
			faces.push_back(Vector3i(v1-1,v2-1,v3-1));
		}
	}
	fin.close();

}


void objModel::draw(vector<bool> mode, double alpha)
{
	if (mode[1]) { //draw line
		//glCallList(showWireList);
		glDisable(GL_LIGHTING);
		glLineWidth(2.0f);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		for (int i = 0; i < faces.size(); i++) {
			for (int j = 0; j < 3; j++) {
				glVertex3d((GLdouble)points[faces[i][j]].x, (GLdouble)points[faces[i][j]].y, (GLdouble)points[faces[i][j]].z);
				glVertex3d((GLdouble)points[faces[i][(j + 1) % 3]].x, (GLdouble)points[faces[i][(j + 1) % 3]].y, (GLdouble)points[faces[i][(j + 1) % 3]].z);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	if (mode[2]) { //draw face
		// enable and specify pointers to vertex arrays
		glEnableClientState(GL_NORMAL_ARRAY);;
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4f(uniformColor.r / 256.0, uniformColor.g / 256.0, uniformColor.b / 256.0, alpha);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

}

void objModel::initDraw()
{
	globalContext *globalCtx = globalContext::GetInstance();
	
	uniform_alpha = 1;
	int defaultID = globalCtx->modelManager->matManager->getDefaultMaterial();
	int index = globalCtx->modelManager->matManager->getVectorIndexFromID(defaultID);
	uniformColor = globalCtx->modelManager->matManager->getColor(index);

	f_materialId = vector<int>(faces.size(), defaultID);

	for (int i = 0; i < faces.size();i++)
	{
		int index = globalCtx->modelManager->matManager->getVectorIndexFromID(f_materialId[i]);
		Color tmp = globalCtx->modelManager->matManager->getColor(index);

		vector<float> colorVector;
		colorVector.push_back((double)tmp.r / 256.0);
		colorVector.push_back((double)tmp.g / 256.0);
		colorVector.push_back((double)tmp.b / 256.0);
		colorVector.push_back(uniform_alpha);
		faceColor.push_back(colorVector);
	}

	for (int i = 0; i < faces.size(); i++) {
		Vector3i vIndex = faces[i];//3个点
		for (int j = 0; j < 3; j++) {
			//索引坐标
			indices.push_back(i * 3 + j);
			//点的坐标
			vertices.push_back(points[vIndex[j]].x);
			vertices.push_back(points[vIndex[j]].y);
			vertices.push_back(points[vIndex[j]].z);
			//向量坐标
			m_normals.push_back(normals[i].x);
			m_normals.push_back(normals[i].y);
			m_normals.push_back(normals[i].z);
		}
	}

	showWireList = glGenLists(1);
	glNewList(showWireList, GL_COMPILE);
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			glVertex3d((GLdouble)points[faces[i][j]].x, (GLdouble)points[faces[i][j]].y, (GLdouble)points[faces[i][j]].z);
			glVertex3d((GLdouble)points[faces[i][(j + 1) % 3]].x, (GLdouble)points[faces[i][(j + 1) % 3]].y, (GLdouble)points[faces[i][(j + 1) % 3]].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEndList();
}

//计算高度
double objModel::getAltitude(double x, double y)
{
	double minZ = DBL_MAX;
	for (int i = 0; i < faces.size(); i++)
	{
		Vector3d a = points[faces[i].x];
		Vector3d b = points[faces[i].y];
		Vector3d c = points[faces[i].z];
		Vector3d p = Vector3d(x, y, 0.0);

		Vector3d ab(b.x - a.x, b.y - a.y, b.z - a.z);
		Vector3d ap(p.x - a.x, p.y - a.y, p.z - a.z);
		Vector3d ac(c.x - a.x, c.y - a.y, c.z - a.z);

		double u = (ap.x*ab.y - ab.x*ap.y) / (ac.x*ab.y - ab.x*ac.y);
		double v = (ap.x*ac.y - ac.x*ap.y) / (ab.x*ac.y - ac.x*ab.y);

		if (!(u < 0 || v<0 || (u + v)>1))
		{
			vector<double> coffs(4);
			coffs = calculateCoffs(a, b, c);
			double z = (-coffs[3] - x*coffs[0] - y* coffs[1]) / coffs[2];
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
	for (int i = 0; i < faces.size();i++)
	{
		Vector3d a = points[faces[i].x];
		Vector3d b = points[faces[i].y];
		Vector3d c = points[faces[i].z];

		Vector3d n = VectorCross(Vector3d(b.x - a.x, b.y - a.y, b.z - a.z), Vector3d(c.x - a.x, c.y - a.y, c.z - a.z));
		n = n.normalize();

		if (n.z < 0)
			n = -1.0 * n;
		normals.push_back(n);
	}
}

vector<double> objModel::calculateCoffs(Vector3d a, Vector3d b, Vector3d c)
{
	vector<double> coffs(4);

	coffs[0] = (b.y - a.y)*(c.z - a.z) - (b.z - a.z)*(c.y - a.y);
	coffs[1] = (b.z - a.z)*(c.x - a.x) - (b.x - a.x)*(c.z - a.z);
	coffs[2] = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
	coffs[3] = -(coffs[0] * a.x + coffs[1] * a.y + coffs[1] * a.z);

	double s = sqrt(coffs[0] * coffs[0] + coffs[1] * coffs[1] + coffs[2] * coffs[2] + coffs[3] * coffs[3]);

	coffs[0] = coffs[0] / s;
	coffs[1] = coffs[1] / s;
	coffs[2] = coffs[2] / s;
	coffs[3] = coffs[3] / s;

	return coffs;
}

