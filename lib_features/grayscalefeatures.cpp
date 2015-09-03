#include "grayscalefeatures.h"


string GrayScaleFeatures::name(){
    return "GrayscaleFeatures";
}

cv::Mat_<float> GrayScaleFeatures::getFeature(Mat &image){

    Mat_<uchar>img;
    cvtColor(image, img, CV_BGR2GRAY);

    vector<int> I;
    long allIntensities = 0;
    for(int y = 0; y < img.rows; ++y){
        for(int x = 0; x < img.cols; ++x){
            int i = (int)img(y,x);
            allIntensities += i;
            I.push_back(i);
        }
    }


    float averageIntensity = allIntensities / (img.rows * img.cols);

    int medianIntensity = 0;

    sort(I.begin(), I.end());
    if(I.size() % 2 == 0){
        int midAvg = (I[round(((I.size()/2))+1)] + I[(round((I.size()/2)))])/2;
        medianIntensity = I[midAvg];
    }
    else{
        medianIntensity = I[round(I.size()/2)];
    }


    Mat mean, dev;

    meanStdDev(img,mean,dev);

    Mat_<float> fMean(mean);
    Mat_<float> fDev(dev);

    Mat_<float> g(1,3);
    g(0,0) = fMean(0,0);
    g(0,1) = fDev(0,0);
    g(0,2) = medianIntensity;

   // cout << g;
    return g;
}
