#include "classifier.h"

const string Classifier::C_ANN = "ANN";
const string Classifier::C_SVM = "SVM";
const uchar Classifier::UNKNOWN = 255;

Classifier::Classifier(){
    this->numberOfClasses = 0;
}
Classifier::~Classifier(){
}

void Classifier::setClassesVecLen(int number_of_classes, int feature_vec_size){
    this->numberOfClasses = number_of_classes;
    this->attributesPerSample = feature_vec_size;
}

cv::Mat Classifier::cr8ResponseMat(std::vector<uchar> &labels, int numberOfSamples){
    // Prepare labels in required format
    //--------------------------------------------------------------------------
    cv::Mat_<float> trainLabels = Mat_<float>::zeros(1, numberOfSamples);
    for(int i = 0; i < labels.size(); i ++){
        trainLabels(0,i) = labels[i];
    }
    return trainLabels;
}

//==============================================================================
void Classifier::evaluate(vector<uchar> predictedLabels, vector<uchar> trueLabels, int numClasses)
{
    cout << "Evaluation..." << endl;


    // Print true and predicted labels and precision rate
    //--------------------------------------------------------------------------
    cout << "True labels:\t";
    for(unsigned int i = 0; i < trueLabels.size(); i++) cout << " " << (int)trueLabels[i];
    cout << endl;

    cout << "Predicted labels:\t";
    for(unsigned int i = 0; i < predictedLabels.size(); i++) cout << " " << (int)predictedLabels[i];
    cout << endl;

    int correctCount = 0;
    for(unsigned int i = 0; i < trueLabels.size(); i++) {
        if(trueLabels[i] == predictedLabels[i]) {
            correctCount++;
        }
    }
    cout << "Precision rate: " << ((correctCount * 100) / float(trueLabels.size())) << "%" << endl;


    // Print confusion matrix
    //--------------------------------------------------------------------------
    // Create confusion matrix (rows = true class, cols = predicted class)
    Mat_<int> confMatrix = Mat_<int>::zeros(numClasses, numClasses);
    for(unsigned int i = 0; i < trueLabels.size(); i++) {
        confMatrix(trueLabels[i], predictedLabels[i])++;
    }

    cout << "Confusion matrix:" << endl;
    cout << confMatrix << endl;
}
//==============================================================================
double Classifier::evaluateVerbose(vector<uchar> predictedLabels, vector<uchar> trueLabels)
{
    // Print true and predicted labels and precision rate
    //--------------------------------------------------------------------------

    int correctCount = 0;
    for(unsigned int i = 0; i < trueLabels.size(); i++) {
        if(trueLabels[i] == predictedLabels[i]) {
            correctCount++;
        }
    }
    cout << double((correctCount * 100) / float(trueLabels.size())) << "%" << endl;
    return double((correctCount * 100) / float(trueLabels.size()));
}


void Classifier::setFeatureVectorSize(int attributesPerSample){
    this->attributesPerSample = attributesPerSample;
}
