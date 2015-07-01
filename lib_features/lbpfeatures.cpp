#include "lbpfeatures.h"

LBPFeatures::LBPFeatures()
{
    this->imgSize = cv::Size(DEF_WIDTH, DEF_HEIGHT);
}

LBPFeatures::~LBPFeatures()
{

}

void LBPFeatures::setSize(int width, int height){
    this->imgSize = cv::Size(width, height);
}

unsigned long LBPFeatures::NeighBits(int neighRadius, Mat &src, int _x, int _y){

    uchar value = src.at<uchar>(_y,_x);
\
    unsigned long code = 0;
    int cnt = 0;
    for(int Y = _y - neighRadius; Y <= _y + neighRadius; ++Y){
        for( int X = _x - neighRadius; X <= _x + neighRadius; ++X){
            if(X == _x && Y == _y){
                //cout << "xXy" << ",";
            }
            else{
                //cout << X << "x" << Y << ",";

                code |= (src.at<uchar>(Y,X) > value) << cnt;
                ++cnt;
            }

        }
    }


    return code;

}

void LBPFeatures::Lbp8(Mat &src, Mat &dst, int neighRadius){


    // Create a new image minus neighboring radius (1 = 8 neighborhood)
    // neightRadius * 8 - pixel count + 1 the center
    if(src.rows - neighRadius*2 > 0 && src.cols - neighRadius*2 > 0){
        dst = Mat::zeros(src.rows-neighRadius*2, src.cols-neighRadius*2, CV_8UC1);

        for(int y = neighRadius; y < src.rows-neighRadius; ++y){
            for(int x = neighRadius; x < src.cols-neighRadius; ++x){
                unsigned long binaryCode = LBPFeatures::NeighBits(neighRadius, src, x, y);

                dst.at<uchar>(y-neighRadius,x-neighRadius) = binaryCode;
             }
        }
    }

}

void LBPFeatures::Lbp32(Mat &src, Mat &dst, int neighRadius){


    // Create a new image minus neighboring radius (1 = 8 neighborhood)
    // neightRadius * 8 - pixel count + 1 the center
    if(src.rows - neighRadius*2 > 0 && src.cols - neighRadius*2 > 0){
        dst = Mat::zeros(src.rows-neighRadius*2, src.cols-neighRadius*2, CV_32SC1);

        for(int y = neighRadius; y < src.rows-neighRadius; ++y){
            for(int x = neighRadius; x < src.cols-neighRadius; ++x){
                unsigned long binaryCode = LBPFeatures::NeighBits(neighRadius, src, x, y);

                dst.at<uint>(y-neighRadius,x-neighRadius) = binaryCode;
            }
        }
    }
}

void LBPFeatures::Lbp64(Mat &src, Mat &dst, int neighRadius){


    // Create a new image minus neighboring radius (1 = 8 neighborhood)
    // neightRadius * 8 - pixel count + 1 the center
    if(src.rows - neighRadius*2 > 0 && src.cols - neighRadius*2 > 0){
        dst = Mat::zeros(src.rows-neighRadius*2, src.cols-neighRadius*2, CV_64FC1);

        for(int y = neighRadius; y < src.rows-neighRadius; ++y){
            for(int x = neighRadius; x < src.cols-neighRadius; ++x){
                unsigned long binaryCode = LBPFeatures::NeighBits(neighRadius, src, x, y);

                dst.at<float>(y-neighRadius,x-neighRadius) = binaryCode;
            }
        }
    }
}

cv::Mat_<float> LBPFeatures::getFeature(Mat &image){

    cvtColor(image,this->_ImgGray,CV_BGR2GRAY);

    resize(this->_ImgGray, this->_ImgGray, this->imgSize);

    cv::Mat dst;
    Lbp8(_ImgGray,dst);

    imshow("c",dst);


    cv::Mat_<float> features;
    features = dst.reshape(1,1);
    return features;
}

