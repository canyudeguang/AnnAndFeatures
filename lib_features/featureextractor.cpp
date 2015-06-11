#include "featureextractor.h"

Mat_<float> FeatureExtractor::joinFeatures(Mat_<float> &f1, Mat_<float> &f2){

    int newCols = f1.cols + f2.cols;
    Mat_<float> joined(1,f1.cols + f2.cols);

    for(int i = 0; i < newCols; ++i){
        if(i >= f1.cols){
            joined(0,i) = f2(0,i-f1.cols);
        }
        else{
            joined(0,i) = f1(0,i);
        }
    }
    joined.copyTo(f1);
    return joined;
}


