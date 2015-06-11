#include "classifier.h"

//==============================================================================
void Classifier::evaluate(vector<uchar> predictedLabels, vector<uchar> trueLabels, int numClasses)
{
    cout << "Evaluation..." << endl;


    // Print true and predicted labels and precision rate
    //--------------------------------------------------------------------------
    cout << "True labels:\t\t";
    for(unsigned int i = 0; i < trueLabels.size(); i++) cout << " " << (int)trueLabels[i];
    cout << endl;

    cout << "Predicted labels:\t\t";
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
