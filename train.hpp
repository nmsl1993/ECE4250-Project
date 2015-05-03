/*
 * train.hpp
 *
 *  Created on: Apr 2, 2015
 *      Author: noah
 */

#ifndef TRAIN_HPP_
#define TRAIN_HPP_

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <ctime>
#include <string>
#include <opencv2/opencv.hpp>
#define MAX_DATE 25

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

static std::string get_SVR_name(void)
{
   time_t now;
   char the_date[MAX_DATE];

   the_date[0] = '\0';

   now = time(NULL);

   if (now != -1)
   {
      strftime(the_date, MAX_DATE, "%d_%m_%I_%M.svm", gmtime(&now));
   }

   return std::string(the_date);
}
#endif /* TRAIN_HPP_ */
