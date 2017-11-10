#ifndef IOOBJ
#define IOOBJ
#include "Mesh.h"
#include <map>
using namespace std;

void WriteOBJ(FILE *f, MESH_PTR pMesh);
void WriteOBJ_No_simplify(FILE *f, MESH_PTR pMesh);
#endif