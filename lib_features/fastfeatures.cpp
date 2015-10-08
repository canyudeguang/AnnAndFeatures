#include "fastfeatures.h"

FastFeatures::FastFeatures()
{

}

FastFeatures::~FastFeatures()
{

}

string FastFeatures::name(){
    return "fSift";
}


cv::Mat_<float> FastFeatures::getFeature(Mat &image){


    vector<KeyPoint> kp;
    FastFeatureDetector detector;
    detector.detect(image,kp);

    Mat drawing;
    image.copyTo(drawing);

    drawKeypoints(drawing,kp,drawing);

    imshow("FAST",drawing);

    cv::Mat_<float> Features(1,3);


    return Features;
}
