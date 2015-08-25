#include "decisiontree.h"

DecisionTrees::DecisionTrees()
{

}

DecisionTrees::~DecisionTrees()
{

}


void DecisionTrees::train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses){

    this->cvDT = new DecisionTree();


    float *priors = NULL;  // weights of each classification for classes
    DTreeParams params = DTreeParams(25, // max depth
                                      5, // min sample count
                                      0, // regression accuracy: N/A here
                                  false, // compute surrogate split, no missing data
                                     15, // max number of categories (use sub-optimal algorithm for larger numbers)
                                     15, // the number of cross-validation folds
                                  false, // use 1SE rule => smaller tree
                                  false, // throw away the pruned tree branches
                                  priors // the array of priors
                                  );

    Mat training_classifications = Mat(trainData.rows, 1, CV_32FC1);
    cv::Mat trainLabels = Classifier::cr8ResponseMat(labels,trainData.rows);


    int tFlag = CV_ROW_SAMPLE;
    this->cvDT->train(trainLabels, tFlag, training_classifications,Mat(),Mat(),Mat(),Mat(),params);

}


std::vector<uchar> DecisionTrees::predict(cv::Mat_<float> &testData){
std::vector<uchar> responses;
return responses;
}

uchar DecisionTrees::predictResponse(cv::Mat_<float> &testData){
    return 0;
}

void DecisionTrees::showGraph(int featuresNum){

}
