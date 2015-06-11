#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H


#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// GUI mostly for testin purposes only
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


using namespace std;
using namespace cv;

class FeatureExtractor
{
public:
    // Virtual functions implemented in a child class
    virtual cv::Mat_<float> getFeature(cv::Mat &image){return Mat(image);}; // not pure virtual :)
    static cv::Mat_<float> joinFeatures(cv::Mat_<float> &f1, cv::Mat_<float> &f2);
};

#endif // FEATUREEXTRACTOR_H
