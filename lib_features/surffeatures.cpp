#include "surffeatures.h"

SurfFeatures::SurfFeatures()
{

}

string SurfFeatures::name(){
    return "fSurf";
}

SurfFeatures::~SurfFeatures()
{

}


double SurfFeatures::avgDist(vector<KeyPoint> &kp, cv::Point anchor){

    if(kp.size() == 0) return 0;
    double dist = 0;
    for(int i = 0; i < kp.size(); ++i){
        cv::Point keyPoint = kp.at(i).pt;
        dist += cv::norm(anchor - keyPoint);
    }
    return double(dist/kp.size());
}


vector<int> SurfFeatures::getAvgIntensities(cv::Mat &img,vector<KeyPoint> &kp, double dp){
    vector<int> intensities(3); // first black, second avg rest, third white
    intensities[0] = 0;
    intensities[1] = 0;
    intensities[2] = 0;

    double eps = 1;
    int restCount = 0;
    for(int i = 0; i<kp.size(); ++i){
        Point A;
        A = kp[i].pt;

        cv::Mat subIm = cvSupport::cutObject(img,A,dp);

        if(subIm.data) {
            double avgInt = cvSupport::avgIntensity(subIm);
            if(avgInt >= 0){
                if( (avgInt + eps) >= 255 ){
                    intensities[2]++;
                }
                else if(avgInt - eps <= 0){
                    intensities[0]++;
                }
                else{
                    intensities[1]+=avgInt;
                    restCount++;
                }
            }
        }
    }

    if(restCount > 0){
        intensities[1] = intensities[1]/restCount;
    }

    return intensities;
}

cv::Mat_<float> SurfFeatures::getFeature(Mat &image){


    cv::Point mouthA, mouthB;
    mouthA.x = 0.2*image.cols;
    mouthB.x = 0.8*image.cols;
    mouthA.y = image.rows * 0.3;
    mouthB.y = image.rows * 0.75;
    cv::Rect r(mouthA,mouthB);


    int minHessian = 10;
    SurfFeatureDetector detector(minHessian); // 400 = minHessian
    vector<KeyPoint> kp1;

    cv::Mat subimage = cv::Mat(image,r);
    detector.detect(subimage,kp1);

    Mat drawing;
    subimage.copyTo(drawing);
    drawKeypoints(drawing,kp1,drawing,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    imshow("SURF RICH",drawing);


    Mat sub;
    image.copyTo(sub);

    sub = cv::Mat(sub,r);
    cvtColor(sub,sub,CV_BGR2GRAY);
    equalizeHist(sub,sub);



    Histogram h;
    h.setGrayImage(sub,256);
    h.histGrey(1);

    threshold(sub,sub,200,240,CV_THRESH_BINARY);

    imshow("SUBIMAGE",sub);

     vector<int> avgInt = getAvgIntensities(sub,kp1,1); // px Surroundinng;




    cv::Mat_<float> Features(1,3);


    double distM = 100/cv::norm(cv::Point(image.cols*0.2, image.rows/2) - cv::Point(image.cols*0.8, image.rows/2));


    Features(0,0) = avgInt[0];
    Features(0,1) = avgInt[1];
    Features(0,2) = avgInt[2];
    if(kp1.size()>1){
        Features(0,0) = double(avgInt[0])/kp1.size();
        Features(0,1) = avgInt[1]/double(kp1.size());
        Features(0,2) = avgInt[2]/double(kp1.size());
    }



    return Features;
}
