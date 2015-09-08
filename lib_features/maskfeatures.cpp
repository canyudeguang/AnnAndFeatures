#include "maskfeatures.h"


string maskfeatures::name(){
    return "fMask";
}

cv::Mat_<float> maskfeatures::getFeature(Mat &image){

    Mat_<float> Features(1,3);

    uint8_t* pixelPtr = (uint8_t*)image.data;
    int cn = image.channels();
    Scalar_<uint8_t> bgrPixel;

    int avgG = 0, avgB = 0, avgR = 0, count = 0;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            bgrPixel.val[0] = pixelPtr[i*image.cols*cn + j*cn + 0]; // B
            bgrPixel.val[1] = pixelPtr[i*image.cols*cn + j*cn + 1]; // G
            bgrPixel.val[2] = pixelPtr[i*image.cols*cn + j*cn + 2]; // R

            if(bgrPixel.val[0] != 0 && bgrPixel.val[1] != 255 && bgrPixel.val[2] != 0){
                count++;
                avgB += bgrPixel.val[0];
                avgG += bgrPixel.val[1];
                avgR += bgrPixel.val[2];
            }
            // do something with BGR values...
        }
    }

    Features(0,0) = avgB/count;
    Features(0,1) = avgG/count;
    Features(0,2) = avgR/count;

    return Features;
}


maskfeatures::maskfeatures()
{

}

maskfeatures::~maskfeatures()
{

}

