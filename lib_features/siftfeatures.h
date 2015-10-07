#ifndef SIFTFEATURES_H
#define SIFTFEATURES_H

#include "featureextractor.h"
#include "lib_support/cvSupport.h"

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>

class SiftFeatures : public FeatureExtractor
{
public:

    string name();
    cv::Mat_<float> getFeature(Mat &image);

    double avgDist(vector<KeyPoint> & kp, cv::Point anchor);
    double avgAngle(vector<KeyPoint> & kp);


    SiftFeatures();
    ~SiftFeatures();
};

#endif // SIFTFEATURES_H
