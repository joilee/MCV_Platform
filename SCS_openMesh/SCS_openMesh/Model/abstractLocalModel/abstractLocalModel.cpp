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
}



