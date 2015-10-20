#ifndef TESTFEATURE_H
#define TESTFEATURE_H

#include "lib_features/featureextractor.h"


class testFeature : public FeatureExtractor
{
public:
    testFeature();
    ~testFeature();

    cv::Mat_<float> getFeature(Mat &image);
    string name();
    cv::Mat _img;
    cv::Mat grayscale;
};

#endif // TESTFEATURE_H
