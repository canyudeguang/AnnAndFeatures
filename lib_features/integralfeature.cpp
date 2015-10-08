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
\
   // cout << a << " " << b << " " << c << " " << d << endl;
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
IntegralF::IntegralF(cv::Mat & img, cv::Point tlW, cv::Point brW, cv::Point tlB, cv::Point brB){
    this->_tlW = tlW;
    this->_brW = brW;
    this->_tlB = tlB;
    this->_brB = brB;

    this->Whites.push_back(IntegralRect(tlW,brW,true));
    this->Blacks.push_back(IntegralRect(tlB,brB,false));

    this->sumBlack = 0;
    this->sumWhite = 0;
    this->sumBlacks(img);
    this->sumWhites(img);

}

// Based on the image and type
//
IntegralF::IntegralF(Mat &img, int type){

    this->sumBlack = 0;
    this->sumWhite = 0;
    switch(type){
    case IntegralF::Edge:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows/2),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows/2),Point(img.cols-1,img.rows-1),false));
        break;
    }
    case IntegralF::Edges:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows/3),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols*2.3,img.rows*2/3),false));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols-1,img.rows-1),true));
        break;
    }
    case IntegralF::Cross:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols/3,img.rows/3),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows*2/3),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols/3,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,0),Point(img.cols-1,img.rows/3),true));

        this->Blacks.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols-1,img.rows*2/3),false));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,0),Point(img.cols*2/3,img.rows/3),false));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows*2/3),Point(img.cols*2/3,img.rows-1),false));
        break;
    }
    case IntegralF::Centroid:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows/3),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols/3,img.rows*2/3),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows/3),Point(img.cols,img.rows*2/3),true));

        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows/3),Point(img.cols*2/3,img.rows*2/3),false));
        break;
    }
    case IntegralF::CenterMouth:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows*0.2),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*0.8),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*0.2),Point(img.cols*0.2,img.rows*0.8),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*0.8,img.rows*0.2),Point(img.cols-1,img.rows*0.8),true));

        this->Blacks.push_back(IntegralRect(Point(img.cols*0.2,img.rows*0.2),Point(img.cols*0.8,img.rows*0.8),false));
        break;
    }
    default:{
        cout << "Other type: " << type << endl;
        break;
    }
    }

    this->sumBlacks(img);
    this->sumWhites(img);
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

int IntegralF::sumBlacks(Mat &integralImage){
    for(int i = 0; i < Blacks.size(); ++i){
        this->sumBlack+= Blacks[i].calcIntegral(integralImage);
    }
    return this->sumBlack;
}

int IntegralF::sumWhites(Mat &integralImage){
    for(int i = 0; i < Whites.size(); ++i){
        this->sumWhite += Whites[i].calcIntegral(integralImage);
    }\
    return this->sumWhite;
}

int IntegralF::calcFromIntegralImage(Mat &integralImage){
    return this->sumWhites(integralImage) - this->sumBlacks(integralImage);
}

float IntegralF::getBlackWhiteRatio(){
    return float(this->sumBlack)/float(this->sumWhite);
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

    imshow("integral",integralImage);
    imshow("grs",grscale);


    IntegralF center(integralImage,IntegralF::CenterMouth);
    IntegralF a(integralImage,IntegralF::Centroid);
    IntegralF b(integralImage,IntegralF::Cross);
    IntegralF c(integralImage,IntegralF::Edge);



    cv::Mat_<float> Features(1,4);
    Features(0,0) = center.sumBlack;
    Features(0,1) = center.sumWhite;
    Features(0,2) = b.sumBlack
    //Features(0,3) = a.getBlackWhiteRatio() * 100;

    return Features;
}
