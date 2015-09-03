#ifndef GRAYSCALEFEATURES_H
#define GRAYSCALEFEATURES_H

#include "featureextractor.h"

#include <vector>
#include <algorithm>

class GrayScaleFeatures : public FeatureExtractor
{
public:
    string name();
    cv::Mat_<float> getFeature(Mat &image);
};

#endif // GRAYSCALEFEATURES_H
