#ifndef MASKFEATURES_H
#define MASKFEATURES_H


#include "featureextractor.h"
class maskfeatures : public FeatureExtractor
{
public:
    cv::Mat_<float> getFeature(Mat &image);

    maskfeatures();
    ~maskfeatures();
};

#endif // MASKFEATURES_H
