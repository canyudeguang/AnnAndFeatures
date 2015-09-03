#ifndef SKELETFEATURES_H
#define SKELETFEATURES_H

#include "lib_features/featureextractor.h"

class SkeletFeatures : public FeatureExtractor
{
public:
    SkeletFeatures();
    ~SkeletFeatures();

    string name();
    cv::Mat_<float> getFeature(Mat &image);
};

#endif // SKELETFEATURES_H
