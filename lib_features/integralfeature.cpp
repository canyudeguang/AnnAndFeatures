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
IntegralF::IntegralF(cv::Point tlW, cv::Point brW, cv::Point tlB, cv::Point brB){
    this->_tlW = tlW;
    this->_brW = brW;
    this->_tlB = tlB;
    this->_brB = brB;

    this->Whites.push_back(IntegralRect(tlW,brW,true));
    this->Blacks.push_back(IntegralRect(tlB,brB,false));

    this->sumBlack = 0;
    this->sumWhite = 0;
//    this->sumBlacks(img);
//    this->sumWhites(img);

}

// Based on the image and type
//
IntegralF::IntegralF(Mat &img, int type){

    this->sumBlack = 0;
    this->sumWhite = 0;
    switch(type){
    case IntegralF::EdgeVertical:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols/2, img.rows-1),true));
        this->Blacks.push_back(IntegralRect(Point(img.cols/2,0),Point(img.cols-1,img.rows-1),false));
        break;
    }
    case IntegralF::EdgeHorizontal:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows/2),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows/2),Point(img.cols-1,img.rows-1),false));
        break;
    }
    case IntegralF::LineHorizontal:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows/3),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols*2.3,img.rows*2/3),false));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols-1,img.rows-1),true));
        break;
    }
    case IntegralF::LineHorizontalThick:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows*0.2),true));
        this->Blacks.push_back(IntegralRect(Point(0,img.rows*0.2),Point(img.cols-1,img.rows*0.8),false));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*0.8),Point(img.cols-1,img.rows-1),true));
        break;
    }
    case IntegralF::LineVertical:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols/3,img.rows-1),true));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,0),Point(img.cols*2/3,img.rows-1),false));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,0),Point(img.cols-1,img.rows-1),true));
        break;
    }
    case IntegralF::LineVerticalThick:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols*0.2,img.rows-1),true));
        this->Blacks.push_back(IntegralRect(Point(img.cols*0.2,0),Point(img.cols*0.8,img.rows-1),false));
        this->Whites.push_back(IntegralRect(Point(img.cols*0.8,0),Point(img.cols-1,img.rows-1),true));
        break;
    }
    case IntegralF::CenterDot10:{
        cv::Point center(img.cols/2, img.rows/2);
        int d = img.rows/10; // diameter
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d),Point(center.x + d, center.y + d),false));
        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,0),Point(center.x+d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,center.y+d),Point(center.x+d,img.rows-1),true));
        break;
    }
    case IntegralF::CenterDot6:{
        cv::Point center(img.cols/2, img.rows/2);
        int d = img.rows/6; // diameter
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d),Point(center.x + d, center.y + d),false));
        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,0),Point(center.x+d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,center.y+d),Point(center.x+d,img.rows-1),true));
        break;
    }
    case IntegralF::CenterDot5:{
        cv::Point center(img.cols/2, img.rows/2);
        int d = img.rows/5; // diameter
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d),Point(center.x + d, center.y + d),false));
        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,0),Point(center.x+d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,center.y+d),Point(center.x+d,img.rows-1),true));
        break;
    }
    case IntegralF::CenterDot3:{
        cv::Point center(img.cols/2, img.rows/2);
        int d = img.rows/3; // diameter
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d),Point(center.x + d, center.y + d),false));
        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,0),Point(center.x+d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,center.y+d),Point(center.x+d,img.rows-1),true));
        break;
    }
    case IntegralF::CenterDotLower:{
        int d = img.rows/5; // diameter
        int off = img.rows/10;
        cv::Point center(img.cols/2, img.rows/2+off);
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d),Point(center.x + d, center.y + d),false));
        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,0),Point(center.x+d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,center.y+d),Point(center.x+d,img.rows-1),true));
        break;
    }
    case IntegralF::CenterDotLow:{
        int d = img.rows/5; // diameter
        int off = img.rows/3;
        cv::Point center(img.cols/2, img.rows/2+off);
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d),Point(center.x + d, img.rows-1),false));
        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x-d,0),Point(center.x+d,center.y-d),true));
        break;
    }

    case IntegralF::CenterCrossRect:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols/3,img.rows/3),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows*2/3),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols/3,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,0),Point(img.cols-1,img.rows/3),true));

        this->Blacks.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols-1,img.rows*2/3),false));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,0),Point(img.cols*2/3,img.rows/3),false));
        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows*2/3),Point(img.cols*2/3,img.rows-1),false));
        break;
    }
    case IntegralF::CenterRect:{
        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows/3),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols-1,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows/3),Point(img.cols/3,img.rows*2/3),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows/3),Point(img.cols,img.rows*2/3),true));

        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows/3),Point(img.cols*2/3,img.rows*2/3),false));
        break;
    }
    case IntegralF::LowRect:{

        this->Whites.push_back(IntegralRect(Point(0,0),Point(img.cols-1,img.rows*2/3),true));
        this->Whites.push_back(IntegralRect(Point(0,img.rows*2/3),Point(img.cols/3,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(img.cols*2/3,img.rows*2/3),Point(img.cols-1,img.rows-1),true));

        this->Blacks.push_back(IntegralRect(Point(img.cols/3,img.rows*2/3),Point(img.cols*2/3,img.rows-1),false));
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
    case IntegralF::CenterCross:{
        cv::Point center(img.cols/2, img.rows/2);
        int d = img.rows/10; // diameter
        this->Blacks.push_back(IntegralRect(Point(center.x - d*3, center.y - d),Point(center.x + d*3, center.y + d),false));
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d*3),Point(center.x + d, center.y - d),false));
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y + d),Point(center.x + d, center.y + d*3),false));

        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1, center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(0,center.y+d),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d, center.y+d),Point(img.cols-1, img.rows-1),true));

        break;
    }
    case IntegralF::CenterCrossFull:{
        cv::Point center(img.cols/2, img.rows/2);
        int d = img.rows/10; // diameter
        this->Blacks.push_back(IntegralRect(Point(center.x - d*3, center.y - d),Point(center.x + d*3, center.y + d),false));
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y - d*3),Point(center.x + d, center.y - d),false));
        this->Blacks.push_back(IntegralRect(Point(center.x - d, center.y + d),Point(center.x + d, center.y + d*3),false));

        this->Whites.push_back(IntegralRect(Point(0,0),Point(center.x-d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d,0),Point(img.cols-1, center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(0,center.y+d),Point(center.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(center.x+d, center.y+d),Point(img.cols-1, img.rows-1),true));

        this->Whites.push_back(IntegralRect(Point(center.x - d, 0),Point(center.x+d,center.y-d),true));
        this->Whites.push_back(IntegralRect(Point(center.x - d, center.y + d),Point(center.x + d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(0, center.y - d),Point(center.x-d*3,center.y+d),true));
        this->Whites.push_back(IntegralRect(Point(center.x +d*3 , center.y-d),Point(img.cols-1,center.y+d),true));

        break;
    }
    case IntegralF::MouthHoles:{
        cv::Point centerL(img.cols*0.23, img.rows*0.5);
        cv::Point centerR(img.cols*0.77, img.rows*0.5);
        int d = img.rows/7; // diameter
        this->Blacks.push_back(IntegralRect(Point(centerL.x - d, centerL.y - d),Point(centerL.x + d, centerL.y + d),false));
        this->Blacks.push_back(IntegralRect(Point(centerR.x - d, centerR.y - d),Point(centerR.x + d, centerR.y + d),false));

        this->Whites.push_back(IntegralRect(Point(0.0),Point(centerL.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(centerL.x + d, 0),Point(centerR.x-d,img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(centerR.x + d, 0),Point(img.cols-1,img.rows-1),true));

        this->Whites.push_back(IntegralRect(Point(centerL.x - d, 0),Point(centerL.x + d, centerL.y - d),true));
        this->Whites.push_back(IntegralRect(Point(centerL.x - d, centerL.y + d),Point(centerL.x + d, img.rows-1),true));
        this->Whites.push_back(IntegralRect(Point(centerR.x - d, 0),Point(centerR.x + d, centerR.y - d),true));
        this->Whites.push_back(IntegralRect(Point(centerR.x - d, centerR.y + d),Point(centerR.x + d, img.rows-1),true));

        break;
    }
    default:{
        cout << "Other type: " << type << endl;
        break;
    }
    }

//    this->sumBlacks(img);
//    this->sumWhites(img);
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

IntegralFeature::~IntegralFeature()
{

}

IntegralFeature::IntegralFeature(IntegralFeature::CLASS_TYPES clasType)
{
    // exhibition:
    // For teeth the best is MouthHoles,LowRect,CetnerDotLower,CenterDot3,CenterCross,LineHorizontalThic,LineVertical
    // Tonge OK as well

    switch (clasType) {
    case IntegralFeature::CLASS_OPEN:
        this->mName = "fIntegralOp";
        this->cType = CLASS_OPEN;
        break;
    case IntegralFeature::CLASS_TEETH:
        this->mName = "fIntegralTe";
        this->cType = CLASS_TEETH;
        break;
    case IntegralFeature::CLASS_TONGUE:
        this->cType = CLASS_TONGUE;
        this->mName = "fIntegralTo";
        break;
    case IntegralFeature::CLASS_SMILE:
        this->cType = CLASS_SMILE;
        this->mName = "fIntegralSm";
        break;
    default:
        this->mName = "fIntegral";
        this->cType = CLASS_DEFAULT;
        break;
    }
}


int IntegralFeature::IntegerSum(cv::Mat &intImage, Point a, Point b ){
    int iB = intImage.at<int>(b.y,b.x);
    int iA = intImage.at<int>(a.y,a.x);
    int res = iB - iA;
    //cout << iB << " - " << iA << " = " << res << endl;
    return res;
}

void IntegralFeature::calcIntegralFeatures(){
    switch(this->cType){
    case CLASS_OPEN:
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot3));
        break;
    case CLASS_TEETH:
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LowRect));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::MouthHoles));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDotLower));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot3));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineHorizontal));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineHorizontalThick));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineVertical));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterCross));
        break;
    case CLASS_TONGUE:
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LowRect));
        break;

    case CLASS_SMILE:
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LowRect));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::MouthHoles));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDotLower));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot3));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineHorizontal));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineVertical));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterCross));
        break;
    default:
        integralShapes.push_back(IntegralF(integralImage,IntegralF::EdgeVertical));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::EdgeHorizontal));
     integralShapes.push_back(IntegralF(integralImage,IntegralF::LineHorizontal));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineHorizontalThick));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineVertical));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LineVerticalThick));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterCross));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterCrossFull));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterCrossRect));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot3));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot5));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot6));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDot10));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDotLow));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterDotLower));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterMouth));
       integralShapes.push_back(IntegralF(integralImage,IntegralF::CenterRect));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::LowRect));
        integralShapes.push_back(IntegralF(integralImage,IntegralF::MouthHoles));
        break;
    }
}

cv::Mat_<float> IntegralFeature::getFeature(Mat &image){

    cv::Mat grscale;
    cvtColor(image,grscale,CV_BGR2GRAY);
    integral(grscale,this->integralImage);

    calcIntegralFeatures();

//    imshow("integral",integralImage);
//    imshow("grs",grscale);
//    cvWaitKey(0);
    cv::Mat_<float> Features(1,integralShapes.size()*2);

    for(int i = 0; i < integralShapes.size(); ++i){
        Features(0,2*i) = integralShapes[i].sumWhites(this->integralImage);
        Features(0,2*i + 1) = integralShapes[i].sumBlacks(this->integralImage);
    }

    //cleanup
    this->integralShapes.clear();

    return Features;
}


string IntegralFeature::name(){
    return this->mName;
}
