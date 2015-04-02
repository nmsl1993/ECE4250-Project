#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "Aesthetics.hpp"
#include "train.hpp"

using namespace std;

int main()
{
	//KMeansCluster(M,4);
	std::cout << "hello train\n" << std::endl;
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

	cout << "dinners over" <<endl;
	cv::Mat trainingData(vs.size(),SVM_PARAMS_NUM, CV_32FC1);
	for(map<string,int>::iterator it = vs.begin(); it !=vs.end(); it++)
	{
		if(it->second == 1)
		{
			char * path_img = (char * ) (path + it->first).c_str();
			displayImage(path_img);
			cv::Mat roi(trainingData(cv::Rect(0,vs.size()-5,SVM_PARAMS_NUM,1))); // now, it points to the original matrix;
			scoreImage(path_img,roi);
			break;
		}


	}
	cout << trainingData <<endl;


	return 0;
}
