#ifndef EDGEFEATURES_H
#define EDGEFEATURES_H

#include "featureextractor.h"
#include "../lib_support/cvSupport.h"

class EdgeFeatures : public FeatureExtractor
{
public:
    cv::Mat_<float> getFeature(Mat &image);
private:
    void initImgs(Mat &image);
    void showAll();

    Mat img_orig;
    Mat img_gray_scale;
    Mat img_tmp;

};

#endif // EDGEFEATURES_H
