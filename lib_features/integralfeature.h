#ifndef INTEGRALFEATURE_H
#define INTEGRALFEATURE_H

#include "lib_features/featureextractor.h"

#include "lib_support/cvSupport.h"

class IntegralFeature : public FeatureExtractor
{
public:
    IntegralFeature();
    ~IntegralFeature();

    cv::Mat_<float> getFeature(cv::Mat &image);

    int IntegerSum(cv::Mat &intImage, cv::Point a, cv::Point b);
};

#endif // INTEGRALFEATURE_H
