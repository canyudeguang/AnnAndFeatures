#ifndef CORNERFEATURES_H
#define CORNERFEATURES_H

#include "featureextractor.h"
class CornerFeatures : public FeatureExtractor
{
public:
    Mat_<float> getFeature(Mat &image);
};

#endif // CORNERFEATURES_H
