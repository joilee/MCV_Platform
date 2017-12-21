#include "objLocalModel.h"
#include "../../Context/context.h"

objLocalModel::objLocalModel(Vector3d center, double localRange, string _name, objModel* cModel)
{
	clearVandF();

	range = localRange;
	name = _name;
	id = atoi(_name.c_str());
	fileType = ModelType::OBJ_LOCAL;

	globalContext *globalCtx = globalContext::GetInstance();
	int defaultID = globalCtx->modelManager->matManager->getDefaultMaterial();
	f_materialId = vector<int>(F.size(), defaultID);

	tempVertices = cModel->getPoints();
	tempFaces = cModel->getFaces();
	tempNormals = cModel->getFaceNormals();

	generateLocalScence(center);
	intDraw();

	tempFaces.clear();
	tempVertices.clear();
	tempNormals.clear();
}

double objLocalModel::getAltitude(double x, double y)
{
	double minZ = DBL_MAX;
	for (int i = 0; i < F.size(); i++)
	{
		Vector3d a = V[F[i].x];
		Vector3d b = V[F[i].y];
		Vector3d c = V[F[i].z];
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

vector<double> objLocalModel::calculateCoffs(Vector3d a, Vector3d b, Vector3d c)
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


void objLocalModel::draw(vector<bool> mode, double alpha)
{
	if (mode[1]) { //draw line
		//glCallList(showWireList);
		glDisable(GL_LIGHTING);
		glLineWidth(2.0f);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		for (int i = 0; i < F.size(); i++) {
			for (int j = 0; j < 3; j++) {
				glVertex3d((GLdouble)V[F[i][j]].x, (GLdouble)V[F[i][j]].y, (GLdouble)V[F[i][j]].z);
				glVertex3d((GLdouble)V[F[i][(j + 1) % 3]].x, (GLdouble)V[F[i][(j + 1) % 3]].y, (GLdouble)V[F[i][(j + 1) % 3]].z);
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

void objLocalModel::intDraw()
{
	globalContext *globalCtx = globalContext::GetInstance();

	uniform_alpha = 1;
	int defaultID = globalCtx->modelManager->matManager->getDefaultMaterial();
	int index = globalCtx->modelManager->matManager->getVectorIndexFromID(defaultID);
	uniformColor = globalCtx->modelManager->matManager->getColor(index);

	f_materialId = vector<int>(F.size(), defaultID);

	for (int i = 0; i < F.size(); i++)
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

	for (int i = 0; i < F.size(); i++) {
		Vector3i vIndex = F[i];//3个点
		for (int j = 0; j < 3; j++) {
			//索引坐标
			indices.push_back(i * 3 + j);
			//点的坐标
			vertices.push_back(V[vIndex[j]].x);
			vertices.push_back(V[vIndex[j]].y);
			vertices.push_back(V[vIndex[j]].z);
			//向量坐标
			normals.push_back(NF[i].x);
			normals.push_back(NF[i].y);
			normals.push_back(NF[i].z);
		}
	}

	showWireList = glGenLists(1);
	glNewList(showWireList, GL_COMPILE);
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < F.size(); i++) {
		for (int j = 0; j < 3; j++) {
			glVertex3d((GLdouble)V[F[i][j]].x, (GLdouble)V[F[i][j]].y, (GLdouble)V[F[i][j]].z);
			glVertex3d((GLdouble)V[F[i][(j + 1) % 3]].x, (GLdouble)V[F[i][(j + 1) % 3]].y, (GLdouble)V[F[i][(j + 1) % 3]].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEndList();

}

void objLocalModel::generateLocalScence(Vector3d center)
{
	int pID = 0;
	int fID = 0;

	for (int i = 0; i < tempFaces.size(); i++)
	{
		Vector3i face;
		if (isInside(center, tempFaces[i]))
		{
			if (!isInMap(tempFaces[i].x))//有新点加入
			{
				pointID.insert(pair<int, int>(tempFaces[i].x, pID));
				V.push_back(tempVertices[tempFaces[i].x]);
				face.x = pID;
				MaxPos = Max(tempVertices[tempFaces[i].x], MaxPos);
				MinPos = Min(tempVertices[tempFaces[i].x], MaxPos);
				pID++;
			}
			else
			{
				face.x = pointID[tempFaces[i].x];
			}


			if (!isInMap(tempFaces[i].y))
			{
				pointID.insert(pair<int, int>(tempFaces[i].y, pID));
				V.push_back(tempVertices[tempFaces[i].y]);
				face.y = pID;
				MaxPos = Max(tempVertices[tempFaces[i].x], MaxPos);
				MinPos = Min(tempVertices[tempFaces[i].x], MaxPos);
				pID++;
			}
			else
				face.y = pointID[tempFaces[i].y];

			if (!isInMap(tempFaces[i].z))
			{
				pointID.insert(pair<int, int>(tempFaces[i].z, pID));
				V.push_back(tempVertices[tempFaces[i].z]);
				face.z = pID;
				MaxPos = Max(tempVertices[tempFaces[i].x], MaxPos);
				MinPos = Min(tempVertices[tempFaces[i].x], MaxPos);
				pID++;
			}
			else
				face.z = pointID[tempFaces[i].z];

			F.push_back(face);
			NF.push_back(tempNormals[i]);
		}
	}
}

bool objLocalModel::isInMap(int id)
{
	bool flag = false;
	map<int, int>::iterator iter;
	iter = pointID.find(id);
	if (iter != pointID.end())
		flag = true;
	return flag;
}


bool objLocalModel::isInside(Vector3d center, Vector3i face)
{
	bool flag = false;

	double lx = center.x - range / 2;
	double rx = center.x + range / 2;
	double ty = center.y + range / 2;
	double by = center.y - range / 2;

	Vector3d p1 = tempVertices[face.x];
	Vector3d p2 = tempVertices[face.y];
	Vector3d p3 = tempVertices[face.z];

	if (p1.x > lx && p1.x<rx && p1.y>by &&p1.y < ty)
		flag = true;
	if (p2.x > lx && p2.x<rx && p2.y>by &&p2.y < ty)
		flag = true;
	if (p3.x > lx && p3.x<rx && p3.y>by &&p3.y < ty)
		flag = true;

	return flag;
}
