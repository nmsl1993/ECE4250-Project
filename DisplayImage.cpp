#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "ROTAverage.hpp"
using namespace cv;

int main(int argc, char** argv )
{
   if ( argc != 2 )
   {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
   }
   Mat image, hsv_image, luv_image, kmeans_output;
   image = imread( argv[1], 1 );
   if ( !image.data )
   {
        printf("No image data \n");
        return -1;
   }
   cvtColor(image,hsv_image,CV_BGR2HSV);
   cvtColor(image, luv_image, CV_BGR2Luv);
   TermCriteria tc = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS,
           50,
           0.0001);

   std::vector<Point3f> points(luv_image.total());
   //kmeans(points,5,kmeans_output,tc,100,KMEANS_RANDOM_CENTERS);
   Scalar pixMean = mean(hsv_image);
   std::cout << "hsv mean" << pixMean << std::endl;

   pixMean = ROTAverage(hsv_image);
   std::cout << "ROT hsv mean" << pixMean << std::endl;


   pixMean = mean(image);
   std::cout << "bgr mean" << pixMean << std::endl;

   pixMean = ROTAverage(image);
   std::cout << "ROT bgr  mean" << pixMean << std::endl;

   namedWindow("Display Image", WINDOW_AUTOSIZE );
   imshow("Display Image", image);
   waitKey(0);
   return 0;
}

