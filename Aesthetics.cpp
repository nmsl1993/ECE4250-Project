#include <stdio.h>
#include "ROTAverage.hpp"
#include "KMeansCluster.hpp"
#include "Aesthetics.hpp"
using namespace cv;


int displayImage(char* imname)
{

   Mat image, hsv_image, luv_image, kmeans_output;
   image = imread( imname, 1 );
   if ( !image.data )
   {
        printf("No image data \n");
        return -1;
   }
   cvtColor(image,hsv_image,CV_BGR2HSV);
   TermCriteria tc = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS,
           50,
           0.0001);
   Scalar pixMean = mean(hsv_image);
#ifdef _DEBUG_
   std::cout << "hsv mean" << pixMean << std::endl;
#endif
   pixMean = ROTAverage(hsv_image);
#ifdef _DEBUG_

   std::cout << "ROT hsv mean" << pixMean << std::endl;
#endif


   pixMean = mean(image);
#ifdef _DEBUG_

   std::cout << "bgr mean" << pixMean << std::endl;
#endif

   pixMean = ROTAverage(image);
#ifdef _DEBUG_

   std::cout << "ROT bgr  mean" << pixMean << std::endl;
#endif

   KMeansCluster kc(image, 5);
   kc.getHSVMeans();
   namedWindow("Display Image", WINDOW_AUTOSIZE );
   imshow("Display Image", image);
   waitKey(0);
   return 0;
}
void scoreImage(char * imname, Mat outputScore)
{
	float data[SVM_PARAMS_NUM] = {0.0f,101,2.0f,3.0f
			,4.0f,5.1f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f
			,12.0f,13.0f,14.0f};
	Mat temp(1,SVM_PARAMS_NUM,CV_32FC1,data);

	temp.copyTo(outputScore);
	//std::cout << score << std::endl;

}

