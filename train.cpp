#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "Aesthetics.hpp"
#include "train.hpp"

using namespace std;
using namespace cv;
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

	int t = 0;
	vector<pair<string,int> > vs;
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
	random_shuffle ( vs.begin(), vs.end() );
	cout << "dinners over" <<endl;
	#define readMax 1000 //vs.size()
	#define trainMax 150 //vs.size()
	#define resultsLength (readMax-trainMax)


	Mat scoreData(readMax,SVM_PARAMS_NUM, CV_32FC1);
	Mat labelsData(readMax,1, CV_32FC1);
	int i = 0;
	for(int i = 0; i < readMax; i++)
	{

		char * path_img = (char * ) (path + vs[i].first).c_str();
		Mat roi(scoreData(Rect(0,i,SVM_PARAMS_NUM,1))); // now, it points to the original matrix;
		labelsData.at<float>(i) = vs[i].second;

		scoreImage(path_img,roi);

		cout << i << " of " << vs.size() << endl;
		//cout <<  << endl;

	}
		for(int c = 0; c < SVM_PARAMS_NUM; c++)
	{
		Mat col = scoreData.col(c);
		normalize(col,col, 0, 1, NORM_MINMAX);

	}
	
	cout << "scores normalized" <<endl;
	//Mat trainData; // now, it points to the original matrix;
	//Mat trainLabels;

	//transpose(scoreData(Rect(0,0,SVM_PARAMS_NUM,trainMax)),trainData);
	//transpose(labelsData(Rect(0,0,1,trainMax)),trainLabels); // now, it points to the original matrix;

	Mat trainData(scoreData(Rect(0,0,SVM_PARAMS_NUM,trainMax)));
	Mat trainLabels(labelsData(Rect(0,0,1,trainMax)));

	//cout << scoreData(Rect(0,0,SVM_PARAMS_NUM,readMax)) <<endl;

	
	 // RBF Kernel
	 // gamma = 3.7
	 // cost = 1
	 // 20 times per feature
	 // 5 fold cross validation
	
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	//params.kernel_type = CvSVM::RBF;
	params.kernel_type = CvSVM::LINEAR;
	std::cout << trainData.size() << trainLabels.size() << std::endl;
	//params.gamma = 3.7;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);

	CvSVM SVM;

	SVM.train(trainData,trainLabels,Mat(),Mat(),params);
	std::cout << "done training" << std::endl;
	Mat predict_label;
	SVM.predict(scoreData,predict_label);

	Mat agreements;
	compare(labelsData(Rect(0,trainMax,1,resultsLength)),predict_label(Rect(0,trainMax,1,resultsLength)), agreements, CMP_EQ);
	agreements /= 255;

	std::cout << agreements.total() << " = " << resultsLength << std::endl;
	std::cout << "done prediction, " << sum(agreements)/(float)resultsLength << std::endl;
	return 0;
}
