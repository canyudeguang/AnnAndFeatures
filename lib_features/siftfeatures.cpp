#include "siftfeatures.h"

SiftFeatures::SiftFeatures()
{

}

SiftFeatures::~SiftFeatures()
{

}

string SiftFeatures::name(){
    return "fSift";
}


cv::Mat_<float> SiftFeatures::getFeature(Mat &image){


    vector<KeyPoint> kp;
    SiftFeatureDetector detector;
    detector.detect(image,kp);

    Mat drawing;
    image.copyTo(drawing);

    drawKeypoints(drawing,kp,drawing);

    imshow("SIFT",drawing);

    cv::Mat_<float> Features(1,3);


    return Features;
}
