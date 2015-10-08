#ifndef FASTFEATURES_H
#define FASTFEATURES_H

#include "featureextractor.h"
#include "lib_support/cvSupport.h"

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>


class FastFeatures :  public FeatureExtractor
{
public:
    string name();
    cv::Mat_<float> getFeature(Mat &image);

    FastFeatures();
    ~FastFeatures();
};

#endif // FASTFEATURES_H
