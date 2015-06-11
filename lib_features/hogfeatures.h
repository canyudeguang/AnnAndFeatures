#ifndef HOGFEATURES_H
#define HOGFEATURES_H

#include "lib_features/featureextractor.h"
#include "lib_support/cvSupport.h"

#include <opencv2/objdetect/objdetect.hpp>

class HOGFeatures : public FeatureExtractor
{
public:
    cv::Mat_<float> getFeature(Mat &image);
    HOGFeatures();
    ~HOGFeatures();
};

#endif // HOGFEATURES_H
