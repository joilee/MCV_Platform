#include "canny.h"

//*****************************
//helper function that says whether arg is between a-b or c-d
//*****************************
bool isBetween(float arg, float a, float b, float c, float d) {
  if((arg >= a && arg <= b) || (arg >= c && arg <= d)) {
    return true;
  } else {
    return false;
  }
}


//***************************
// helper function that returns true if a>b and c
//***************************
bool isFirstMax(int a, int b, int c){
  if(a>b && a>c){
       return true;
  }
  return 0;
}


//****************************
// buckets the thetas into 0, 45, 90, 135
//****************************

int getOrientation(float angle) {
  if(isBetween(angle, -22.5, 22.5, -180, -157.5 ) || isBetween(angle, 157.5, 180, -22.5, 0))
    return 0;
  if(isBetween(angle, 22.5, 67.5, -157.5, -112.5))
    return 45;
  if(isBetween(angle, 67.5, 112.5, -112.5, -67.5))
    return 90;
  if(isBetween(angle, 112.5, 157.5, -67.5, -22.5))
    return 135; 
    
  return -1;

}

//***************************
// convolve is a general helper funciton that applies a convolution
// to the image and then returns the weighted sum so that
// it can replace whatever pixel we were just analyzing
//卷积运算,可以看作是一串内积运算,
//卷积运算，其实就是可看作是加权求和的过程，使用到的图像区域中的每个像素分别与卷积核(权矩阵)的每个元素对应相乘，
//所有乘积之和作为区域中心像素的新值。
//**************************
float cImage ::convolve(int con[][MAX_SIZE], int dim,  float divisor, int i, int j) {
    int midx = dim/2;
    int midy = dim/2;

    float weightedSum = 0;
    for(int x = i-midx; x < i + dim-midx; x++) {
      for(int y = j-midy; y < j + dim-midy; y++) {
		weightedSum += divisor*(double)(con[x-i+midx][y-j+midy]*imageArray[x][y]);
      }
    }
    return weightedSum;
}

/************************************************************************/
/* canny算法                                                                                            */
/************************************************************************/
	void cImage::canny(vector<vector<double> > &normalMatrix,vector<vector<int> > &cannyPoint,int row,int col)
	{
		normal2matrix(normalMatrix,row,col);
		cannyCore();
		cannyOutput(cannyPoint,row,col);
	}

/************************************************************************/
/* 输出结果                                                                     */
/************************************************************************/
void cImage::cannyOutput(vector<vector<int> > &cannyPoint,int row,int col)
{
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			cannyPoint[i][j]=0;
		}
	}
	int  count=0;
	for(int i=1;i<row-1;i++)
	{
		for(int j=1;j<col-1;j++)
		{
			if(imageArray[j][i]==0)
			{
				cannyPoint[i][j]=255;
				count++;
			}


		}
	}
	cout<<"特征点"<<count<<" 个"<<endl;
}

/************************************************************************/
/* canny算法     核心部分                                                                                          */
/************************************************************************/
void cImage::cannyCore()
{
	gaussianBlur();
	sobel();
	noMax();
	hysteresis();
}
/************************************************************************/
// gaussian blur
// applies a gaussian blur via a convolution of a gaussian
// matrix with sigma = 1.4. hard-coded in.
// future development of can generate the gauss matrix on the fly
/************************************************************************/
void  cImage::gaussianBlur()
{
	//define gauss matrix
	int gaussArray[5][5] = {  {2, 4, 5, 4, 2},
	{4, 9, 12,9, 4},
	{5, 12, 15, 12, 5},
	{4, 9, 12,9, 4},
	{2, 4, 5, 4, 2} };

	float gaussDivisor = 1.0/159.0;
	float sum = 0.0;
	int dim = 5;

	for(int i=2; i < COLUMNS-2; i++) {
		for(int j=2; j < ROWS-2; j++) {
			sum = convolve(gaussArray,dim, gaussDivisor, i, j);
			imageArray[i][j] = (int)sum;
		}
	}
}


//****************************
// Applies a sobel filter to find the gradient direction
// and magnitude. those values are then stored in thetas and magArray
// so that info can be used later for further analysis
//****************************
void cImage::sobel() {
  float sum = 0.0;
  float G_x, G_y, G, theta;
  int sobel_y[5][5] = {  {-1, 0, 1,0,0},
			 {-2, 0, 2,0,0},
			 {-1, 0, 1,0,0}, 
			 {0, 0, 0, 0, 0},
			 {0, 0, 0, 0, 0}};
			  
  int sobel_x[5][5] = {  {1, 2, 1, 0, 0},
			 {0, 0, 0, 0, 0},
			 {-1, -2, -1, 0, 0},
			 {0, 0, 0, 0, 0},
			 {0, 0, 0, 0, 0} };
			
  int dim = 3;
  //columns and rows might be mixed up here
  for (int i = 1; i < COLUMNS-1; i++ ) {
    for (int j = 1; j < ROWS-1; j++ ) {
      G_x = convolve(sobel_x, dim, 1, i, j);
      G_y = convolve(sobel_y, dim, 1, i, j);
      G = sqrt(G_x*G_x + G_y*G_y);
     // cout << G << "\n";
      // cout << atan2(G_y, G_x) << "\n";
      thetas[i][j] = getOrientation(180.0*atan2(G_y, G_x)/PI);
      //	cout << thetas[i][j] << endl;
      magArray[i][j] = G;
    }
  }
}


//*****************************
//non-maximum suppression 
//depending on the orientation, pixels are either thrown away or accepted
//by checking it's neighbors
//*****************************
void cImage::noMax(){
	int theta = 0;
	for( int j=1 ; j < ROWS-1 ; j++)
	 {
	    for( int i=1 ; i < COLUMNS-1 ; i++)
	     {
			theta = (int) thetas[i][j];
			switch(theta){
				case 0:
					if(isFirstMax(magArray[i][j],magArray[i+1][j],magArray[i-1][j])){
						imageArray[i][j] =0; // black
					}
					else{
						imageArray[i][j] = 255; // white
					}
				break;
				
				case 45:
					if(isFirstMax(magArray[i][j],magArray[i+1][j+1],magArray[i-1][j-1])){
						imageArray[i][j] =0; // black
					}
					else{
						imageArray[i][j] = 255; // white
					}
				
				break;
				
				case 90:
					if(isFirstMax(magArray[i][j],magArray[i][j+1],magArray[i][j-1])){
						imageArray[i][j] = 0; // black
					}
					else{
						imageArray[i][j] = 255; // white
					}
				break;
				
				case 135:
					if(isFirstMax(magArray[i][j],magArray[i+1][j-1],magArray[i-1][j+1])){
						imageArray[i][j] = 0; // black
					}
					else{
						imageArray[i][j] = 255; // white
					}
				break;
				
				default:
				  	cout << "error in nomax()"<< endl;
				break;
			}
		}
	}
}


//*******************************
//hysteresis noise filter makes lines continuous and filters out the noise
// see the pdf that we used to understand this step in english (Step 5)
//*******************************
  void cImage::hysteresis(){
    bool greaterFound;
    bool betweenFound;
        for( int j=2 ; j < ROWS-2 ; j++)
		{
			for( int i=2 ; i < COLUMNS-2 ; i++)
			{
				 if(magArray[i][j] < T_LOW){
					imageArray[i][j] = 255; // white
				 }        
			
				 if(magArray[i][j] > T_HIGH){
				   imageArray[i][j] = 0; // black
				  }
		
	       /*If pixel (x, y) has gradient magnitude between tlow and thigh and 
		 any of its neighbors in a 3 ?3 region around
		 it have gradient magnitudes greater than thigh, keep the edge*/
	       
				if(magArray[i][j] >= T_LOW && magArray[i][j] <= T_HIGH)
				{
					 greaterFound = false;
					 betweenFound = false;
					 for(int m = -1; m < 2; m++) 
					 {
								 for(int n = -1; n < 2; n++)
								 {
										if(magArray[i+m][j+n] > T_HIGH) 
										{ 
											  imageArray[i][j] = 0;
											 greaterFound = true;
										 }	     
										 if(magArray[i][j] > T_LOW && magArray[i][j] < T_HIGH) betweenFound = true;
								}
		             }
		 
					if(!greaterFound && betweenFound) 
					{
							for(int m = -2; m < 3; m++) 
							{
									for(int n = -2; n < 3; n++) 
									{
										if(magArray[i+m][j+n] > T_HIGH) greaterFound = true;
									 }
							}
					}
		 
					if(greaterFound) imageArray[i][j] = 0;
					else imageArray[i][j] = 255;
		 
	       }
	       
	     }
	 }
  }	
			
			
/*If pixel (x, y) has gradient magnitude between tlow and thigh and any of its neighbors in a 3 ?3 region around
it have gradient magnitudes greater than thigh, keep the edge (write out white).
?If none of pixel (x, y)抯 neighbors have high gradient magnitudes but at least one falls between tlow and thigh,
search the 5 ?5 region to see if any of these pixels have a magnitude greater than thigh. If so, keep the edge
(write out white).
*/


  /************************************************************************/
  /* 初始化imageArray  先列 后行                                                                   */
  /************************************************************************/
  void cImage::initImageArray(int row,int col)
  {
	  //allocate Memory
	  imageArray = new int *[col] ; // row memory allocation
	  for( int i = 0 ; i < col ; i++ ){ // column memory allocation
		  imageArray[i] = new int[row];
	  }

	  thetas = new float *[col];
	  for(int i = 0; i < col; i++) {
		  thetas[i] = new float[row];
	  }

	  magArray = new int *[col];
	  for(int i=0; i <col; i++) {
		  magArray[i] = new int[row];
	  }
  }

void cImage::matrixToGrey(vector<vector<double> > &src,vector<vector<int> >&grey,int row,int col)
  {	
	  //grey.resize(row,vector<int>(col));
	  for(int i=0;i<row;i++)
	  {
		  for(int j=0;j<col;j++)
		  {
			  double tmpScale=255*src[i][j]/M_PI_2;
			  int greyValue=floor(tmpScale);
			  if(greyValue<0)greyValue=0;
			  if(greyValue>255)greyValue=255;
			  grey[i][j]=greyValue;
		  }
	  }
		
  }



  /************************************************************************/
  /*    把向量变成灰度，最后变成int**                                                                  */
  /************************************************************************/
  void  cImage::normal2matrix(vector<vector<double> > &normalMatrix,int row,int col)
  {
	vector<vector<int> > grey;
	grey.resize(row,vector<int>(col));
	matrixToGrey(normalMatrix,grey,row,col);
	  initImageArray(row,col);
		for(int j=0;j<row;j++)
		{
			for(int i=0;i<col;i++)
			{
				 imageArray[i][j] =grey[j][i];
			}
		}
  }



/************************************************************************/
/*  thetaToGrey     (将theta （0~Pi/2）映射到灰度值(0~255)) 
*/
/************************************************************************/

void thetaToGrey(vector<vector<double> > &src,vector<vector<int> >&grey,int row,int col)
{	
	//grey.resize(row,vector<int>(col));
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
		{
			double tmpScale=255*src[i][j]/M_PI_2;
			int greyValue=floor(tmpScale);
			if(greyValue<0)greyValue=0;
			if(greyValue>255)greyValue=255;
			grey[i][j]=greyValue;
		}
		//return grey;
}


