#ifndef BRIGHTFEATURE_H
#define BRIGHTFEATURE_H

#include "featureextractor.h"
#include "lib_support/cvSupport.h"

#include "lib_features/histogram.h"

class BrightFeature : public FeatureExtractor
{
public:
    BrightFeature();
    ~BrightFeature();


    std::vector<cv::Point> bPoints;

    std::vector<cv::Point> findBrightest(cv::Mat &img);
    void showBrightest(cv::Mat &img);

    cv::Mat_<float> sobel(cv::Mat &image);

    cv::Mat_<float> getFeature(cv::Mat &image);

    cv::Mat claheHist(cv::Mat &img);
    cv::Mat labClahe(cv::Mat &img);

    cv::Mat_<float> myThres(cv::Mat & img, int l, int h);

    vector<vector<Point> > getContours(cv::Mat &img);

    int findNfulledInHistogram(cv::Mat &grImage, int start, int stop, int N, int countThres = 5);

    cv::Mat _gray, _eq, _clahe, _img;

};

#endif // BRIGHTFEATURE_H
