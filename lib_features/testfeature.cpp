#include "testfeature.h"

testFeature::testFeature()
{

}

string testFeature::name(){
    return "fTest";
}

testFeature::~testFeature()
{

}




cv::Mat_<float> testFeature::getFeature(Mat &image){
    cv::Mat_<float> Features(1,2);

    image.copyTo(_img);

    cvtColor(_img,grayscale, CV_BGR2GRAY);

    cv::Mat thres;

    cv::bilateralFilter(_img, thres, 9, 75,75);

cvtColor(thres,thres, CV_BGR2GRAY);


//   medianBlur(grayscale, thres,9);

    adaptiveThreshold(thres,thres,255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,(_img.cols/15)*2+1,0);

imshow("img",thres);




    return Features;
}

