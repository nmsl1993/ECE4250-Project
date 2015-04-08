/*
 * KMeansCluster.h
 *
 *  Created on: Mar 31, 2015
 *      Author: noah
 */

#ifndef KMEANSCLUSTER_HPP_
#define KMEANSCLUSTER_HPP_

#include <opencv2/opencv.hpp>
#include "Aesthetics.hpp"

class KMeansCluster {
private:
	cv::Mat image, luv_image, centers, labels, reshaped_image32f;
public:
	KMeansCluster (cv::Mat, int);
	virtual ~KMeansCluster();

	cv::Mat getHSVMeans();
	cv::Mat getImage();
};

#endif /* KMEANSCLUSTER_HPP_ */
