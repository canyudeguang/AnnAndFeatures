#include "boostclass.h"

BoostClass::BoostClass()
{

}

void BoostClass::train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses){
    CvBoostParams params;
    params.boost_type = Boost::REAL;
    params.weak_count = 100;

    params.weight_trim_rate = 0; // <0,1> Saves computation time, 0 to turn off

    params.cv_folds = 0;// cross validation folds
    params.max_depth = 25; // of trees

    // Prepare labels in required format
    //--------------------------------------------------------------------------
    int numberOfSamples = trainData.rows;
    cv::Mat_<float> trainLabels = Mat_<float>::zeros(1, numberOfSamples);
    for(int i = 0; i < labels.size(); i ++){
        trainLabels(0,i) = labels[i];
    }


    this->cvboost = new CvBoost();

    //Both data layouts (tflag=CV_ROW_SAMPLE and tflag=CV_COL_SAMPLE) are supported
    int tflag = CV_ROW_SAMPLE;

    cvboost->train(trainData,tflag,trainLabels,Mat(),Mat(),Mat(),Mat(),params);

#if INFO
    cout << "Boost training DONE" << endl;
#endif
}

vector<uchar> BoostClass::predict(cv::Mat_<float> &testData){

    vector<uchar> responses;
    for(int i = 0; i < testData.rows; ++i){
        float predict = cvboost->predict(testData.row(i));
        responses.push_back(predict);
    }
    return responses;
}

uchar BoostClass::predictResponse(cv::Mat_<float> &testData){

}

void BoostClass::showGraph(int featuresNum){

}


