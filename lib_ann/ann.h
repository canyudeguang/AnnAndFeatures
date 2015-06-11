#pragma once
#ifndef ANN_H
#define ANN_H

// OpenCV headers
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>

#include <iostream>
#include <string>
using namespace std;

// Other headers
#include "classifier.h"

// states


//==============================================================================
class ANN : public Classifier {
public:

    // static variables
    static const uchar EYE_OPEN = 0;
    static const uchar EYE_CLOSE = 1;
    static const uchar CLASSES = 2;
    static const uchar MOUTH_CLASSES = 6;
    // static functions
    static string labelToString(uchar);
    //static uchar EYE_OPEN;
    // Member variables

    static uchar getEYE_OPEN();
    static uchar getEYE_CLOSE();
    // Ctor
    ANN();
    // Member functions
    void saveTofile(char *filename);
    void loadFromFile(const char *filename);


    void setParameters(int inputs, vector<int> & lay, int output);
    void parametricTrain(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int iter, vector<int>& nodes,int hidden);
    void parametricTrainMouth(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int iter, vector<int>& nodes,int hidden);

    std::vector<uchar> predictMouth(cv::Mat_<float> &testData);

    // Inteface of the Parent
    void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses);
    std::vector<uchar> predict(cv::Mat_<float> &testData);

    uchar predictResponse(cv::Mat_<float> &testData);
    void showGraph(int featuresNum);

    Mat_<float> getResponses();
    vector<uchar> getPredictLabels();

private:

    Mat_<float> predictions;
    vector<uchar> predictLabels;

    static const string LABEL_OPEN;
    static const string LABEL_CLOSED;
    static const string LABEL_NONE;
    //=================================================================================
    CvANN_MLP* nnetwork;

    int attributesPerSample;
    int numberOfClasses;


};



#endif
