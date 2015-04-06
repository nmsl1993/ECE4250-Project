#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "Aesthetics.hpp"
#include "train.hpp"
#include "serialize_mat.hpp"


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
		if(i == readMax)
			break;
	}
	for(int c = 0; c < SVM_PARAMS_NUM; c++)
	{
		Mat col = scoreData.col(c);
		normalize(col,col, 0, 1, NORM_MINMAX);

	}
	cout << "scores normalized" <<endl;
	
	cout << "saving" <<endl;
	Mat c = scoreData.clone(); //We do this to ensure that the data is continous
	boost::serialization::saveMat(c,string("feature.bin"));
	return 0;
}
