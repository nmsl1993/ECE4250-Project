#include <stdio.h>
#include "ROTAverage.hpp"
#include "KMeansCluster.hpp"
#include "Aesthetics.hpp"
#include "Sizer.hpp"
#include "Transforms.hpp"
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
	static int count = 0;
	/*
	float data[SVM_PARAMS_NUM] = {0.2f,101,t+t/10+t/100.0,3.0f
			,4.0f,5.10001f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f
			,12.0f,13.0f,14.0f};
	 */
	Mat image, hsv_image, luv_image;

	image = imread( imname, 1 );
	assert(image.data );

	/*********HSV MEANS *********/
	Scalar means = mean(image);
	means /= 255.0;
	Mat meanmat(1,1,CV_32FC3, means);
	Mat hsv_means;
	cvtColor(meanmat,hsv_means,CV_BGR2HSV);
	//scvtColor(image,hsv_image,CV_BGR2HSV);

	//Scalar hsv_means2 = mean(hsv_image);

	Mat hsv_means_f3(1,3,CV_32FC1,hsv_means.data);
	//Mat hsv_means_f3(1,3,CV_32FC1,hsv_means2);


	//std::cout << hsv_means_f3 << std::endl;

	//std::cout << hsv_means2 << std::endl;
	hsv_means_f3.copyTo(outputScore(Rect(0,0,3,1)));
	/****************************/

	/*********ROT HSV MEANS *****/
	Scalar rot_means = ROTAverage(image);
	rot_means /= 255.0;


	Mat rot_meanmat(1,1,CV_32FC3, rot_means);
	Mat rot_hsv_means;
	cvtColor(rot_meanmat,rot_hsv_means,CV_BGR2HSV);

	Mat rot_hsv_means_f3(1,3,CV_32FC1,rot_hsv_means.data);
	Mat rot_rgb_means;
	rot_hsv_means_f3.copyTo(outputScore(Rect(3,0,3,1)));
	/*********Sizer *****/

	float d[2] ={SizeFeature(image), AspectRatioFeature(image)};
	Mat sizer(1,2,CV_32FC1, d);
	sizer.copyTo(outputScore(Rect(6,0,2,1)));

	/****************************/

	/******* LUV Image ***/
	cvtColor(image,luv_image,CV_BGR2Luv);
	Scalar luv_mean = mean(luv_image);

	Mat luvmeanmat(1,3,CV_32FC1,luv_mean);

	luvmeanmat.copyTo(outputScore(Rect(8,0,3,1)));

	/****************************/

	/****** Centrist Algornthim **** */
	Mat grayScale, gs_float;
	Mat censusImage(image.size().height-2,image.size().width-2,CV_8UC1);

	cvtColor(image,grayScale,CV_BGR2GRAY);
	grayScale.convertTo(gs_float,CV_32FC1,1/255.0);//gs_float

	censusTransform<float>(gs_float, censusImage);

	MatND hist;
	int channels[] = {0};
	int histSize[] = {256};
	float gray_range[] = {0,256.0};
	const float * ranges[] = {gray_range};
	calcHist(&censusImage,1,channels,Mat(),hist,1,histSize,ranges,true,false);


	Mat hist_transpose;
	transpose(hist,hist_transpose);
	Mat cutHist = hist_transpose(Rect(1,0,254,1));

	Mat floatHist;
	cutHist.convertTo(floatHist,CV_32FC1,1.0/sum(cutHist).val[0]);
	floatHist.copyTo(outputScore(Rect(11,0,254,1)));
count++;
	/******************************/
}

