#include "TriSimplify.h"
#define  stepLength 5

bool isLine(double x[4],double y[4],int id1,int id2,int id3)
{
//#ifdef DEBUG_MODE 
//	cout<<"x "<<x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<x[3]<<endl;
//	cout<<"y "<<y[0]<<" "<<y[1]<<" "<<y[2]<<" "<<y[3]<<endl;
//	cout<<id1<<" "<<id2<<" "<<id3<<endl;
//#endif // DEBUG_MODE 

	double v1[2],v2[2];
	v1[0]=x[id2]-x[id1];
	v1[1]=y[id2]-y[id1];
	v2[0]=x[id3]-x[id1];
	v2[1]=y[id3]-y[id1];
//#ifdef DEBUG_MODE
//    cout<<v1[0]<<" "<<v1[1]<<" "<<v2[0]<<" "<<v2[1]<<endl; 
//#endif // DEBUG_MODE

	if(v2[0]*v1[1]==v2[1]*v1[0])
	{
		return true;
	}else
	{
		return false;
	}
}

/************************************************************************/
/* 简化三角形 将共线的三角形合并                                        */
/************************************************************************/
void  simplifyTri(MESH_PTR pMesh)
{
	/************************************************************************/
	/* 简化算法准备工作                                                      */
	/************************************************************************/
	double standardArea=(stepLength*stepLength)/2;
	TRIANGLE_PTR pTri = pMesh->pTriArr;
	int* pi;

	//step0:设置初始变量
	map<pair<int,int>,SimEdge*> edgeCollect;
	vector<SimTri *> originTri;
	vector<SimEdge*> originEdge;
	int oringTriCount=0,originEdgeCount=0;
	

	TRIANGLE_PTR lastTri= pMesh->pTriArr;
	while(pTri != NULL)	
	{
		pTri->changeFlag=false;
		int id[3];
		double x[3],y[3],z[3];
		pi = &(pTri->i1);
		for (int j=0; j<3; j++)	
		{	
			id[j] = *pi++;	
			//id[j]=id[j]-2;
			x[j]=((VERTEX2D_PTR)(pMesh->pVerArr+id[j]))->x;
			y[j]=((VERTEX2D_PTR)(pMesh->pVerArr+id[j]))->y;
			z[j]=((VERTEX2D_PTR)(pMesh->pVerArr+id[j]))->z;
		}

		//step1:对水平且面积不等于正方形网格一半的三角形，检测并处理 true转step2 false转step4
		//判断是否水平
		double x1=x[1]-x[0];
		double x2=x[2]-x[0];
		double y1=y[1]-y[0];
		double y2=y[2]-y[0];
		double tmpArea=fabs( x1*y2 - y1*x2);
		if(z[0]==z[1]&&z[0]==z[2]&&(tmpArea<standardArea))
		{
		//step2:遍历三个边，每个边都要加入边表
			SimTri *tmpTri=new SimTri(id[0],id[1],id[2]);
			originTri.push_back(tmpTri);
			oringTriCount++;
			 for(int k=0;k<3;k++)
			 {
				 int smallId,bigId;
				 if(id[k]>id[(k+1)%3])//01 12 20
				 {
					 smallId=id[(k+1)%3];bigId=id[k];
				 }else
				 {
					 smallId=id[k];bigId=id[(k+1)%3];
				 }

				 map<pair<int,int>,SimEdge*>::iterator it=edgeCollect.find(pair<int,int>(smallId,bigId));
				 if(it!=edgeCollect.end())
				 {
					//step 2.1 该边已经存在 需要检测是否有三角形存在：存在说明有问题,报错；不存在说明可以加入vector
					 bool sameFlag=false;
					 for(int m=0;m<it->second->triPtr.size();m++)
					 {
						 if(tmpTri->isEqual(it->second->triPtr[m]))
						 {
							 sameFlag=true;
							 break;
						 }
					 }
					 if(!sameFlag)
					 {
						it->second->triPtr.push_back(tmpTri);
						tmpTri->e[k]=it->second;
					 }else
					 {
						#ifdef DEBUG_MODE 
						cout<<"Error In Simplify Triangle :repeat to add triangle in  vector<SimTri *> triPtr;"<<endl;
						#endif
					 }


				 }else
				 {
					 //step 2.2 该边不存在，需要加入该边
					 pair<int,int> p1=make_pair(smallId,bigId);
					 SimEdge *tmpEdge=new SimEdge(smallId,bigId);

					 //在三角形和边中 互相设置指针
					 tmpEdge->triPtr.push_back(tmpTri);
					 tmpTri->e[k]=tmpEdge;

					 edgeCollect.insert(make_pair(p1,tmpEdge));
					 originEdge.push_back(tmpEdge);
					 originEdgeCount++;
				 }
			 }  
			pTri->changeFlag=true;
		}else
		{
			//step4: 三角形无需简化，可以直接输出
			pTri->changeFlag=false;
		}
		lastTri=pTri;
		pTri = pTri->pNext;
		
	}//三角形遍历完毕，边 以及 待处理三角形都已经存储好

	/************************************************************************/
	/* 简化算法正式开始                                                      */
	/************************************************************************/
	queue<SimEdge*> testQueue;
	for(int k=0;k<originEdge.size();k++)
	{
		#ifdef DEBUG_MODE 
		if(originEdge[k]->triPtr.size()==0)
		{
			cout<<"something wrong hahaha"<<endl;
		}
		#endif // DEBUG_MODE 
		testQueue.push(originEdge[k]);
		originEdge[k]->inQueue=true;
	}
	#ifdef DEBUG_MODE 
		cout<<"Queue size"<<testQueue.size()<<endl;
	#endif // DEBUG_MODE 


	while(!testQueue.empty())
	{
		SimEdge * tmpEdge=testQueue.front();
		testQueue.pop();
		if(tmpEdge->inQueue==true)
		{
			//检测该边相邻的三角形是否共线
			if(tmpEdge->triPtr.size()==2)
			{
				int p[4];//p[0] p[1]为公共边
				p[0]=tmpEdge->id1;
				p[1]=tmpEdge->id2;
				p[2]=tmpEdge->triPtr[0]->getAnotherID(p[0],p[1]);
				p[3]=tmpEdge->triPtr[1]->getAnotherID(p[0],p[1]);
				double xx[4],yy[4];
				for(int n=0;n<4;n++)
				{
					xx[n]=((VERTEX2D_PTR)(pMesh->pVerArr+p[n]))->x;
					yy[n]=((VERTEX2D_PTR)(pMesh->pVerArr+p[n]))->y;
				}

				//判断是否共线
				bool lineFlag[2];
				lineFlag[0]=isLine(xx,yy,2,0,3);
				lineFlag[1]=isLine(xx,yy,2,1,3);

				if (lineFlag[0]||lineFlag[1])//如果共线
				{
					#ifdef DEBUG_MODE 
					cout<<"lineFlag[0] "<<lineFlag[0]<<" lineFlag[1] "<<lineFlag[1]<<endl;
					#endif // DEBUG_MODE 

					for(int s=0;s<2;s++)//对其中共线的进行处理
					{
						if(lineFlag[s]==false)
							continue;

						//  设置边的isDeleted为true
						//  设置inQueue为false
						//	设置该边指向的三角形指针都为空
						//	从map中删除该边 
						tmpEdge->isDeleted=true;
						tmpEdge->inQueue=false;
						for(int ii=0;ii<2;ii++)
						{
							tmpEdge->triPtr[ii]->isDeleted=true;
						}
						tmpEdge->triPtr.clear();
					
						int IDMax=p[0]>p[1]?p[0]:p[1];
						int IDMin=p[0]<p[1]?p[0]:p[1];
						map<pair<int,int>,SimEdge*>::iterator itMAP=edgeCollect.find(pair<int,int>(IDMin,IDMax));
						if(itMAP!=edgeCollect.end())
						{
							edgeCollect.erase(itMAP);
						}


						int newID=abs(1-s);
						SimTri * newTri= new SimTri(p[2],p[newID],p[3]);

						//对新生成边
						 IDMax=p[2]>p[3]?p[2]:p[3];
						 IDMin=p[2]<p[3]?p[2]:p[3];
						map<pair<int,int>,SimEdge*>::iterator itNew=edgeCollect.find(pair<int,int>(IDMin,IDMax));
						if(itNew==edgeCollect.end())
						{
							SimEdge * newEdge=new SimEdge(IDMin,IDMax);
							newTri->e[2]=newEdge;
							newEdge->triPtr.push_back(newTri);
							pair<int,int> pNew=make_pair(IDMin,IDMax);
							edgeCollect.insert(make_pair(pNew,newEdge));
						}else
						{
							newTri->e[2]=itNew->second;
							itNew->second->triPtr.push_back(newTri);
						}

						//	新建三角形，
						//	1边和2边的三角形指针需要改变，
						//	新3边指向新三角形，
						//	三角形也要指向这三条边
						
						IDMax=p[2]>p[newID]?p[2]:p[newID];
						IDMin=p[2]<p[newID]?p[2]:p[newID];
						map<pair<int,int>,SimEdge*>::iterator itEdge1=edgeCollect.find(pair<int,int>(IDMin,IDMax));
						newTri->e[0]=itEdge1->second;
						for(int mm=0;mm<itEdge1->second->triPtr.size();mm++)
						{
							int b[3]={p[0],p[1],p[2]};
						    if (itEdge1->second->triPtr[mm]->isEqual2(b))
						    {
								itEdge1->second->triPtr[mm]=newTri;
						    }
						    	
						}


						IDMax=p[3]>p[newID]?p[3]:p[newID];
						IDMin=p[3]<p[newID]?p[3]:p[newID];
						map<pair<int,int>,SimEdge*>::iterator itEdge2=edgeCollect.find(pair<int,int>(IDMin,IDMax));
						newTri->e[1]=itEdge2->second;
						for(int mm=0;mm<itEdge2->second->triPtr.size();mm++)
						{
							int b[3]={p[0],p[1],p[3]};
							if (itEdge2->second->triPtr[mm]->isEqual2(b))
							{
								itEdge2->second->triPtr[mm]=newTri;
							}
						}

						/*新三角形的isDeleted为false
							新三角形放入vector
							新三条边都要入队列，inQueue为true*/
						newTri->isDeleted=false;
						originTri.push_back(newTri);
						for (int tmpCount=0;tmpCount<3;tmpCount++)
						{
							newTri->e[tmpCount]->inQueue=true;
							testQueue.push(newTri->e[tmpCount]);
						}

						//删除4 、5 指向原来两个三角形的指针，不改变入队列的bool值
						IDMax=p[2]>p[s]?p[2]:p[s];
						IDMin=p[2]<p[s]?p[2]:p[s];
						map<pair<int,int>,SimEdge*>::iterator itEdge4=edgeCollect.find(pair<int,int>(IDMin,IDMax));
						if(itEdge4!=edgeCollect.end())
						{

							for(int mm=0;mm<itEdge4->second->triPtr.size();mm++)
							{
								int b[3]={p[0],p[1],p[2]};
								if (itEdge4->second->triPtr[mm]->isEqual2(b))
								{
									itEdge4->second->triPtr.erase(itEdge4->second->triPtr.begin()+mm);
								}
							}
							if(itEdge4->second->triPtr.size()==0)
							{
								#ifdef DEBUG_MODE 
								cout<<"question is here"<<itEdge4->second->id1<<" "<<itEdge4->second->id2<<endl;
								#endif // DEBUG_MODE 
								itEdge4->second->inQueue=false;
								itEdge4->second->isDeleted=false;
								edgeCollect.erase(itEdge4);
							}
						}


						IDMax=p[3]>p[s]?p[3]:p[s];
						IDMin=p[3]<p[s]?p[3]:p[s];
						map<pair<int,int>,SimEdge*>::iterator itEdge5=edgeCollect.find(pair<int,int>(IDMin,IDMax));
						if(itEdge5!=edgeCollect.end())
						{
							for(int mm=0;mm<itEdge5->second->triPtr.size();mm++)
							{
								int b[3]={p[0],p[1],p[3]};
								if (itEdge5->second->triPtr[mm]->isEqual2(b))
								{
									itEdge5->second->triPtr.erase(itEdge5->second->triPtr.begin()+mm);
								}
							}

							if(itEdge5->second->triPtr.size()==0)
							{
								#ifdef DEBUG_MODE 
								cout<<"question is here"<<itEdge5->second->id1<<" "<<itEdge5->second->id2<<endl;
								#endif // DEBUG_MODE 
								itEdge5->second->inQueue=false;
								itEdge5->second->isDeleted=false;
								edgeCollect.erase(itEdge5);
							}
						}
					}
				}else
				{
					tmpEdge->inQueue=false;
				}

			}else if(tmpEdge->triPtr.size()==1)
			{
				tmpEdge->inQueue=false;
			}else
			{
				tmpEdge->inQueue=false;
				#ifdef DEBUG_MODE 
				cout<<"Error : vector<SimTri *> size is "<<tmpEdge->triPtr.size()<<" Queue size "<<testQueue.size()<<endl;
				//cout<<tmpEdge->smg<<endl;
				#endif			
			}



		}
	}

	for(int i=0;i<originTri.size();i++)
	{
		if(originTri[i]->isDeleted==false)
		{
			lastTri=AddTriangleNode2(pMesh,lastTri,originTri[i]->id1,originTri[i]->id2,originTri[i]->id3);
		}

	}
	
	#ifdef DEBUG_MODE 
	cout<<"end of triSim"<<endl;
	#endif


}