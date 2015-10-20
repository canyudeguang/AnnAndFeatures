#ifndef SURFFEATURES_H
#define SURFFEATURES_H

#include "featureextractor.h"
#include "lib_support/cvSupport.h"

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "lib_features/histogram.h"

class SurfFeatures: public FeatureExtractor
{
public:

    string name();
    cv::Mat_<float> getFeature(Mat &image);

    double avgDist(vector<KeyPoint> & kp, cv::Point anchor);
    double avgAngle(vector<KeyPoint> & kp);


    vector<int> getAvgIntensities(cv::Mat & img, vector<KeyPoint> & kp, double dp = 10.0);
    SurfFeatures();
    ~SurfFeatures();
};

#endif // SURFFEATURES_H
