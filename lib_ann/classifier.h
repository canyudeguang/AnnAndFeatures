#ifndef CLASSIFIER_H
#define CLASSIFIER_H

// OpenCV headers
#include <opencv/cv.h>
// Standard library header files
#include <iostream>

using namespace std;
using namespace cv;

// Enumeration of available types of classifier
enum classifierTypes{CLASS_ANN, CLASS_SVM};

//==============================================================================
class Classifier {
public:
    // Virtual functions implemented in a child class
    virtual void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses) = 0;
    virtual std::vector<uchar> predict(cv::Mat_<float> &testData) = 0;
    virtual uchar predictResponse(cv::Mat_<float> &testData) = 0;
    virtual void showGraph(int featuresNum) = 0;

    void evaluate(std::vector<uchar> predictedLabels, std::vector<uchar> trueLabels, int numClasses);
};


#endif // CLASSIFIER_H
