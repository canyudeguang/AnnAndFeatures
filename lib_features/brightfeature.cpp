#include "brightfeature.h"

BrightFeature::BrightFeature()
{

}

string BrightFeature::name(){
    return "BrightFeature";
}

BrightFeature::~BrightFeature()
{

}


std::vector<cv::Point> BrightFeature::findBrightest(Mat &img){
    Mat_<uchar> gray(img);
    int brightest = 0;
    for(int x = 0; x < gray.cols; ++x){
        for(int y = 0; y < gray.rows; ++y){
            int pix = gray(y,x);
            if(pix > brightest ){
                brightest = pix;
            }
        }
    }
    for(int x = 0; x < gray.cols; ++x){
        for(int y = 0; y < gray.rows; ++y){
            int pix = gray(y,x);
            if(pix == brightest ){
                this->bPoints.push_back(cv::Point(x,y));
            }
        }
    }
    return this->bPoints;
}

void BrightFeature::showBrightest(Mat &img){

    Mat copy;
    img.copyTo(copy);
    for(int i = 0; i < this->bPoints.size(); ++i){
        circle(copy,this->bPoints[i],1,Scalar(0,0,255));
    }
    imshow("brightest",copy);
}

cv::Mat_<float> BrightFeature::getFeature(Mat &image){


    //@TODO prvni udelat vsude stejnou svetelnost...


    cv::Rect roi = Rect(image.cols/2 - image.cols/4,image.rows/2 - image.rows/4, image.cols/2, image.rows/2);

    cv::Mat roiImg = image(roi);

    cv::Mat gray;
    cvtColor(roiImg,gray, CV_BGR2GRAY);
    //equalizeHist(gray,gray);

//    cv::Mat out;
//    int th = threshold(gray, out, 0, 255, CV_THRESH_OTSU);
//    cout << th << endl;
//    Canny(gray,gray,th,0);
//    resize(gray,gray,Size(128,64));
//    imshow("a",gray);
//    blur(roiImg,roiImg,Size(5,5));

    Mat Dx,Dy;
    Sobel(gray,Dx,CV_64F,1,0,3);
    Sobel(gray,Dy,CV_64F,0,1,3);
    //findBrightest(gray);

    resize(Dx,Dx,Size(32,16));
    resize(Dy,Dy,Size(32,16));



    Dy.push_back(Dx);
    Dy = Dy.reshape(0,1);

   // showBrightest(roiImg);



//    this->bPoints.clear();


    cv::Mat_<float> Features;
    Features.push_back(Dy);


    return Features;
}
