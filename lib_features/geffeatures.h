#ifndef GEFFEATURES_H
#define GEFFEATURES_H

#include "featureextractor.h"
#include "lib_support/cvSupport.h"


class GEFfeatures : public FeatureExtractor
{
public:
    GEFfeatures();
    ~GEFfeatures();

     cv::Mat_<float> getFeature(cv::Mat &image);
     string name();
};

#endif // GEFFEATURES_H
