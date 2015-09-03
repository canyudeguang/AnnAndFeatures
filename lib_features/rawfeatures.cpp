#include "rawfeatures.h"

string RawFeatures::name(){
    return "RawFeatures";
}

/* Ctor */
RawFeatures::RawFeatures()
{

}
/* Dtor */
RawFeatures::~RawFeatures()
{

}

cv::Mat_<float> RawFeatures::thres(){
    // grayscale image
    cvtColor(this->img_orig, this->img_gray,CV_BGR2GRAY);
    cv::threshold(this->img_gray, img_thres, 25, 255, THRESH_OTSU);


    resize(this->img_thres,this->img_thres,cv::Size(128,64));

    Mat_<float> imgInRow;
    this->img_thres.convertTo(imgInRow, CV_32F); // Convert to float (required by train function)
    imgInRow = imgInRow.reshape(0, 1);
    return imgInRow;
}
cv::Mat_<float> RawFeatures::rgb1D(){

    // Scale image to the same size 128,64
    resize(this->img_orig,this->img_orig,cv::Size(128,64));

    cv::Mat_<float> arrayFromRgb = img_orig.reshape(1, img_orig.channels()*img_orig.size().area());
    arrayFromRgb = arrayFromRgb.reshape(0,1);
    return arrayFromRgb;
}

/* Get Features virtual method */
Mat_<float> RawFeatures::getFeature(Mat &image)
{
    // copy to original image
    image.copyTo(this->img_orig);


    cv::Mat_<float> thers = this->thres();
    cv::Mat_<float> rgb = this->rgb1D();
    return FeatureExtractor::joinFeatures(thers,rgb);

}
