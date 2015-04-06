#include "KMeansCluster.hpp"

using namespace cv;
void show_result(const Mat& labels, const Mat& centers, int height, int width);

KMeansCluster::KMeansCluster(Mat in, int cluster_number) {
	image = in;
	cvtColor(image, luv_image, CV_BGR2Luv);

	Mat reshaped_image = luv_image.reshape(1, luv_image.cols * luv_image.rows);

	assert(luv_image.type() == CV_8UC3);
	reshaped_image.convertTo(reshaped_image32f, CV_32FC1, 1.0 / 255.0);

	TermCriteria criteria = TermCriteria(TermCriteria::COUNT, 10, 1);
	kmeans(reshaped_image32f, cluster_number, labels, criteria, 1,
			KMEANS_PP_CENTERS, centers);
//#ifdef _DEBUG_
	show_result(labels, centers, luv_image.rows, luv_image.cols);
//#endif
}
Mat KMeansCluster::getHSVMeans() {

	Mat centers_u8;
		centers.convertTo(centers_u8, CV_8UC1, 255.0);
	Mat hsvCenters, bgrCenters;
	cvtColor(centers_u8.reshape(3),bgrCenters,CV_Luv2BGR);
	cvtColor(bgrCenters,hsvCenters,CV_BGR2HSV);
#ifdef _DEBUG_
	std::cout << centers_u8.reshape(3) << std::endl;
	std::cout << bgrCenters << std::endl;
	std::cout << hsvCenters << std::endl;
#endif
	return hsvCenters;
}
KMeansCluster::~KMeansCluster() {
	// TODO Auto-generated destructor stub
}

void show_result(const Mat& labels, const Mat& centers, int height, int width) {
	assert(labels.type() == CV_32SC1);
	assert(centers.type() == CV_32FC1);

	Mat U8C3_image(height, width, CV_8UC3);
	MatIterator_<Vec3b> U8C3_first = U8C3_image.begin<Vec3b>();
	MatIterator_<Vec3b> U8C3_last = U8C3_image.end<Vec3b>();
	MatConstIterator_<int> label_first = labels.begin<int>();


	//std::cout << centers << std::endl;
	Mat centers_u8;
	centers.convertTo(centers_u8, CV_8UC1, 255.0);

	Mat centers_u8c3 = centers_u8.reshape(3);

	while (U8C3_first != U8C3_last) {
		const Vec3b& luv = centers_u8c3.ptr<Vec3b>(*label_first)[0];
		*U8C3_first = luv;
		++U8C3_first;
		++label_first;
	}
	cvtColor(U8C3_image, U8C3_image, CV_Luv2BGR);

	imshow("KMeans Clustered", U8C3_image);
}

