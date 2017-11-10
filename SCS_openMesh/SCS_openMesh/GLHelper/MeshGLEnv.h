#ifndef __MESHGLENV_H__
#define __MESHGLENV_H__

struct MeshGLEnv
{
	float xRot;
	float yRot;
	float xTrans;
	float yTrans;
	float zTrans;

	int ox;
	int oy;
	int buttonState;
	float xRotLength;
	float yRotLength;

	float lightPos[4];
	float specular[4];
	float specref[4];
	float ambientLight[4];
	float spotDir[4];
};

#endif
