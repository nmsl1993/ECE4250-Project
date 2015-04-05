/*
 * censusTransform.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: noah
 */



#include "Transforms.hpp"
using namespace cv;
//void censusTransform(Mat<Tp> in, Mat<T[]> out)

template <typename T>
void censusTransform(InputArray inA, Mat outA)
{

	Mat in = inA.getMat();
	Mat out = outA;

	for(int i = 1; i < in.rows-1;i++) //aka y
	{
		for(int j = 1; j < in.cols-1;j++) //aka x
		{
			int xp1 = j+1;
			int xm1 = j-1;

			int yp1 = i+1;
			int ym1 = i-1;

			//int xp1 = x+1 < in.size().width ? x+1 : x;
			//int xm1 = x-1 >= 0 ? x-1 : x;

			//int yp1 = y+1 < in.size().height ? y+1 : y;
			//int ym1 = y-1 >= 0 ? y-1 : y;
			T val = in.at<T>(i,j);
			uchar censusPixel = 0;
			censusPixel |= (in.at<T>(i-1,j-1) <= val) << 7;
			censusPixel |= (in.at<T>(i-1,j) <= val) << 6;
			censusPixel |= (in.at<T>(i-1,j+1) <= val) << 5;
			censusPixel |= (in.at<T>(i,j-1) <= val) << 4;
			censusPixel |= (in.at<T>(i,j+1) <= val) << 3;
			censusPixel |= (in.at<T>(i+1,j-1) <= val) << 2;
			censusPixel |= (in.at<T>(i+1,j) <= val) << 1;
			censusPixel |= (in.at<T>(i+1,j+1) <= val) << 0; // <<0 does nothing


			out.at<uchar>(i-1,j-1) = censusPixel;//censusTransformedPixel;

		}

	}

}
template void censusTransform<float>(InputArray inA, Mat outA); //Explicit instantiation

