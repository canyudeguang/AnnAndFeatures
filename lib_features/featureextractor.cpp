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


cv::Mat_<float> FeatureExtractor::extractFromImgs(vector<string> &image_paths, vector<FeatureExtractor *> vec_features){
    cv::Mat_<float> Features;
    for(uint i = 0; i < image_paths.size(); ++i){
        cv::Mat img = imread(image_paths[i],CV_LOAD_IMAGE_ANYCOLOR);
        // joined features from all extractors
        Mat_<float> fjoined;
        fjoined.setTo(0);

        for(uint j = 0; j < vec_features.size(); ++j){
            Mat_<float> f = vec_features[j]->getFeature(img);
            FeatureExtractor::joinFeatures(fjoined,f);
        }

        Features.push_back(fjoined); // add feature_vector to mat of all features
    }

    return Features;
}
