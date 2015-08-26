#ifndef INTEGRALFEATURE_H
#define INTEGRALFEATURE_H

#include "lib_features/featureextractor.h"

#include "lib_support/cvSupport.h"


class IntegralRect{
public:
    cv::Point A,B,C,D;
    bool isWhite;

    IntegralRect(cv::Point tl, cv::Point br, bool isWhite);
    int calcIntegral(cv::Mat &integralImage);

    void show(cv::Mat &img);
    void imprint(cv::Mat &img);
};

class IntegralF{

private:
    cv::Point _tlW, _brW, _tlB, _brB;
    std::vector<IntegralRect> Whites, Blacks;


public:
    int sumWhite;
    int sumBlack;

    IntegralF(cv::Point tlW, cv::Point brW, cv::Point tlB, cv::Point brB);
    IntegralF(cv::Mat & img, int type);

    static void showFeature(cv::Mat &image, IntegralF & feature);
    void showFeature(Mat &image);

    int calcFromIntegralImage(cv::Mat &integralImage);

    static enum TYPE {
        Edge = 0,
        Edges,
        Cross,
        Centroid
    } types;


};

class IntegralFeature : public FeatureExtractor
{

public:

    IntegralFeature();
    ~IntegralFeature();

    cv::Mat_<float> getFeature(cv::Mat &image);


    int IntegerSum(cv::Mat &intImage, cv::Point a, cv::Point b);
};





#endif // INTEGRALFEATURE_H
