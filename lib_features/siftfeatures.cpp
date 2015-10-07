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

double SiftFeatures::avgDist(vector<KeyPoint> &kp, cv::Point anchor){

    if(kp.size() == 0) return 0;
    double dist = 0;
    for(int i = 0; i < kp.size(); ++i){
        cv::Point keyPoint = kp.at(i).pt;
        dist += cv::norm(anchor - keyPoint);
    }
    return double(dist/kp.size());
}

cv::Mat_<float> SiftFeatures::getFeature(Mat &image){


    int minHessian = 400;
    SurfFeatureDetector detector(minHessian); // 400 = minHessian
    vector<KeyPoint> kp1;

    detector.detect(image,kp1);


    Mat drawing;
    image.copyTo(drawing);
    drawKeypoints(drawing,kp1,drawing);

    imshow("surf",drawing);

    cv::Mat_<float> Features(1,3);


    double distM = 100/cv::norm(cv::Point(image.cols*0.2, image.rows/2) - cv::Point(image.cols*0.8, image.rows/2));


    Features(0,0) = kp1.size();
    Features(0,1) = this->avgDist(kp1,cv::Point(image.cols*0.2, image.rows/2))/distM;
    Features(0,2) = this->avgDist(kp1,cv::Point(image.cols*0.8, image.rows/2))/distM;


    return Features;
}
