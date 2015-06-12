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

#define DEBUG 1

/*
 *  AnnTrainer for Images
 *  @author     Pavel Šimurda
 *  @email      pavel@simurda.eu
 *  @date       12.jun 2015
 *  @desc       Ann class for OpenCV allows to perform ANN training and Prediction
 *              for variable tasks. The basic task would be defined as Classifying
 *              images into N classes.
 *              Program logic using this class should be: Load images, Load labels,
 *              set ANN networ, perform training, perform testing, evaluate
 *
 *              It is required to have a FeatureExtractor Class, which will extract
 *              a feature vector from each image. And save thoose values for network
 *              to train.
*/

//==============================================================================
class ANN : public Classifier {
public:
    /**
     * @brief Set string values for labels
     * @param labels Array of string with labels
     * @param numOfClasses number of labels
     */
    void setLabels( string labels[],int numOfClasses);
    /**
     * @brief Get index of given string label
     * @param label string of a label
     * @return index(numerical label) of the string, returns numberOfClasses+1 if doesn't exist
     */
    int getLabelIndex(string label);
    /**
     * @brief Get string value of numerical label
     * @param iLabel index(numerical value) of a label
     * @return string value of label given by its index(numerical label), "null" if doesn't exist
     */
    string getLabelString(uint iLabel);
    /**
     * @brief Extracts labels from filenames (or with path) into ANN object. Requires strLabel to be as name of the image
     * Attention! If it will mar as a label if strLabel occurs in the path (any folder or filename)
     * Therefore if you want to label images by separating them in different directories or by filename
     * both is possible but don't mix them
     * @param fileNames vector of strings with filenames
     * @return vector with uchar labels
     */
    vector<uchar> extLabelFromFileName(vector<string> &fileNames);



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

    uchar findStrLabel(const string & filename);

    vector<string> strLabels;




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
