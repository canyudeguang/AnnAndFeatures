#include "pointyfeature.h"


cv::Point PointyFeature::get_point_landmark(int index, float * landmarks){
    return cv::Point(landmarks[index*2],landmarks[index*2+1]);
}


void PointyFeature::showStasm(string name){
        Mat model;
        this->_img.copyTo(model);

        // Jaw
        for(int i = 0; i < 12; ++i){
            circle(model, this->_vecStasmLandmarks[i] ,1,cv::Scalar(0,100,255),2);
        }
        // Forehead
        for(int i = 12; i < 16; ++i){
            circle(model,this->_vecStasmLandmarks[i],1,cv::Scalar(0,0,255),2);
        }
        // EyeBrows
        for(int i = 16; i < 28; ++i){
            circle(model,this->_vecStasmLandmarks[i],1,cv::Scalar(200,0,255),2);
        }
        // Eyes
        for(int i = 28; i < 48; ++i){
            circle(model,this->_vecStasmLandmarks[i],1,cv::Scalar(255,128,0),2);
        }
        // Nose
        for(int i = 48; i < 59; ++i){
            circle(model,this->_vecStasmLandmarks[i],1,cv::Scalar(50,255,0),2);
        }
        // Mouth
        for(int i = 59; i < stasm_NLANDMARKS; ++i){
            circle(model,this->_vecStasmLandmarks[i],1,cv::Scalar(0,50,255),2);
        }

    imshow(name,model);
}

PointyFeature::PointyFeature()
{

}

PointyFeature::~PointyFeature()
{

}

void PointyFeature::setPoints(vector<cv::Point> vec_landmarks){
    // @TODO copy ?
    this->_vecStasmLandmarks = vec_landmarks;
}


string PointyFeature::name(){
    static const string name = "fPoints";
    return name;
}

double PointyFeature::avgEyeDist(){
    double dLeft, dRight;
    dLeft = cv::norm(this->_vecStasmLandmarks[L_LEyeOuter] - this->_vecStasmLandmarks[L_LEyeInner]);
    dRight = cv::norm(this->_vecStasmLandmarks[L_REyeInner] - this->_vecStasmLandmarks[L_REyeOuter]);

    return double(dLeft + dRight)/2;
}

cv::Mat_<float> PointyFeature::getFeature(Mat &image){
    image.copyTo(this->_img);
    this->showStasm("stasm");

    // mouth Inner from left to right upper 68,67,66

    circle(image,_vecStasmLandmarks[68],2,Scalar(0,0,255));
    circle(image,_vecStasmLandmarks[67],2,Scalar(0,255,0));
    circle(image,_vecStasmLandmarks[66],2,Scalar(255,0,0));
    // bottum 69,70,71 from left to right
    circle(image,_vecStasmLandmarks[69],2,Scalar(255,0,0));
    circle(image,_vecStasmLandmarks[70],2,Scalar(0,255,0));
    circle(image,_vecStasmLandmarks[71],2,Scalar(0,0,255));

    double avgEyeDist = this->avgEyeDist();
    double o1,o2,o3;
    o1 = cv::norm(this->_vecStasmLandmarks[68] - this->_vecStasmLandmarks[69]);
    o2 = cv::norm(this->_vecStasmLandmarks[67] - this->_vecStasmLandmarks[70]);
    o3 = cv::norm(this->_vecStasmLandmarks[66] - this->_vecStasmLandmarks[71]);

    cout <<  o1 << " " << o2 << " " << o3 << endl;

    Mat_<float> Features(1,3);
    Features(0,0) = o1/avgEyeDist;
    Features(0,1) = o2/avgEyeDist;
    Features(0,2) = o3/avgEyeDist;
    return Features;
}
