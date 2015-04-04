#include <opencv2/opencv.hpp>
#include "Transforms.h"
using namespace cv;
int main()
{
	Mat image,grayScale, gs_float;
	image = imread( "94615.jpg", 1 );
	assert(image.data );
	Mat censusImage(image.size().width,image.size().height,CV_8UC1);

	cvtColor(image,grayScale,CV_BGR2GRAY);
	grayScale.convertTo(gs_float,CV_32FC1,1/255.0);//gs_float
	//std::cout << gs_float << std::endl;
	censusTransform<float>(gs_float, censusImage);
	imshow("gs_float" , gs_float);
	imshow("Census" , censusImage*255);
	waitKey(0);
	return 0;
}
