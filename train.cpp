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
#define readMax 5000 //vs.size()
#define trainMax 1000 //vs.size()
#define resultsLength (readMax-trainMax)
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
		{
			break;
		}
	}
	for(int c = 0; c < SVM_PARAMS_NUM; c++)
	{
		Mat col = scoreData.col(c);
		normalize(col,col, 0, 1, NORM_MINMAX);

	}
	cout << "scores normalized" <<endl;
	Mat trainData(scoreData(Rect(0,0,SVM_PARAMS_NUM,trainMax))); // now, it points to the original matrix;
	Mat trainLabels(labelsData(Rect(0,0,1,trainMax))); // now, it points to the original matrix;

	//cout << scoreData(Rect(0,0,SVM_PARAMS_NUM,readMax)) <<endl;

	/**
	 * RBF Kernel
	 * gamma = 3.7
	 * cost = 1
	 * 20 times per feature
	 * 5 fold cross validation
	 */
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::RBF;
	params.gamma = 3.7;
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
