#ifndef RAWFEATURES_H
#define RAWFEATURES_H

#include "featureextractor.h"

class RawFeatures : public FeatureExtractor
{
public:
    Mat_<float> getFeature(Mat &image);
    RawFeatures();
    ~RawFeatures();
    cv::Mat_<float> thres();
    cv::Mat_<float> rgb1D();
private:
    cv::Mat img_orig, img_gray, img_thres;
};

#endif // RAWFEATURES_H
