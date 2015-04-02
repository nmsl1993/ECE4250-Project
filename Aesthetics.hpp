/*
 * Aesthetics.hpp
 *
 *  Created on: Apr 2, 2015
 *      Author: noah
 */


#ifndef AESTHETICS_HPP_
#define AESTHETICS_HPP_

//#define _DEBUG_
#include <opencv2/opencv.hpp>
const int SVM_PARAMS_NUM = 15;
int displayImage(char* imname);

void scoreImage(char * imname, cv::Mat outputScore);

#endif /* AESTHETICS_HPP_ */
