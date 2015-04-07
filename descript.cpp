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
	vector<pair <string,int> > vs;

	int t = 0;
	while ( index_train.good() && ground_truth.good())
	{
		getline ( index_train, index, '\n' );
		getline ( ground_truth, score_str, '\n' );
		//cout << index << endl;
		int score;
		istringstream(score_str) >> score;
		pair<string,int> p;
		p.first = trim(index);
		p.second = score;
		vs.push_back(p);
	}

	Mat scoreData(readMax,SVM_PARAMS_NUM, CV_32FC1);
	Mat labelsData(readMax,1, CV_32FC1);
	for(int i = 0; i < readMax; i++)
	{

		char * path_img = (char * ) (path + vs[i].first).c_str();
		Mat roi(scoreData(Rect(0,i,SVM_PARAMS_NUM,1))); // now, it points to the original matrix;
		labelsData.at<float>(i) = vs[i].second;

		scoreImage(path_img,roi);

		cout << i << " of " << vs.size() << endl;
		//cout <<  << endl;
	}
	
	
	
	cout << "scores normalized" <<endl;
	
	cout << "saving" <<endl;
	Mat c = scoreData.clone(); //We do this to ensure that the data is continous
	Mat d = labelsData.clone(); //We do this to ensure that the data is continous

	boost::serialization::saveMat(c,string("feature.bin"));
	boost::serialization::saveMat(d,string("labels.bin"));

	return 0;
}
