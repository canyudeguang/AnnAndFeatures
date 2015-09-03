#ifndef SVM_H
#define SVM_H

#include "classifier.h"

#define DEBUG 0
#define INFO 0

class mySVM : public Classifier
{
public:
    mySVM();

    void setTestData(cv::Mat_<float> &testData, vector<uchar> &labels);
    \

    // Interface of the parent
    void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses);
    std::vector<uchar> predict(cv::Mat_<float> &testData);
    uchar predictResponse(cv::Mat_<float> &testData);
    void showGraph(int featuresNum);

    int save2file(const char *filename);
    int loadFromFile(const char *filename);

private:
    CvSVM* cvsvm;
    cv::Mat_<float> mTestData;
    vector<uchar> mLabels;
};

#endif // SVM_H
