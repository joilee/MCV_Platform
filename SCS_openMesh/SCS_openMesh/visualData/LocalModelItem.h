#pragma once
#include <util/vector.h>
#include <vector>

class LocalModelItem
{
public:
	LocalModelItem();
	~LocalModelItem();

	inline int getLocalFaceNum(){ return LocalFaceNum; }
	inline Vector3d getlocalMin(){ return localMin; }
	inline Vector3d getlocalMax(){ return localMax; }

	void setLocalFaceNum(int a){ LocalFaceNum = a; }
	void setlocalMin(Vector3d a){ localMin = a; }
	void setlocalMax(Vector3d a){ localMax = a; }
	void setFlag(bool a){ updateLocal = a; }

	//增加局部场景数据
	void addLocalScene(int faceNum, Vector3d minBox, Vector3d maxBox);
	bool needUpdate(){ return updateLocal; }
	void clearLocal();
private:
	bool updateLocal;
	int LocalFaceNum;
	Vector3d localMin;
	Vector3d localMax;
};

