#include "classifier.h"

Classifier::Classifier(){
    this->nullLabel = false;
    this->isRestMember = false;
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


//==============================================================================
void Classifier::setLabels(string labels[], int numOfClasses){
    for(int i = 0; i < numOfClasses; i++){
        this->strLabels.push_back(labels[i]);
        if(labels[i] == "REST"){
            this->isRestMember = true;
        }
    }
    this->numberOfClasses = numOfClasses;

#if INFO
 cout << this->numberOfClasses <<  " Labels set: ";
 for(uint i = 0; i < this->strLabels.size(); i++) cout << this->strLabels[i] << ":" << i << ", ";
 cout << endl;
#endif
}
//==============================================================================
int Classifier::getLabelIndex(string label){
    return find(this->strLabels.begin(), this->strLabels.end(), label) - this->strLabels.begin();
}
//==============================================================================
string Classifier::getLabelString(uint iLabel){
    if(iLabel >= numberOfClasses || iLabel < 0) return "null";
    return this->strLabels[iLabel];
}
//==============================================================================
vector<uchar> Classifier::extLabelFromFileName(vector<string> &fileNames){
    vector<uchar> labels;

    for(size_t i = 0; i < fileNames.size(); ++i){
        uchar foundLabel = this->findStrLabel(fileNames[i]);
       // if(foundLabel){
        //@Must fill all labels even those are wrong - unknown label perhaps (-1)
        labels.push_back(foundLabel);
       // }
    }
#if INFO
 if(!this->nullLabel){
     cout << "Label extraction correct: " << labels.size() << " labels" << endl;
 }
#endif
    return labels;

}
//==============================================================================
uchar Classifier::findStrLabel(const string & filename){
    //find for each label
    for(size_t i = 0; i < this->strLabels.size(); ++i){
        if(filename.find(this->strLabels[i]) != string::npos){
            return i;
        }
    }

    if(isRestMember){
        return this->strLabels.size()-1;
    }


    cout << "fail " << filename << endl;
    this->nullLabel = true;
    return -1;
}
//==============================================================================
bool Classifier::hasNullLabel(){
    return this->nullLabel;
}
