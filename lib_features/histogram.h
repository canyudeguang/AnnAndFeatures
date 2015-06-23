#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/*****************************************************************************************
C++/C includes
*****************************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

/*****************************************************************************************
Open CV Includes
*****************************************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


#define NUM_OF_HISTS 5

// Three byte values e.g RGB, HSV, HSL... where b1 is R, b2 is G etc.
class Three{
    public:
        Three(int b1,int b2,int b3);
        int b[3];
        // ammount of pixels with b1 value
        // for one bint bin of histogram

        friend ostream& operator<<(ostream& os, const Three& threes);
};

class Threes{
    public:
    // adds value into vector of Threes
    void addValue(int b1, int b2, int b3); // add one 3byte value
    int lowestIndex(int byte); // returns index of the lowest ammount of pixels the specific bin
    int highestIndex(int byte);  // returns index of the highest ammount of pixels the specific bin

    int tail(int n, int byte); // returns pixelcount of n last bins of certain byte
    int front(int n, int byte); // returns pixelcount of n first bins of certain byte

    friend ostream& operator<<(ostream& os, const Threes& threes);

    vector<Three> featureVec; // defacto histogram values in one vector of Threes
};


class Histogram
{
public:
    Histogram();
    Histogram(string file, int bins);
    void loadImg(string file, int bins);
    Histogram(Mat image, int bins);

    void histGrey(int isshow = 0);
    void histColor(int isshow = 0);
    void histHSL(int isshow = 0);
    void histHSV(int isshow = 0);

    Mat histColorImg;
    Mat srcColor;

    Mat resHistClr;
    Mat resHistInt;
    Mat resHistHSL;
    Mat resHistHSV;

    bool isSrc;
    string imName;

    Threes ColorHistogramValues;

    vector<float> getFeatureHisto();

    string HISTOGRAM_NAME_WINDOW_HISTOGRAM_COLOR;
    string HISTOGRAM_NAME_WINDOW_HISTOGRAM;
    string HISTOGRAM_NAME_WINDOW_IMAGE;


    // image Mat for each histogram
    // values for each histogram
    // There are 5 types RGB, HSL, HSV and full variants of the last two mentioned.
    Mat HistogramImg[NUM_OF_HISTS];
    Threes HistogramValues[NUM_OF_HISTS];
    void HistogramCalc(string imgpath, int type, int bins, int isShow = 0);
    // _FULL is higher H  but range seems also to 255,128,255 _FULL ranges
    // HSV ranges = 180, 255, 255
    enum {HISTO_TYPE_RGB, HISTO_TYPE_HSV, HISTO_TYPE_HSL, HISTO_TYPE_HSV_FULL, HISTO_TYPE_HSL_FULL};


    Mat_<float> getRGBfeatures();

    cv::Mat showHistogram(Mat_<float> &histogram);


private:
    vector<float> R,G,B;
    string HIST_NAME[NUM_OF_HISTS];
    void defineWindowsNames();
    Mat srcGray;

    int histSize; // number of bins
    vector<float> histValues;



};

#endif // HISTOGRAM_H
