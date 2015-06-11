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

    Mat skel = Mat::zeros(img.size(), CV_8UC1);
    Mat temp;
    Mat eroded;

    Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

    bool done;
    do {
        // Opening
        erode(img, eroded, element);
        dilate(eroded, temp, element); // temp = open(img)

        subtract(img, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(img);

        done = (norm(img) == 0);
    } while (!done);

    imshow("Original", image); waitKey();
    imshow("Skeleton", skel); waitKey();
    return skel;
}
