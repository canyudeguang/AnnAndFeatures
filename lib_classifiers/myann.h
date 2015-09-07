#ifndef MYANN_H
#define MYANN_H

#include "lib_classifiers/classifier.h"

#define DEBUG
#define INFO


/*
 *  AnnTrainer for Images
 *  @author     Pavel Šimurda
 *  @email      pavel@simurda.eu
 *  @date       07.seo 2015
 *  @desc       Ann improved class for OpenCV allows to perform ANN training and Prediction
 *              for variable tasks. The basic task would be defined as Classifying
 *              images into N classes.
 *              Program logic using this class should be: Load images, Load labels,
 *              set ANN network, perform training, perform testing, evaluate
 *
 *              It has ability to set Network from a file.yml or from a commanline Parameters
 *
 *              It is required to have a FeatureExtractor Class, which will extract
 *              a feature vector from each image. And save thoose values for network
 *              to train.
*/

class myANN : public Classifier
{
public:
    myANN();

    static const double DEFAULT_EPS;
    static const double DEFAULT_BP1;
    static const double DEFAULT_BP2;
    static const int DEFAULT_ITER;


    int loadFromFile(const char *filename);
    int loadFromParams(string params);
    int save2file(const char *filename);

    // Inteface of the Parent
    void train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses);
    std::vector<uchar> predict(cv::Mat_<float> &testData);
    uchar predictResponse(cv::Mat_<float> &testData);
    void showGraph(int featuresNum);

    void setLayers(vector<string> layers);
    void setIterations(int iter);

    ~myANN();

    Mat_<float> predictions;
    vector<uchar> predictLabels;


private:
    CvANN_MLP* nnetwork;

    int iters;
    double bp_param1, bp_param2;
    double eps;

    vector<int> nn_layers;
    int feature_vec_size;
    int num_of_classes;
    Mat layers;



};

#endif // MYANN_H
