#include "LocalModelItem.h"

LocalModelItem::LocalModelItem()
{
	LocalFaceNum = 0;
	localMin = Vector3d(0, 0, 0);
	localMax = Vector3d(0, 0, 0);
	updateLocal = false;
}

LocalModelItem::~LocalModelItem()
{

}


void LocalModelItem::addLocalScene(int faceNum, Vector3d minBox, Vector3d maxBox)
{
	setLocalFaceNum(faceNum);
	cout << "just debug" << endl;
	setlocalMin(minBox);
	setlocalMax(maxBox);
}
void LocalModelItem::clearLocal()
{
	LocalFaceNum = 0;
	localMin = Vector3d(0, 0, 0);
	localMax = Vector3d(0, 0, 0);
}