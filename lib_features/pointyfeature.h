#ifndef POINTYFEATURE_H
#define POINTYFEATURE_H

#include "featureextractor.h"
class PointyFeature : public FeatureExtractor
{
public:
    string name();
    cv::Mat_<float> getFeature(cv::Mat &image);

    PointyFeature();
    ~PointyFeature();
};

#endif // POINTYFEATURE_H
