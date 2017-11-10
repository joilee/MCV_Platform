#include "TriMesh.h"

double disP2P(Pot p,Pot q)
{
	double res = (p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y)+(p.z-q.z)*(p.z-q.z);
	return sqrt(res);
}



Pot get_Normal(Pot p1,Pot p2,Pot p3)  
{  
	double x1 = p2.x-p1.x;
	double y1 = p2.y-p1.y;
	double z1 = p2.z-p1.z;

	double x2 = p3.x-p2.x;
	double y2 = p3.y-p2.y;
	double z2 = p3.z-p2.z;

	double a = y1*z2 - y2*z1;
	double b = x2*z1 - x1*z2;
	double c = x1*y2 - x2*y1;

	double radix = sqrt(a*a+b*b+c*c);
	
	return Pot(a/radix,b/radix,c/radix);  

} 


/************************************************************************/
/* ���ݵ�ֲ��ľ���������д���

��Σ�row,col,i,j,���Ͻ�����xmin,ymax
���Σ�vector<Pot> adjPoints
*/
/************************************************************************/
void getAdjPoint(vector<Pot> &adjPoints,vector<vector<int> >&heights, int row,int col,double stdlen,int i,int j,double xmin,double ymax)
{
	double tmpX=xmin+j*stdlen;
	double tmpY=ymax-i*stdlen;
	//˳ʱ����ӵ�
	//��Ϊ9���������
	if(i==0&&j==0)//case1: ������Ͻǵ㲻����
	{
		Pot P1=Pot(tmpX+stdlen,tmpY,heights[i][j+1]);
		Pot P2=Pot(tmpX+stdlen,tmpY-stdlen,heights[i+1][j+1]);
		Pot P3=Pot(tmpX,tmpY-stdlen,heights[i+1][j]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
	}else if(i==0&&j<col-1&&j>0) //case2:
	{
		Pot P1=Pot(tmpX+stdlen,tmpY,heights[i][j+1]);
		Pot P2=Pot(tmpX+stdlen,tmpY-stdlen,heights[i+1][j+1]);
		Pot P3=Pot(tmpX,tmpY-stdlen,heights[i+1][j]);
		Pot P4=Pot(tmpX-stdlen,tmpY-stdlen,heights[i+1][j-1]);
		Pot P5=Pot(tmpX-stdlen,tmpY,heights[i][j-1]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
		adjPoints.push_back(P4);
		adjPoints.push_back(P5);

	}else if(i==0&&j==col-1)//case3
	{
		Pot P1=Pot(tmpX,tmpY-stdlen,heights[i+1][j]);
		Pot P2=Pot(tmpX-stdlen,tmpY-stdlen,heights[i+1][j-1]);
		Pot P3=Pot(tmpX-stdlen,tmpY,heights[i][j-1]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
	}else if(0<i&&i<row-1&&j==0)//case4
	{
		Pot P1=Pot(tmpX,tmpY+stdlen,heights[i-1][j]);
		Pot P2=Pot(tmpX+stdlen,tmpY+stdlen,heights[i-1][j+1]);
		Pot P3=Pot(tmpX+stdlen,tmpY,heights[i][j+1]);
		Pot P4=Pot(tmpX+stdlen,tmpY-stdlen,heights[i+1][j+1]);
		Pot P5=Pot(tmpX,tmpY-stdlen,heights[i+1][j]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
		adjPoints.push_back(P4);
		adjPoints.push_back(P5);
	}else if (0<i&&i<row-1&&0<j&&j<col-1)//case5
	{ //�Ӹõ������һ�������
		Pot P1=Pot(tmpX,tmpY+stdlen,heights[i-1][j]);
		Pot P2=Pot(tmpX+stdlen,tmpY+stdlen,heights[i-1][j+1]);
		Pot P3=Pot(tmpX+stdlen,tmpY,heights[i][j+1]);
		Pot P4=Pot(tmpX+stdlen,tmpY-stdlen,heights[i+1][j+1]);
		Pot P5=Pot(tmpX,tmpY-stdlen,heights[i+1][j]);
		Pot P6=Pot(tmpX-stdlen,tmpY-stdlen,heights[i+1][j-1]);
	    Pot P7=Pot(tmpX-stdlen,tmpY,heights[i][j-1]);
		Pot P8=Pot(tmpX-stdlen,tmpY+stdlen,heights[i-1][j-1]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
		adjPoints.push_back(P4);
		adjPoints.push_back(P5);
		adjPoints.push_back(P6);
		adjPoints.push_back(P7);
		adjPoints.push_back(P8);
	}else if (0<i&&i<row-1&&j==col-1)//case6
	{
		Pot P1=Pot(tmpX,tmpY-stdlen,heights[i+1][j]);
		Pot P2=Pot(tmpX-stdlen,tmpY-stdlen,heights[i+1][j-1]);
		Pot P3=Pot(tmpX-stdlen,tmpY,heights[i][j-1]);
		Pot P4=Pot(tmpX-stdlen,tmpY+stdlen,heights[i-1][j-1]);
		Pot P5=Pot(tmpX,tmpY+stdlen,heights[i-1][j]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
		adjPoints.push_back(P4);
		adjPoints.push_back(P5);
	}else if(j==0&&i==row-1)//case7
	{
		Pot P1=Pot(tmpX,tmpY+stdlen,heights[i-1][j]);
		Pot P2=Pot(tmpX+stdlen,tmpY+stdlen,heights[i-1][j+1]);
		Pot P3=Pot(tmpX+stdlen,tmpY,heights[i][j+1]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
	}else if (i==row-1&&0<j&&j<col-1)//case8
	{
		Pot P1=Pot(tmpX-stdlen,tmpY,heights[i][j-1]);
		Pot P2=Pot(tmpX-stdlen,tmpY+stdlen,heights[i-1][j-1]);
		Pot P3=Pot(tmpX,tmpY+stdlen,heights[i-1][j]);
		Pot P4=Pot(tmpX+stdlen,tmpY+stdlen,heights[i-1][j+1]);
		Pot P5=Pot(tmpX+stdlen,tmpY,heights[i][j+1]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
		adjPoints.push_back(P4);
		adjPoints.push_back(P5);
	}else if(i==row-1&&j==col-1)//case9
	{
		Pot P1=Pot(tmpX-stdlen,tmpY,heights[i][j-1]);
		Pot P2=Pot(tmpX-stdlen,tmpY+stdlen,heights[i-1][j-1]);
		Pot P3=Pot(tmpX,tmpY+stdlen,heights[i-1][j]);
		adjPoints.push_back(P1);
		adjPoints.push_back(P2);
		adjPoints.push_back(P3);
	}
	
}

/************************************************************************/
/* �õ�һ����ķ�����   
   ��� ԭʼ��  ��Χ���vector
   ���ز��� Pot��ʽ�ķ�����
*/
/************************************************************************/
Pot GetNormalPoint(Pot src,vector<Pot> adjPoint )
{
	
	int adjSize=adjPoint.size();
	Pot tmpNormal=Pot(0,0,0);
	for(int k=0;k<adjSize-1;k++)
	{
		double per=disP2P(src,adjPoint[k+1])/(disP2P(src,adjPoint[k])+disP2P(src,adjPoint[k+1])+disP2P(adjPoint[k],adjPoint[k+1]));
		Pot Nk=get_Normal(adjPoint[k],src,adjPoint[k+1]);
		tmpNormal.x+=Nk.x*per;
		tmpNormal.y+=Nk.y*per;
		tmpNormal.z+=Nk.z*per;
	}

	if (adjSize==8)
	{
		double per=disP2P(src,adjPoint[0])/(disP2P(src,adjPoint[7])+disP2P(src,adjPoint[0])+disP2P(adjPoint[7],adjPoint[0]));
		Pot Nk=get_Normal(adjPoint[7],src,adjPoint[0]);
		tmpNormal.x+=Nk.x*per;
		tmpNormal.y+=Nk.y*per;
		tmpNormal.z+=Nk.z*per;
	}

	return tmpNormal;


}



/************************************************************************/
/* ��ȡդ����ÿ����ķ�������ˮƽ���� ,����Ź��񹹳�8��������ƽ���ķ������ĦȽ�
   ���: heights,row col,stdlen,�Լ����ϽǶ������꣬���¼�Сy����������x
   ����: normals(�Ƚǵ���ʽ)

   ��ע����ͼһ����Ҫ����3*3�ģ���Ȼ������⡣
   added by lg
*/
/************************************************************************/


void localGetNormalMatrix (vector<vector<double> >& normals,vector<vector<int> > &heights,int startRow,int endRow,int startCol,int endCol,double stdlen,double xmin,double ymax,int row,int col)
{


	for(int i=startRow;i<=endRow;i++)
	{
		for(int j=startCol;j<=endCol;j++)
		{

			double tmpX=xmin+j*stdlen;
			double tmpY=ymax-i*stdlen;
			double tmpZ=heights[i][j];

			Pot source=Pot(tmpX,tmpY,tmpZ);//�õ����ά����
			vector<Pot> adjPoints;
			getAdjPoint(adjPoints,heights, row,col,stdlen, i, j,xmin, ymax);//����ڽӵ�
			Pot normalPoint= GetNormalPoint(source, adjPoints );//��øõ�ķ�����
			double theta=acos(normalPoint.z/normalPoint.getLength());//���thetaֵ
			normals[i-startRow][j-startCol]=theta;
		}

	
	}

}


/************************************************************************/
/* �߶�դ������ �� cannyͼ��                                                                     */
/************************************************************************/
void height2canny(GridHeight &heightR,vector<vector<int> > &cannyPoint,int rowNum,int colNum,double stdLen,double xmin,double ymax,int area[4])
{
	vector<vector<double> >normalMatrix;
	int rowCount=area[1]-area[0]+1;
	int colCount =area[3]-area[2]+1;
	normalMatrix.resize(rowCount,vector<double>(colCount));
	localGetNormalMatrix(normalMatrix,heightR.p,area[0],area[1],area[2],area[3],stdLen,xmin, ymax,rowNum,colNum);
	//������canny�㷨����
	cImage * tmpTest=new cImage(rowCount,colCount);
	tmpTest->canny(normalMatrix,cannyPoint,rowCount,colCount);

}






/************************************************************************/
/* ��������� ����3      Ϊ�ֲ�׼��                                                             */
/************************************************************************/
int getFeaturePoint3(int steps,int localRow,int localCol,vector<vector<int> >& cannyPoint)
{
	int totalVer=0;//ͳ�Ƹ����������������
	if(localRow<10||localCol<10)
	{
		steps=2;
	}
	int stepRow=localRow/steps;
	int stepCol=localCol/steps;

	for(int n=0;n<localCol;n++)
	{
		for(int m=localRow-1;m>=0;m--)
		{
			if(cannyPoint[m][n]!=0)
			{
				totalVer++;
			}
		}
	}

	//�ټ�����ϵĵ� �Ҳ������������غ�
	for(int i=0;i<=steps;i++)
	{
		for(int j=0;j<=steps;j++)
		{
			int m=i*stepRow;
			int n=j*stepCol;
			if(m>=localRow)m=localRow-1;
			if(n>=localCol)n=localCol-1;
			if(cannyPoint[m][n]==0)
			{
				totalVer++;
			}
		}
	}
	return totalVer;
}





/************************************************************************/
/*���㱣����MESH_PTR �ṹ��  ����3 �����ڲ����                        */
/************************************************************************/
int setMeshPtr3(MESH_PTR pMesh,int steps,int localRow,int localCol,vector<vector<int> >& cannyPoint,GridHeight &heightR,double xmin,double ymax,double stdLen,int startRow,int startCol)
{
	int amount=3;
	if(localRow<10||localCol<10)
	{
		steps=2;
	}
	int stepRow=localRow/steps;
	int stepCol=localCol /steps;

	//ѡ��������
	for(int n=0;n<localCol;n++)
	{
		for(int m=localRow-1;m>=0;m--)
		{
			if(cannyPoint[m][n]!=0)
			{
				double x=xmin+(n+startCol)*stdLen;
				double y=ymax-(m+startRow)*stdLen;
				double z=heightR.p[m+startRow][n+startCol];
				((VERTEX2D_PTR)(pMesh->pVerArr+amount))->x = x;
				((VERTEX2D_PTR)(pMesh->pVerArr+amount))->y = y;
				((VERTEX2D_PTR)(pMesh->pVerArr+amount))->z = z;
				amount++;
			}
		}
	}
	//�ټ�����ϵĵ� �Ҳ������������غ�
	for(int i=0;i<=steps;i++)
	{
		for(int j=0;j<=steps;j++)
		{
			//
			int rowIndex=i*stepRow;
			int colIndex=j*stepCol;

			if(rowIndex>=localRow)rowIndex=localRow-1;
			if(colIndex>=localCol)colIndex=localCol-1;
			int m=startRow+rowIndex;
			int n=startCol+colIndex;


			if(cannyPoint[rowIndex][colIndex]==0)
			{
				double x=xmin+n*stdLen;
				double y=ymax-m*stdLen;
				double z=heightR.p[m][n];
				((VERTEX2D_PTR)(pMesh->pVerArr+amount))->x = x;
				((VERTEX2D_PTR)(pMesh->pVerArr+amount))->y = y;
				((VERTEX2D_PTR)(pMesh->pVerArr+amount))->z = z;
				amount++;
			}
		}
	}
	return amount;

}
