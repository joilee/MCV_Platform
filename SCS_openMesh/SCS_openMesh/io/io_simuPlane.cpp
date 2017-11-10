#include "io_simuPlane.h"
#include <fstream>
#include <string>
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            *(result++) = item;
        }
    }
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

/************************************************************************/
/*
input：
output：
*/
/************************************************************************/
void saveSimuPlaneResult(std::vector<std::vector<EField>> &AP_EPoints,std::vector<int> vetical,std::vector<int> hori,const string filepath) {
    std::ofstream  output(filepath);
    output<<std::fixed;
    for (int i=0; i<AP_EPoints.size(); i++) {
        if (AP_EPoints[i].size()==0)
            continue;

        output<<"set "<<i<<" vertical "<<vetical[i]<<" horizon "<<hori[i]<<endl;//每个小区为一个组

        for (int j=0; j<AP_EPoints[i].size(); j++) {
            output<<"EField  "<<AP_EPoints[i][j].Position.x<<" "<<AP_EPoints[i][j].Position.y<<" "<<AP_EPoints[i][j].Position.z<<" "<<AP_EPoints[i][j].MolStrength<<" "<<AP_EPoints[i][j].Path.size()<<endl;
        }

    }
    output.close();
    return;
}

void loadSimuPlaneResult(std::vector<std::vector<EField>> &AP_EPoints,std::vector<int>& vetical,std::vector<int>& hori,const QString filepath) {
    AP_EPoints.clear();
    vetical.clear();
    hori.clear();

    std::ifstream fileIn(filepath.toStdString());
    if (!fileIn.is_open()) {
        //弹出一个对话框
        return ;
    }
    char buffer[256];

    while (!fileIn.eof()) {
        memset(buffer,0,256);
        fileIn.getline(buffer,256);
        std::string lineString=buffer;
        std::vector<std::string> tmp = split(lineString,' ');//
        if (!tmp.empty()&&tmp[0]=="set") { //新小区 创建
            std::vector<EField> thisSet;
            int verticalNum=atoi(tmp[3].c_str());
            int rowNum=atoi(tmp[5].c_str());
            vetical.push_back(verticalNum);
            hori.push_back(rowNum);


            tmp.clear();

            for (int i=0; i<(verticalNum+1)*(rowNum+1); i++) {
                //读取下一行
                //读取各个点数据
                memset(buffer,0,256);
                fileIn.getline(buffer,256);
                lineString=buffer;
                tmp = split(lineString,' ');
                if (tmp[0]=="EField") {
                    EField a;
                    a.Position.x=atof(tmp[1].c_str());
                    a.Position.y=atof(tmp[2].c_str());
                    a.Position.z=atof(tmp[3].c_str());
                    a.MolStrength=atof(tmp[4].c_str());
                    a.pathsize=atoi(tmp[5].c_str());
                    thisSet.push_back(a);
                }
            }
            AP_EPoints.push_back(thisSet);
        }
    }
}