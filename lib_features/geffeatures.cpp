#include "geffeatures.h"

GEFfeatures::GEFfeatures()
{

}

GEFfeatures::~GEFfeatures()
{

}

string GEFfeatures::name(){
    return "fGEF";
}


cv::Mat_<float> GEFfeatures::getFeature(cv::Mat & image){
    cv::Mat_<float> Features;

    cv::Mat img;


    double sig,theta,lambda,gamma,phi;
    sig = 20;       // gaussvar;
    lambda = 40.0;  //wavelength
    theta = 180;   //orientation
    phi = 0.3;      //phaseoffset
    gamma = 0.5;    //aspectratio
imshow("image",image);
    for(int i = 0; i < 360; i+=10){
        image.copyTo(img);
        theta = i;
        cv::Mat kernel = cv::getGaborKernel(cv::Size(100,100), sig, theta, lambda, gamma, phi);
        cv::filter2D(img, img, CV_32F, kernel);

        imshow("GABOR",img);
        cvWaitKey(0);
    }

    return Features;
}


