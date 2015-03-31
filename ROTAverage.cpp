#include "ROTAverage.hpp"

using namespace cv;
Scalar ROTAverage(Mat in)
{
	//std::cout << in.size().width << std::endl;
	Rect region_of_interest = Rect(in.size().width/3, in.size().height/3,in.size().width/3, in.size().height/3);
	Mat image_roi = in(region_of_interest);
	return mean(image_roi);
}
