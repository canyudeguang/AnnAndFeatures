#include "skeletfeatures.h"

SkeletFeatures::SkeletFeatures()
{

}

SkeletFeatures::~SkeletFeatures()
{

}

cv::Mat_<float> SkeletFeatures::getFeature(Mat &image){
    Mat img;
    image.copyTo(img);

    resize(img,img,cv::Size(128,64));

    cvtColor(img,img,CV_BGR2GRAY);
    cv::threshold( img, img, 25, 255, THRESH_OTSU);

    Mat_<uchar> skel = Mat::zeros(img.size(), CV_8UC1);
    Mat temp;
    Mat eroded;

    Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

    bool done;
    int i = 0;
    do {
        // Opening
        erode(img, eroded, element);
        dilate(eroded, temp, element); // temp = open(img)

        subtract(img, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(img);

        done = (norm(img) == 0);
        i++;
       // if(i >= 10000) break;
    } while (!done);

    //imshow("Original", img);
    //imshow("Skeleton", skel); waitKey();

    cv::Mat_<float> feat;
    //skel.convertTo(feat,CV_32F);
    feat = skel.reshape(0,1);
    return feat;
}
