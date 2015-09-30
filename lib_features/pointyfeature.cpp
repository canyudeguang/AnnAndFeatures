#include "pointyfeature.h"

PointyFeature::PointyFeature()
{

}

PointyFeature::~PointyFeature()
{

}


string PointyFeature::name(){
    static const string name = "fPoints";
    return name;
}

cv::Mat_<float> PointyFeature::getFeature(Mat &image){

    Mat_<float> Features(1,3);
    return Features;
}
