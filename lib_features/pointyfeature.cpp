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

cv::Mat_<float> PointyFeature::mouthFeature(Mat &image){
    image.copyTo(this->_img);
    this->showStasm("stasm");

    // mouth Inner from left to right upper 68,67,66

//    circle(image,_vecStasmLandmarks[68],2,Scalar(0,0,255));
//    circle(image,_vecStasmLandmarks[67],2,Scalar(0,255,0));
//    circle(image,_vecStasmLandmarks[66],2,Scalar(255,0,0));
//    // bottum 69,70,71 from left to right
//    circle(image,_vecStasmLandmarks[69],2,Scalar(255,0,0));
//    circle(image,_vecStasmLandmarks[70],2,Scalar(0,255,0));
//    circle(image,_vecStasmLandmarks[71],2,Scalar(0,0,255));

    double avgEyeDist = this->avgEyeDist();
    double o1,o2,o3;
    o1 = cv::norm(this->_vecStasmLandmarks[68] - this->_vecStasmLandmarks[69]);
    o2 = cv::norm(this->_vecStasmLandmarks[67] - this->_vecStasmLandmarks[70]);
    o3 = cv::norm(this->_vecStasmLandmarks[66] - this->_vecStasmLandmarks[71]);

    //cout <<  o1 << " " << o2 << " " << o3 << endl;

    Mat_<float> Features(1,3);
    Features(0,0) = o1/avgEyeDist*100;
    Features(0,1) = o2/avgEyeDist*100;
    Features(0,2) = o3/avgEyeDist*100;
    return Features;
}

cv::Mat_<float> PointyFeature::LeyeFeature(Mat &image){
    image.copyTo(this->_img);
    this->showStasm("stasm");

    // mouth Inner from left to right upper 68,67,66

//    circle(image,_vecStasmLandmarks[31],2,Scalar(0,0,255));
//    circle(image,_vecStasmLandmarks[32],2,Scalar(0,255,0));
//    circle(image,_vecStasmLandmarks[33],2,Scalar(255,0,0));
//    // bottum 69,70,71 from left to right
//    circle(image,_vecStasmLandmarks[35],2,Scalar(255,0,0));
//    circle(image,_vecStasmLandmarks[36],2,Scalar(0,255,0));
//    circle(image,_vecStasmLandmarks[37],2,Scalar(0,0,255));


    double avgEyeDist = this->avgEyeDist();
    double o1,o2,o3;
    o1 = cv::norm(this->_vecStasmLandmarks[31] - this->_vecStasmLandmarks[35]);
    o2 = cv::norm(this->_vecStasmLandmarks[32] - this->_vecStasmLandmarks[36]);
    o3 = cv::norm(this->_vecStasmLandmarks[33] - this->_vecStasmLandmarks[37]);

    //cout <<  o1 << " " << o2 << " " << o3 << endl;

    Mat_<float> Features(1,3);
    Features(0,0) = o1/avgEyeDist*100;
    Features(0,1) = o2/avgEyeDist*100;
    Features(0,2) = o3/avgEyeDist*100;
    return Features;
}

cv::Mat_<float> PointyFeature::ReyeFeature(Mat &image){
    image.copyTo(this->_img);
    this->showStasm("stasm");

    // mouth Inner from left to right upper 68,67,66

//    circle(image,_vecStasmLandmarks[41],2,Scalar(0,0,255));
//    circle(image,_vecStasmLandmarks[42],2,Scalar(0,255,0));
//    circle(image,_vecStasmLandmarks[43],2,Scalar(255,0,0));
//    // bottum 69,70,71 from left to right
//    circle(image,_vecStasmLandmarks[45],2,Scalar(255,0,0));
//    circle(image,_vecStasmLandmarks[46],2,Scalar(0,255,0));
//    circle(image,_vecStasmLandmarks[47],2,Scalar(0,0,255));


    double avgEyeDist = this->avgEyeDist();
    double o1,o2,o3;
    o1 = cv::norm(this->_vecStasmLandmarks[41] - this->_vecStasmLandmarks[45]);
    o2 = cv::norm(this->_vecStasmLandmarks[42] - this->_vecStasmLandmarks[46]);
    o3 = cv::norm(this->_vecStasmLandmarks[43] - this->_vecStasmLandmarks[47]);

    //cout <<  o1 << " " << o2 << " " << o3 << endl;

    Mat_<float> Features(1,3);
    Features(0,0) = o1/avgEyeDist*100;
    Features(0,1) = o2/avgEyeDist*100;
    Features(0,2) = o3/avgEyeDist*100;
    return Features;
}

cv::Mat_<float> PointyFeature::smileFeature(Mat &image){
    image.copyTo(this->_img);
    this->showStasm("stasm");

    // mouth Inner from left to right upper 68,67,66

    circle(image,_vecStasmLandmarks[59],2,Scalar(0,0,255));
    circle(image,_vecStasmLandmarks[65],2,Scalar(0,255,0));
    circle(image,_vecStasmLandmarks[74],2,Scalar(255,0,0));
    // bottum 69,70,71 from left to right
    circle(image,_vecStasmLandmarks[56],2,Scalar(255,0,0));
   // circle(image,_vecStasmLandmarks[46],2,Scalar(0,255,0));
   // circle(image,_vecStasmLandmarks[47],2,Scalar(0,0,255));


    double avgEyeDist = this->avgEyeDist();
    double o1,o2,o3;
    o1 = cv::norm(this->_vecStasmLandmarks[56] - this->_vecStasmLandmarks[59]);
    o2 = cv::norm(this->_vecStasmLandmarks[56] - this->_vecStasmLandmarks[65]);
    o3 = cv::norm(this->_vecStasmLandmarks[56] - this->_vecStasmLandmarks[74]);

    //cout <<  o1 << " " << o2 << " " << o3 << endl;

    Mat_<float> Features(1,3);
    Features(0,0) = o1/avgEyeDist*100;
    Features(0,1) = o2/avgEyeDist*100;
    Features(0,2) = o3/avgEyeDist*100;
    return Features;
}


cv::Mat_<float> PointyFeature::getFeature(Mat &image){
    Mat_<float> Features = this->smileFeature(image);
    return Features;
}
