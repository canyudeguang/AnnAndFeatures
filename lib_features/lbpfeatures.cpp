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

cv::Mat_<float> LBPFeatures::showPeaks(Mat &src, Mat &dst, int thres){
    cvtColor(src,dst,CV_GRAY2BGR);

    int erosion_type;
    int erosion_elem = 0;
    int erosion_size = 2;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement( erosion_type,
                                           Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                           Point( erosion_size, erosion_size ) );



    Mat source_edited;
    src.copyTo(source_edited);


     erode(source_edited,source_edited,element);
     //dilate(source_edited,source_edited,element);



     Mat_<float> features(1,256);
     features.setTo(0);

    // Values are: 0, 1, 2, 3, 4, 6, 7, 8, 9, 11, 15, 16, 20, 22, 23, 31, 32, 33, 35, 40, 41, 43,
    // 64, 96, 104, 105,
    // 128, 144, 148, 150, 192, 208, 212, 224, 232, 240
    for(int y = 0; y < src.rows; ++y){
        for(int x = 0; x < src.cols; ++x){
            uchar pix = source_edited.at<uchar>(y,x);
            features(0,pix)++;
            if(pix >= 0 && pix <= 43){
                circle(dst,Point(x,y),1,Scalar(255,0,0));
            }
            if(pix >= 64 && pix <= 105){
                circle(dst,Point(x,y),1,Scalar(0,255,0));
            }

            if(pix >= 128 && pix <= 240){
                circle(dst,Point(x,y),1,Scalar(0,0,255));
            }
        }
    }


    imshow("Blur",source_edited);
    moveWindow("Blur",(this->imgSize.width + 5)*2,(this->imgSize.height + 60));

    return features;
}

cv::Mat_<float> LBPFeatures::getFeature(Mat &image){
    // Convert to GrayScale
    cvtColor(image,this->_ImgGray,CV_BGR2GRAY);

    // Blur and Equalize histogram
    blur(this->_ImgGray,this->_ImgGray,cv::Size(9,9));
    equalizeHist(this->_ImgGray,this->_ImgGray);
    // Resize to the scalable size
    resize(this->_ImgGray, this->_ImgGray, this->imgSize);

    // Perform LBP
    cv::Mat dst;
    Lbp8(_ImgGray,dst);
    // Blur again

    cv::Mat peaks;
     showPeaks(dst,peaks,200);


    imshow("LBP",dst);
    imshow("Original",this->_ImgGray);
    imshow("PEAKS",peaks);

    moveWindow("Original",0,0);
    moveWindow("LBP",this->imgSize.width + 5,0);
    moveWindow("PEAKS",(this->imgSize.width + 5)*2,0);

    cv::Mat_<float> features;
    features = dst.reshape(1,1);
    return features;
}

