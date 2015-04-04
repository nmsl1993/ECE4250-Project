/*
 * censusTransform.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: noah
 */



#include "Transforms.h"
using namespace cv;
//void censusTransform(Mat<Tp> in, Mat<T[]> out)
/*
template <typename T>
void censusTransform(InputArray inA, OutputArray outA)
{

	Mat in = inA.getMat();
	Mat out = outA;//.getMat();

	for(int x = 0; x < in.size().width;x++)
	{
		for(int y = 0; y < in.size().height;y++)
		{
			int xp1 = x+1 < in.size().width ? x+1 : x;
			int xm1 = x-1 >= 0 ? x-1 : x;

			int yp1 = y+1 < in.size().height ? y+1 : y;
			int ym1 = y-1 >= 0 ? y-1 : y;

			T val = in.at<T>(xm1,ym1);
			if(
					(in.at<T>(xm1,ym1) <= val) &&
					(in.at<T>(x,ym1) <= val) &&
					(in.at<T>(xp1,ym1) <= val) &&
					(in.at<T>(xm1,y) <= val) &&
					(in.at<T>(xp1,y) <= val) &&
					(in.at<T>(xm1,yp1) <= val) &&
					(in.at<T>(x,yp1) <= val) &&
					(in.at<T>(xp1,yp1) <= val)
				)

			{
				//std::cout << "WARN" << std::endl;
				out.at<uchar>(x,y) = 1;//censusTransformedPixel;
			}
			else
			{
				//std::cout << "NOWARN" << std::endl;

				out.at<uchar>(x,y) = 0;//censusTransformedPixel;

			}

		}

	}

}
*/
