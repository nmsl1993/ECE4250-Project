#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
BOOST_SERIALIZATION_SPLIT_FREE(Mat)
namespace boost {
namespace serialization {

    /*** Mat ***/
    template<class Archive>
    void save(Archive & ar, const Mat& m, const unsigned int version)
    {
      size_t elemSize = m.elemSize(), elemType = m.type();

      ar & m.cols;
      ar & m.rows;
      ar & elemSize;
      ar & elemType; // element type.
      size_t dataSize = m.cols * m.rows * m.elemSize();

      //cout << "Writing matrix data rows, cols, elemSize, type, datasize: (" << m.rows << "," << m.cols << "," << m.elemSize() << "," << m.type() << "," << dataSize << ")" << endl;

      for (size_t dc = 0; dc < dataSize; ++dc) {
          ar & m.data[dc];
      }
    }

    template<class Archive>
    void load(Archive & ar, Mat& m, const unsigned int version)
    {
        int cols, rows;
        size_t elemSize, elemType;

        ar & cols;
        ar & rows;
        ar & elemSize;
        ar & elemType;

        m.create(rows, cols, elemType);
        size_t dataSize = m.cols * m.rows * elemSize;

        //cout << "reading matrix data rows, cols, elemSize, type, datasize: (" << m.rows << "," << m.cols << "," << m.elemSize() << "," << m.type() << "," << dataSize << ")" << endl;

        for (size_t dc = 0; dc < dataSize; ++dc) {
                  ar & m.data[dc];
        }
    }

    void saveMat(Mat& m, string filename) {
            assert(m.isContinuous());
            ofstream ofs(filename.c_str());
            boost::archive::binary_oarchive oa(ofs);
            //boost::archive::text_oarchive oa(ofs);
            oa << m;
    }

    void loadMat(Mat& m, string filename) {
            std::ifstream ifs(filename.c_str());
            boost::archive::binary_iarchive ia(ifs);
            //boost::archive::text_iarchive ia(ifs);
            ia >> m;
    }
}
}