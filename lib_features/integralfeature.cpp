#include "integralfeature.h"

IntegralFeature::IntegralFeature()
{

}

IntegralFeature::~IntegralFeature()
{

}


int IntegralFeature::IntegerSum(cv::Mat &intImage, Point a, Point b ){
    int iB = intImage.at<int>(b.y,b.x);
    int iA = intImage.at<int>(a.y,a.x);
    int res = iB - iA;
    //cout << iB << " - " << iA << " = " << res << endl;
    return res;
}

cv::Mat_<float> IntegralFeature::getFeature(Mat &image){

    // Boreder mouth points
    int width = image.cols/3;
    int height = image.rows/3;

    Point center = Point(image.cols/2,image.rows/2);
    Point k = Point(center.x + width, center.y);
    Point k1 = Point(center.x - width, center.y);
    Point q = Point(center.x, center.y + height);
    Point q1 = Point(center.x, center.y - height);

    Point tl = Point(k1.x, q1.y);
    Point br = Point(k.x,q.y);

    cv::Mat integralImage;
    integral(image,integralImage);

    int total = IntegerSum(integralImage,tl,br);

    int top = IntegerSum(integralImage, tl, Point(br.x,center.y));
    int bottom = IntegerSum(integralImage, Point(tl.x, center.y), br);

    int ratio = width/10;
    int centralPoint = IntegerSum(integralImage,Point(center.x - ratio, center.y - ratio*2), Point(center.x + ratio, center.y + ratio));


    cv::Mat_<float> Features(1,3);
    Features(0,0) = top;
    Features(0,1) = bottom;
    Features(0,2) = centralPoint;

    //cout << Features << endl;

    return Features;
}
