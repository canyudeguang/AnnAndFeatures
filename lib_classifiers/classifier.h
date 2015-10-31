#ifndef CLASSIFIER_H
#define CLASSIFIER_H

// OpenCV headers
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

#include "lib_support/support.h"

using namespace std;
using namespace cv;



class Classifier {

public:
    static const string C_ANN, C_SVM;
    static const uchar UNKNOWN;

    Classifier();
    virtual ~Classifier() = 0;
    virtual string name() = 0;

    void setClassesVecLen(int number_of_classes, int feature_vec_size);

    // Virtual functions implemented in a child class
    virtual void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels) = 0;
    virtual std::vector<uchar> predict(cv::Mat_<float> &testData) = 0;
    virtual uchar predictResponse(cv::Mat_<float> &testData) = 0;

    virtual int loadFromParams(string params) = 0;
    virtual string getStrSettings() = 0;

    virtual int loadFromFile(const char * filename) = 0;
    virtual int save2file(const char * filename) = 0;

    void evaluate(std::vector<uchar> predictedLabels, std::vector<uchar> trueLabels, int numClasses);
    double evaluateVerbose(std::vector<uchar> predictedLabels, std::vector<uchar> trueLabels);


    static cv::Mat cr8ResponseMat(std::vector<uchar> & labels, int numberOfSamples);
    // Classifier methods

    void setFeatureVectorSize(int attributesPerSample);

    int numberOfClasses;
    int attributesPerSample;
};


#endif // CLASSIFIER_H
