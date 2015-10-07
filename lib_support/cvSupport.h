#ifndef CV_SUPPORT_H
#define CV_SUPPORT_H

#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "lib_support/support.h"

using namespace std;
using namespace cv;

/**
  Get pixel values under line segment from point A to B in the image.
  */
namespace cvSupport{

class DoublePoint{
public:
    DoublePoint();
    void setL_XY(int x, int y);
    void setR_XY(int x, int y);
    void setPointL_XY(cv::Point left);
    void setPointR_XY(cv::Point right);

    void rotate(Mat m);
    void draw(cv::Mat img, cv::Scalar color);

    cv::Point getLPoint();
    cv::Point getRPoint();

    cv::Point getCenter();

    double distance();
private:
    cv::Point left,right;
};

void onMouse(int event, int x, int y, int flags, void *userData);
class PixUnderCursor{
public:
    PixUnderCursor(string windowName, cv::Mat & img);
    void show_img();
    void print(int x, int y);
private:
    string win_name;
    cv::Mat img;
};

class RGBHash{
public:
    RGBHash(uchar r, uchar g, uchar b);
    RGBHash(cv::Vec3b Vec3bgr);
    uchar R,G,B;
    u_int32_t hash;
    string print();
    string rgbPrint();
    bool operator<(const RGBHash & rgbh);
    double distance(const RGBHash & rgbh);
private:
    void init(uchar r, uchar g, uchar b);
};

class RGBHashCoord : public RGBHash{
public:
    RGBHashCoord(cv::Vec3b Vec3bgr, int x, int y);
    string print();
    int x,y;
};


class LineParametric{
public:
    cv::Point a,b;
    cv::Vec2b slope, normal;
    LineParametric(cv::Point a, cv::Point b);
    LineParametric(cv::Point a, cv::Vec2b slope, int lenght = 1);

    cv::Vec2b SlopeVector();
    cv::Vec2b NormalVector();

    void draw(cv::Mat &img, cv::Scalar color = cv::Scalar(0,0,255), int thick = 1 );
};

const int N_COLORS = 8;
const cv::Scalar COLORS[N_COLORS] = { cv::Scalar(0,0,255),
                                cv::Scalar(0,255,0),
                                cv::Scalar(255,0,0),
                                cv::Scalar(255,0,255),
                                cv::Scalar(0,255,255),
                                cv::Scalar(255,255,0),
                                cv::Scalar(0,128,255),
                                cv::Scalar(255,0,128)
                              };
const string COLORS_NAMES[N_COLORS] = {"RED",
                                      "GREEN",
                                      "BLUE",
                                      "CYAN",
                                      "YELLOW",
                                      "AZURE",
                                      "ORANGE",
                                      "VIOLET"};

const cv::Scalar DEFAULT_COLOR = cv::Scalar(255,255,255);

vector<uchar> pixsUnderLine(Mat & image, Point A, Point B);
int numOfSegments(vector<uchar> &line);
int numberOf(vector<uchar> &line, uchar pix);

int indexBrowser(uint &index, uint size);
void imageBrowser(vector<string> &images);

Point rotatePoint2D(Point p, Mat M);
Mat rotateImg(Mat &img, double angle, Point center = Point(0,0));
Mat translateImg(Mat &img, int offsetx, int offsety);

enum BORDER_PLACE {BORDER_TOP, BORDER_BOTTOM, BORDER_LEFT, BORDER_RIGHT};

Mat addRows(Mat &img, int rows2add, BORDER_PLACE border = BORDER_BOTTOM);

double angleBottom(cv::Point l, cv::Point r);
cv::Mat copyFromImage(cv::Mat & img, vector<cv::Point> & points);

cv::Mat drawHistogram(vector<float> hist_values, int w = 512, int h = 256, cv::Scalar color_BG = cv::Scalar(0,0,0), cv::Scalar color_Hist = cv::Scalar(128,128,128));
cv::Mat drawHistogram(vector<float> hist_values, int val, int range, int w, int h);

void show(const char* name, cv::Mat & image, cv::Size size);
void show(const char* name, cv::Mat & image, double sizeModifier);

cv::Mat subImage(cv::Mat & img, cv::Point tl, cv::Point br);

void setPixColor(cv::Mat & img, int x, int y, cv::Scalar color);


void colorReduce(cv::Mat & img, int divisor = 8);
}


#endif
