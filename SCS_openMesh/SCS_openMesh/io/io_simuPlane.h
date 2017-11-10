#ifndef IO_SIMPLANE
#define IO_SIMPLANE

#include "util/EField.h"
#include <vector>
#include <QString>
void saveSimuPlaneResult(std::vector<std::vector<EField>> &AP_EPoints,std::vector<int> vetical,std::vector<int> hori,const string filepath);

void loadSimuPlaneResult(std::vector<std::vector<EField>> &AP_EPoints,std::vector<int>& vetical,std::vector<int>& hori,const QString filepath);

#endif
