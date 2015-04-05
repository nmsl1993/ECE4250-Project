#include <opencv2/opencv.hpp>

#include "Transforms.hpp"
#include "serialize_mat.hpp"
using namespace cv;
int main()
{
	Mat image,grayScale, gs_float;
	image = imread( "94615.jpg", 1 );
	assert(image.data );
	//Mat censusImage(image.size().width-1,image.size().height-1,CV_8UC1);
	Mat censusImage(image.size().height-1,image.size().width-1,CV_8UC1);

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

	std::cout << hist.size() << std::endl;

	Mat hist_transpose;
	transpose(hist,hist_transpose);
	Mat cutHist = hist_transpose(Rect(1,0,254,1));

	std::cout << hist_transpose << std::endl;

	std::cout << cutHist << std::endl;

	std::cout << sum(cutHist).val[0] << std::endl;
	Mat floatHist;
	cutHist.convertTo(floatHist,CV_32FC1,1.0/sum(cutHist).val[0]);
	while(waitKey(100) != 'x')
	{
	;
	}
	return 0;
}

/*
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "Aesthetics.hpp"
#include "train.hpp"
//#include "serialize_mat.hpp"


using namespace std;
using namespace cv;
#define readMax vs.size()
int main()
{
	//KMeansCluster(M,4);
	std::cout << "hello descript\n" << std::endl;
	//displayImage("94615.jpg");

	ifstream index_train ( "../training/index_train.txt" );
	ifstream ground_truth ( "../training/ground_truth.txt" );

	string index;
	string score_str;
	string path ("../training/image/");
	map<string,int> vs;

	int t = 0;
	while ( index_train.good() && ground_truth.good())
	{
		getline ( index_train, index, '\n' );
		getline ( ground_truth, score_str, '\n' );
		//cout << index << endl;
		int score;
		istringstream(score_str) >> score;
		vs[trim(index)]=score;
	}

	Mat scoreData(readMax,SVM_PARAMS_NUM, CV_32FC1);
	Mat labelsData(readMax,1, CV_32FC1);
	int i = 0;
	for(map<string,int>::iterator it = vs.begin(); it !=vs.end(); it++)
	{

		char * path_img = (char * ) (path + it->first).c_str();
		Mat roi(scoreData(Rect(0,i,SVM_PARAMS_NUM,1))); // now, it points to the original matrix;
		labelsData.at<float>(i) = it->second;

		scoreImage(path_img,roi);

		cout << i << " of " << vs.size() << endl;
		i++;
	}
	for(int c = 0; c < SVM_PARAMS_NUM; c++)
	{
		Mat col = scoreData.col(c);
		normalize(col,col, 0, 1, NORM_MINMAX);

	}
	cout << "scores normalized" <<endl;
	
	return 0;
}
*/