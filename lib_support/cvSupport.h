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
}


#endif
