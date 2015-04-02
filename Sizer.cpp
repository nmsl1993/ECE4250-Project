/*
 * Sizer.cpp
 *
 *  Created on: Mar 31, 2015
 *      Author: noah
 */

#include "Sizer.hpp"
using namespace cv;

int SizeFeature(Mat in)
{
return in.size().width + in.size().height;
}
float AspectRatioFeature(Mat in)
{
	return in.size().width/((float) in.size().height);
}
