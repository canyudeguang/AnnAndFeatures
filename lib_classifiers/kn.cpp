#include "kn.h"

KN::KN()
{

}

void KN::train(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int numClasses){

    this->cvKn = new KNearest();

    cv::Mat trainLabels = Classifier::cr8ResponseMat(labels,trainData.rows);

    this->cvKn->train(trainData,trainLabels);
}

std::vector<uchar> KN::predict(cv::Mat_<float> &testData){
    std::vector<uchar> responses;

    for(int i = 0; i < testData.rows; ++i){
        float response = this->cvKn->find_nearest(testData.row(i),6);
        responses.push_back(response);
    }


    return responses;
}

uchar KN::predictResponse(cv::Mat_<float> &testData){
    return 0;
}

void KN::showGraph(int featuresNum){

}


int KN::save2file(const char *filename){
    return -1;
}

int KN::loadFromFile(const char *filename){
    return -1;
}

