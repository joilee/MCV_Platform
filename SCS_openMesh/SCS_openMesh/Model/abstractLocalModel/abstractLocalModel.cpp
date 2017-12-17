#include "abstractLocalModel.h"

abstractLocalModel::abstractLocalModel()
{
	clearVandF();
}

abstractLocalModel::~abstractLocalModel()
{
	clearVandF();
}

void abstractLocalModel::clearVandF()
{
	V.clear();
	F.clear();
	f_materialId.clear();
	NF.clear();
	MinPos = Vector3d(0, 0, 0);
	MaxPos = Vector3d(0, 0, 0);
}



