#include "blobfeatures.h"

BlobFeatures::BlobFeatures()
{

}

BlobFeatures::~BlobFeatures()
{

}

cv::Mat_<float> BlobFeatures::getFeature(Mat &image){


    SimpleBlobDetector::Params params;
    params.minThreshold = 10;
    params.maxThreshold = 200;

    params.filterByArea = true;
    params.minArea = 1000;

    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    params.filterByConvexity = true;
    params.minConvexity = 0.87;

    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;

    SimpleBlobDetector SBD(params);



    vector<KeyPoint> keypoints;
    SBD.detect(image,keypoints);


    cout << "BLOB feature: "<< keypoints.size() << endl;

    Mat img_kp;
    drawKeypoints(image,keypoints,img_kp, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imshow("keypoints", img_kp );
    cv::Mat_<float> features;
    return features;

}

