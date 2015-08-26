#include "integralfeature.h"


/**
** Integral Rectangle
** *************************************************************************************
*/
IntegralRect::IntegralRect(cv::Point tl, cv::Point br, bool isWhite){
    this->A = cv::Point(tl.x, tl.y);
    this->B = cv::Point(br.x, tl.y);
    this->C = cv::Point(tl.x, br.y);
    this->D = cv::Point(br.x, br.y);
    this->isWhite = isWhite;
}

int IntegralRect::calcIntegral(Mat &integralImage){
    int a = integralImage.at<int>(A.y, A.x);
    int b = integralImage.at<int>(B.y, B.x);
    int c = integralImage.at<int>(C.y, C.x);
    int d = integralImage.at<int>(D.y, D.x);

    int Isum = a-b-c+d;
    return Isum;
}

void IntegralRect::show(Mat &img){
    cv::Mat copy;
    img.copyTo(copy);
    Rect r = Rect(A.x, A.y, B.x-A.x, D.y-B.y);
    if(this->isWhite){
        rectangle(copy,r,cv::Scalar(255,255,255),2);
    }
    else{
        rectangle(copy,r,cv::Scalar(0,0,0),2);
    }

    imshow("Feature",copy);
}
void IntegralRect::imprint(Mat &img){
    Rect r = Rect(A.x, A.y, B.x-A.x, D.y-B.y);
    if(this->isWhite){
        rectangle(img,r,cv::Scalar(255,255,255),2);
    }
    else{
        rectangle(img,r,cv::Scalar(0,0,0),2);
    }
}

/**
** Integral Feature - packed rectangles
** *************************************************************************************
*/
IntegralF::IntegralF(cv::Point tlW, cv::Point brW, cv::Point tlB, cv::Point brB){
    this->_tlW = tlW;
    this->_brW = brW;
    this->_tlB = tlB;
    this->_brB = brB;
}

// Based on the image and type
//
IntegralF::IntegralF(Mat &img, int type){
    switch(type){
    case IntegralF::Edge:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols,img.rows/2),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows/2),Point(img.cols,img.rows),false));
        break;
    }
    case IntegralF::Edges:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols,img.rows/3),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols*2.3,img.rows*2/3),false));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols,img.rows),true));
        break;
    }
    case IntegralF::Cross:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols/3,img.rows/3),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows*2/3),Point(img.cols,img.rows),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols/3,img.rows),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,0),Point(img.cols,img.rows/3),true));

        this->Blacks.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols,img.rows*2/3),false));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,0),Point(img.cols*2/3,img.rows/3),false));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows*2/3),Point(img.cols*2/3,img.rows),false));
        break;
    }
    case IntegralF::Centroid:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols,img.rows/3),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols,img.rows),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols/3,img.rows*2/3),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows/3),Point(img.cols,img.rows*2/3),true));

        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows/3),Point(img.cols*2/3,img.rows*2/3),false));
        break;
    }
    default:{
        cout << "Other type: " << type << endl;
        break;
    }
    }
}

void IntegralF::showFeature(Mat &image, IntegralF &feature){
    cv::Mat copy;
    image.copyTo(copy);

    imshow("Feature",copy);
}
void IntegralF::showFeature(Mat &image){
    cv::Mat copy;
    image.copyTo(copy);
    for(int i = 0; i < Whites.size(); ++i){
        Whites[i].imprint(copy);
    }
    for(int i = 0; i < Blacks.size(); ++i){
        Blacks[i].imprint(copy);
    }
    imshow("Feature",copy);
}

int IntegralF::calcFromIntegralImage(Mat &integralImage){
    int whiteSum = 0;
    int blackSum = 0;
    for(int i = 0; i < Whites.size(); ++i){
        whiteSum += Whites[i].calcIntegral(integralImage);
    }
    for(int i = 0; i < Blacks.size(); ++i){
        blackSum+= Blacks[i].calcIntegral(integralImage);
    }

    return whiteSum - blackSum;
}

IntegralFeature::IntegralFeature()
{

}

IntegralFeature::~IntegralFeature()
{

}


int IntegralFeature::IntegerSum(cv::Mat &intImage, Point a, Point b ){
    int iB = intImage.at<int>(b.y,b.x);
    int iA = intImage.at<int>(a.y,a.x);
    int res = iB - iA;
    //cout << iB << " - " << iA << " = " << res << endl;
    return res;
}

cv::Mat_<float> IntegralFeature::getFeature(Mat &image){

    cv::Mat integralImage, grscale;
    cvtColor(image,grscale,CV_BGR2GRAY);
    integral(grscale,integralImage);

    IntegralF edges(image,IntegralF::Edges);
    IntegralF cross(image,IntegralF::Cross);
    IntegralF center(image,IntegralF::Centroid);

    cross.showFeature(image);


    cv::Mat_<float> Features(1,3);
    Features(0,0) = edges.calcFromIntegralImage(integralImage);
    Features(0,1) = cross.calcFromIntegralImage(integralImage);
    Features(0,2) = center.calcFromIntegralImage(integralImage);


    return Features;
}
