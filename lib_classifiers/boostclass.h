#ifndef BOOSTCLASS_H
#define BOOSTCLASS_H

#include "classifier.h"

#include "opencv2/ml/ml.hpp"

#define INFO 1

class BoostClass : public Classifier
{
public:
    BoostClass();

    // Interface of the parent
    void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses);
    std::vector<uchar> predict(cv::Mat_<float> &testData);
    uchar predictResponse(cv::Mat_<float> &testData);
    void showGraph(int featuresNum);

private:
    CvBoost *cvboost;
};

#endif // BOOSTCLASS_H
