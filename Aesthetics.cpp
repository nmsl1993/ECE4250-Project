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

