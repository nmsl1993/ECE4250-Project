#include <opencv2/opencv.hpp>
#include "KMeansCluster.hpp"
#include "Transforms.hpp"
using namespace cv;
int main()
{
	Mat image,grayScale, gs_float;
	image = imread( "745615.jpg", 1 );
	assert(image.data );
	//Mat censusImage(image.size().width-1,image.size().height-1,CV_8UC1);
	Mat censusImage(image.size().height-1,image.size().width-1,CV_8UC1);
	KMeansCluster kc (image,5);
	cvtColor(image,grayScale,CV_BGR2GRAY);
	grayScale.convertTo(gs_float,CV_32FC1,1/255.0);//gs_float
	//std::cout << gs_float << std::endl;
	censusTransform<float>(gs_float, censusImage);
	imshow("image" , image);

	imshow("gs_float" , gs_float);
	imshow("Census" , censusImage);

	MatND hist;
	int channels[] = {0};
	int histSize[] = {256};
	float gray_range[] = {0,256.0};
	const float * ranges[] = {gray_range};
	calcHist(&censusImage,1,channels,Mat(),hist,1,histSize,ranges,true,false);

//	std::cout << hist.size() << std::endl;

	Mat hist_transpose;
	transpose(hist,hist_transpose);
	Mat cutHist = hist_transpose(Rect(1,0,254,1));

//	std::cout << hist_transpose << std::endl;

//	std::cout << cutHist << std::endl;

//	std::cout << sum(cutHist).val[0] << std::endl;
	Mat floatHist;
	cutHist.convertTo(floatHist,CV_32FC1,1.0/sum(cutHist).val[0]);

	std::cout << floatHist << std::endl;
	while(waitKey(100) != 'x')
	{
	;
	}
	return 0;
}
