//#include "array2D.h"
#include "readGrid.h"

#include "defaultPara.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
void thetaToGrey(vector<vector<double> > &src,vector<vector<int> >&grey,int row,int col);

#define MAX_SIZE 5
#define PI 3.1415927
#define T_LOW 39
#define T_HIGH 96

class  cImage
{
public:

	//
	int **imageArray;
	float  **thetas;
	int** magArray;

	unsigned int ROWS;
	unsigned int COLUMNS;
	char DEPTH;

	cImage(int _row,int _col)
	{
		ROWS=_row;
		COLUMNS=_col;
	}
	//Îö¹¹º¯Êý
	~cImage()
	{
		// free memory                                                                        
		for( int i = 0 ; i < ROWS ; i++ )
		{
			delete [] imageArray[i];                                                                                                 
			delete [] thetas[i];
			delete [] magArray[i];
		}
		delete [] imageArray; 
		delete [] thetas;
		delete [] magArray;
		return ;
	}
	void showImage(char * name);
	void cannyOutput(vector<vector<int> > &cannyPoint,int row,int col);
	void matrixToGrey(vector<vector<double> > &src,vector<vector<int> >&grey,int row,int col);
	void initImageArray(int row,int col);
	void normal2matrix(vector<vector<double> > &normalMatrix,int row,int col);
	void cannyCore();
	void canny(vector<vector<double> > &normalMatrix,vector<vector<int> > &cannyPoint,int row,int col);
	void gaussianBlur();
	void sobel();
	void noMax();
	void hysteresis();
	float convolve(int con[][MAX_SIZE], int dim,  float divisor, int i, int j);
};
