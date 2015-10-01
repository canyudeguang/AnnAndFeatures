#ifndef POINTYFEATURE_H
#define POINTYFEATURE_H

#include "featureextractor.h"

#include "lib_stasm/stasm.h"
class PointyFeature : public FeatureExtractor
{
public:
    string name();
    cv::Mat_<float> getFeature(cv::Mat &image);

    void setPoints(vector<cv::Point> vec_landmarks);
    PointyFeature();
    ~PointyFeature();

    void showStasm(string name);
    cv::Point get_point_landmark(int index, float * landmarks);
private:
    cv::Mat _img;
    vector<cv::Point> _vecStasmLandmarks;

    double avgEyeDist();
};

#endif // POINTYFEATURE_H
