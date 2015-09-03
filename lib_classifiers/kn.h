#ifndef KN_H
#define KN_H

#include "classifier.h"

#define DEBUG 0
#define INFO 0

class KN : public Classifier
{
public:
    KN();

    // Interface of the parent
    void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses);
    std::vector<uchar> predict(cv::Mat_<float> &testData);
    uchar predictResponse(cv::Mat_<float> &testData);
    void showGraph(int featuresNum);

    int save2file(const char *filename);
    int loadFromFile(const char *filename);

    KNearest * cvKn;

};

#endif // KN_H
