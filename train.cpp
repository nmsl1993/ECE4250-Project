#include <iostream>
#include <opencv2/opencv.hpp>
#include "KMeansCluster.hpp"
using namespace cv;
int main()
{
	Mat M(7,7,CV_32FC2,Scalar(1,3));
	//KMeansCluster(M,4);
	std::cout << "hello train\n" << std::endl;
	return 0;
}
