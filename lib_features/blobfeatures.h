#ifndef BLOBFEATURES_H
#define BLOBFEATURES_H

#include "lib_features/featureextractor.h"

class BlobFeatures : public FeatureExtractor
{
public:
    BlobFeatures();
    ~BlobFeatures();

    cv::Mat_<float> getFeature(Mat &image);
};

#endif // BLOBFEATURES_H
