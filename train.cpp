#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "Aesthetics.hpp"
#include "train.hpp"
#include "serialize_mat.hpp"

const int trainSize = 3000;
const int iterations = 10;
using namespace std;
using namespace cv;
bool testBinValid(Mat features, Mat labels);
int main()
{
	srand(time(NULL));
	//KMeansCluster(M,4);
	std::cout << "hello train\n" << std::endl;
	Mat featureData;
	Mat labelData;
	cout << "loading" <<endl;
	boost::serialization::loadMat(featureData,string("feature_old.bin"));
	boost::serialization::loadMat(labelData,string("labels.bin"));

	//assert(testBinValid(featureData, labelData));

	int validationSize = featureData.size().height - trainSize;

	for(int c = 0; c < SVM_PARAMS_NUM; c++)
	{
		Mat col = featureData.col(c);
		normalize(col,col, 0, 1, NORM_MINMAX);

	}
	cout << "training set size " << trainSize << " validation size" << validationSize << endl;
	//cout << labelData << endl;

	vector<pair< Mat, float> >vp(featureData.size().height);



	for(int c = 0; c < featureData.size().height; c++)
	{
		//cout << featureData.row(c) << endl;
		vp[c].first = featureData.row(c);

		vp[c].second = round(labelData.at<float>(c,1));
		if(vp[c].second != round(labelData.at<float>(c,1)))
		{
			cout << "probrem w" << vp[c].second << "@ " << c << endl;
		}
	}


	Mat trainingSet(trainSize,featureData.size().width,CV_32FC1);
	Mat trainingLabels(trainSize,1,CV_32FC1);
	Mat validationSet(validationSize,featureData.size().width,CV_32FC1);
	Mat validationLabels(validationSize,1,CV_32FC1);
	
	float accumulator = 0;
	for(int tests = 0; tests < iterations; tests++)
	
	{
	random_shuffle ( vp.begin(), vp.end() );


	for(int i = 0; i < featureData.size().height; i++)
	{
		if(i<trainSize)
		{

			vp[i].first.copyTo(trainingSet.row(i));
		
			trainingLabels.at<float>(i) = vp[i].second;
		}
		else
		{
			vp[i].first.copyTo(validationSet.row(i - trainSize));
			validationLabels.at<float>(i - trainSize) = vp[i].second;
		}
	}
	Mat bottleneck;
	//int match_index = 0;
	//for(match_index = 0; t < featureData.size().height; t++)
	//{

	///}
	//compare(validationSet.row(7000-trainSize), featureData.row(7000) ,bottleneck, CMP_EQ);
	//assert(sum(bottleneck)[0]/255 == SVM_PARAMS_NUM);
	

	/*
	for(int q = 0; q < trainSize; q++)
	{

		if(	!trainingLabels.at<float>(q) == labelData.at<float>(q))
		{
		//cout << "@index" << q << " " << validationLabels.at<float>(q-trainSize) << "?=" << labelData.at<float>(q) << endl;// << "?=" << vp[q].second << endl;

		cout << "@index" << q << " " << trainingLabels.at<float>(q) << "?=" << trainingSet.at<float>(q) << "?=" << vp[q].second << endl;
		}

	}
	*/
/*	
	for(int q = 0; q < trainSize; q++)
	{

		if(	!validationLabels.at<float>(q) == labelData.at<float>(q+trainSize))
		{
		//cout << "@index" << q << " " << validationLabels.at<float>(q-trainSize) << "?=" << labelData.at<float>(q) << endl;// << "?=" << vp[q].second << endl;

		cout << "@index" << q << " " << validationLabels.at<float>(q) << "?=" << labelData.at<float>(q+trainSize) << "?=" << vp[q+trainSize].second << endl;
		}

	}
*/
	 // RBF Kernel
	 // gamma = 3.7
	 // cost = 1
	 // 20 times per feature
	 // 5 fold cross validation
	
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::RBF;
	//params.kernel_type = CvSVM::LINEAR;
	params.gamma = .1;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,10000,1e-6);
	CvSVM SVM;
	cout << trainingSet.size() << " " << trainingLabels.size() << endl;
	SVM.train(trainingSet,trainingLabels,Mat(),Mat(),params);
	std::cout << "done training" << std::endl;


	Mat predict_label;
	SVM.predict(validationSet,predict_label);
	//SVM.predict(trainingSet,predict_label);

	Mat agreements;
	//cout << trainingLabels.rowRange(900,1000) << endl;
	//cout << predict_label.rowRange(900,1000) << endl;
	compare(predict_label,validationLabels, agreements, CMP_EQ);
	//compare(predict_label,trainingLabels, agreements, CMP_EQ);

	agreements /= 255;
	accumulator += sum(agreements).val[0]/agreements.size().height;
	}
	cout << "done prediction, " <<  accumulator/iterations << endl;
	

	return 0;
	
}
bool testBinValid(Mat features, Mat labels)
{
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

	int ran_num = rand() % vs.size();
	char * path_img = (char * ) (path + vs[ran_num].first).c_str();
	Mat feature_vec(1,SVM_PARAMS_NUM ,CV_32FC1);
	Mat agreements;

	scoreImage(path_img,feature_vec);

	if (feature_vec.total() != features.row(ran_num).total())
		return false;
	compare(feature_vec,features.row(ran_num), agreements, CMP_EQ);

	//compare(features,feature_vec, agreements, CMP_EQ);
	int agree_num = sum(agreements)[0]/255;
	cout << agree_num << " ?= " << SVM_PARAMS_NUM << endl;

	cout << labels.at<float>(ran_num) << " ?= " << vs[ran_num].second << endl;

	return agree_num == SVM_PARAMS_NUM &&  labels.at<float>(ran_num) == vs[ran_num].second;
}
