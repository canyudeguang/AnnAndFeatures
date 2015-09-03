#include "edgefeatures.h"

string EdgeFeatures::name(){
    return "EdgeFeatures";
}

Mat_<float> EdgeFeatures::getFeature(Mat &image)
{
    Mat_<float> Features(1,8);
    // Init all images of the class
    initImgs(image);

    // Reduce noise with a kernel
    blur(img_tmp,img_tmp,Size(5,5));
    // Canny detector
    //===============================================================================
    Mat detected_edges;
    int lowThres = 100;
    int ratio = 3;
    int kernelSize = 5;
    Canny(img_tmp, detected_edges, lowThres, lowThres*ratio, kernelSize);

    Point p[8] = {Point(0,0), Point(image.cols, image.rows),
                  Point(image.cols, 0), Point(0,image.rows),
                  Point(image.cols/2, 0), Point(image.cols/2,image.rows),
                  Point(0, image.rows/2), Point(image.cols,image.rows/2)};


    for(int i = 0; i < 8; i+=2){
        vector<uchar> line = cvSupport::pixsUnderLine(detected_edges,p[i],p[i+1]);
        int numOfSeg = cvSupport::numOfSegments(line);
        int numOfOccur = cvSupport::numberOf(line,255);
        //FEATURES
        Features(0,i) = numOfSeg;// Number of Segments
        if(numOfOccur == 0){
            Features(0,i+1) = 0;
        }
        else{
           Features(0,i+1) = numOfOccur/(float)numOfSeg; // AverageSegmentLenght
        }

        //@TODO - average
    }



    return Features;
}


void EdgeFeatures::showAll(){
    imshow("Original", this->img_orig);
    imshow("Temp", this->img_tmp);
    imshow("GrayScale", this->img_gray_scale);
    while( (cvWaitKey(0) != 'q') ){
        continue;
    }
}

void EdgeFeatures::initImgs(Mat &image){
    image.copyTo(this->img_orig);
    image.copyTo(this->img_tmp);
    cvtColor(image,this->img_gray_scale,CV_BGR2GRAY);
}
