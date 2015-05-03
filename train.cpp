#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "Aesthetics.hpp"
#include "train.hpp"
#include "serialize_mat.hpp"

const int iterations = 10;
using namespace std;
using namespace cv;
const int train_mode = 1;
const int trainSize = 3000;
bool testBinValid(Mat features, Mat labels);

void train(cv::Mat trainingSet, cv::Mat trainingLabels, CvSVM * SVR);

int main()
{
	std::cout << "hello train\n" << std::endl;
	Mat featureData;
	Mat labelData;
	boost::serialization::loadMat(featureData,string("feature_old.bin"));
	boost::serialization::loadMat(labelData,string("labels.bin"));

	int validationSize = featureData.size().height - trainSize;


	for(int c = 0; c < SVM_PARAMS_NUM; c++)
	{
		Mat col = featureData.col(c);
		normalize(col,col, 0, 1, NORM_MINMAX);

	}

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
	
	CvSVM SVM;
	if(train_mode)
	{
	Mat trainingSet(trainSize,featureData.size().width,CV_32FC1);
	Mat trainingLabels(trainSize,1,CV_32FC1);
	Mat validationSet(validationSize,featureData.size().width,CV_32FC1);
	Mat validationLabels(validationSize,1,CV_32FC1);
	srand(time(NULL));
	random_shuffle ( vp.begin(), vp.end() );
	for(int i = 0; i < featureData.size().height; i++)
	{
		if(i<trainSize)
		{

			vp[i].first.copyTo(trainingSet.row(i));
		
			trainingLabels.at<float>(i) = (float) (rand()% 10);
		}
		else
		{
			vp[i].first.copyTo(validationSet.row(i - trainSize));
			validationLabels.at<float>(i - trainSize) = vp[i].second;
		}
	}

	train(trainingSet, trainingLabels,&SVM);
    std::string name = get_SVR_name();
	cout << name << endl;
	SVM.save(name.c_str());
	}


	/*
	Mat predict_label;
	SVM.predict(validationSet,predict_label);
	//SVM.predict(trainingSet,predict_label);

	Mat agreements;
	compare(predict_label,validationLabels, agreements, CMP_EQ);
	//compare(predict_label,trainingLabels, agreements, CMP_EQ);

	agreements /= 255;
	accumulator += sum(agreements).val[0]/agreements.size().height;

	*/
	return 0;
	
}
void train(Mat trainingSet, Mat trainingLabels, CvSVM  *SVR)
{
	
	CvSVMParams params;
	params.svm_type = CvSVM::EPS_SVR;
	params.kernel_type = CvSVM::LINEAR;
	params.p = 1;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,10000,1e-6);
	SVR->train(trainingSet,trainingLabels,Mat(),Mat(),params);
	//SVR->train_auto(trainingSet,trainingLabels,Mat(),Mat(),params,10);
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
