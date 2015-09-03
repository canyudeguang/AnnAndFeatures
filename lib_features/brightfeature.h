#ifndef BRIGHTFEATURE_H
#define BRIGHTFEATURE_H

#include "featureextractor.h"

class BrightFeature : public FeatureExtractor
{
public:
    BrightFeature();
    ~BrightFeature();

    string name();

    std::vector<cv::Point> bPoints;

    std::vector<cv::Point> findBrightest(cv::Mat &img);
    void showBrightest(cv::Mat &img);

    cv::Mat_<float> getFeature(cv::Mat &image);

};

#endif // BRIGHTFEATURE_H
