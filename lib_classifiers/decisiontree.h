#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "classifier.h"
#include <opencv2/ml/ml.hpp>

class DecisionTrees : public Classifier
{
public:
    DecisionTrees();
    ~DecisionTrees();

    // Interface of the parent
    void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses);
    std::vector<uchar> predict(cv::Mat_<float> &testData);
    uchar predictResponse(cv::Mat_<float> &testData);
    void showGraph(int featuresNum);

    int loadFromFile(const char *filename);
    int save2file(const char *filename);


    cv::DecisionTree * cvDT;

};

#endif // DECISIONTREE_H
