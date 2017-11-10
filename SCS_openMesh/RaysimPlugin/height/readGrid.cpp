#include "readGrid.h"

/************************************************************************/
/* 得到一个点对应的高度                                                                     */
/************************************************************************/
double getPointAltitude(GridHeight &gridR,double x,double y,int row,int col,double base,double xmin,double ymax)
{
	double xmax=xmin+(col)*base;
	double ymin=ymax-(row)*base;
	if(x<xmin||y>ymax||x>xmax||y<ymin)
	{
		cout<<"wrong coordinates from building vector"<<endl;
	}
	int pointRow=int((ymax-y)/base);
	int pointCol=int((x-xmin)/base);
#ifdef DEBUG_MODE
	if(gridR.p[pointRow][pointCol]<0)
	{
		cout<<gridR.p[pointRow][pointCol]<<endl;
	}
#endif

	return gridR.p[pointRow][pointCol];

	//return 0;
}



/*****************************************************************************************
************************读取高度栅格数据****************************************************
***************************modified BY LG**************************************************************/

/* 根据路径，读取所有的文件*/
void readHeightGrid(string index,vector<GridHeight> &gridR,vector<int>&rowNumber,vector<int> &colNumber,vector<double>&bases,vector<double> &xmins,vector<double>&ymaxs )
{

	gridR.clear();
	rowNumber.clear();
	colNumber.clear();
	bases.clear();
	xmins.clear();
	ymaxs.clear();

	string indn = index;

	ifstream Findex;
	int addrOfIndn=0;
	for (int i=indn.size()-1;i>=0;i--)
	{
		if (indn[i]=='\\'||indn[i]=='/')
		{
			addrOfIndn =i;
			break;
		}
	}

	string headFile = indn.substr(0,addrOfIndn+1);

	Findex.open(indn);

	vector<string> Hnames;
	vector<double> xmaxs,ymins;
	while(Findex.good() && !Findex.eof())
	{
		string hn="";
		double xmin,xmax,ymin,ymax,base;
		Findex>>hn;
		if (hn!="")
		{
			Findex>>xmin>>xmax>>ymin>>ymax>>base;
			//cout<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<base<<endl;
			Hnames.push_back(hn);
			xmins.push_back(xmin);
			xmaxs.push_back(xmax);
			ymins.push_back(ymin);
			ymaxs.push_back(ymax);
			bases.push_back(base);
		}

	}
	//	cout<<ymaxs[0]<<endl;

	/************************************************************************/
	/*       针对每个文件单独处理，获取高度值和行数列数                         */
	/************************************************************************/

	for (int hns=0;hns<Hnames.size();hns++)
	{
		ifstream FileH;
		string tt = headFile +Hnames[hns];
		//cout<<tt<<endl;

		FileH.open(tt,ios::binary);

		//获取行列值
		int row = 0;
		row = int((ymaxs[hns]-ymins[hns])/bases[hns]);

		int col = 0;
		col = int((xmaxs[hns]-xmins[hns])/bases[hns]);
	


		//int **height = new_Array2D<int>(row,col);
		vector<vector<int> >height ;
		height.resize(row,vector<int>(col));
		for (int i=0;i<row;i++)
		{
			for (int j=0;j<col;j++)
			{
				height[i][j]=0;
			}
		}
		GridHeight gt(height,row,col);

		//存储三个值
		gridR.push_back(gt);
		rowNumber.push_back(row);
		colNumber.push_back(col);

		unsigned char c1,c2;
		char c3[2];
		const int Nc=80;
		//char cstr[Nc];
		// 	
		//int st;
		int no=0,flag=1;

		int num=0;
		int colt = gridR[hns].col;

		while(FileH.good()&&!FileH.eof())
		{
			FileH.read(c3,2);
			c1= c3[0];
			c2= c3[1];	
			
			int tmp =  int(c1)*256 +int(c2);
			if(tmp>32767)tmp=0;
#ifdef DEBUG_MODE
			if(tmp<0)
			{
				cout<<"its right"<<" "<<tmp<<endl;
			}
#endif
			if(no>=row*col)
			{
			
			}else
			{
				gridR[hns].p[no/colt][no%colt] = tmp;
				no++;
			}
		
		}
		FileH.close();
	}

}





int cmp( PotIntxy a, PotIntxy b)
{
	if (a.x<b.x)
	{
		return 1;
	}
	else if (a.x>b.x)
	{
		return 0;
	}
	else
	{
		return a.y<b.y;

	}
}