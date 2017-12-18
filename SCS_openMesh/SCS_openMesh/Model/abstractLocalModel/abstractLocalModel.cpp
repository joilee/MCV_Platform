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
	MinPos = Vector3d(DBL_MAX, DBL_MAX, DBL_MAX);
	MaxPos = Vector3d(DBL_MIN, DBL_MIN, DBL_MIN);
}



