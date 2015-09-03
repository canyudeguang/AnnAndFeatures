#ifndef EXPERIMENTFEATURE_H
#define EXPERIMENTFEATURE_H

#include "featureextractor.h"
#include "../lib_support/cvSupport.h"

#include "opencv2/objdetect/objdetect.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class experimentFeature : public FeatureExtractor
{
public:
    experimentFeature();
    cv::Mat_<float> getFeature(Mat &image);
    string name();
    Mat getGradientImage(Mat &img);

    int thres;
    int max_thres;

    RNG rng;

private:
    static void on_trackbar(int event, void *param);
    void init_imgs(Mat &img);

    Mat otsu(Mat &img);
    Mat colorReduce(Mat &img);
    Mat blops(Mat &img);

    Mat_<float> firstSignificantBlop(Mat &img);

    Mat haarEye(Mat &img);

    Mat img_g, img_hsv, img_orig, img_rgb, img_tmp;
    Mat img_drawing;

    Mat_<float> maxContourWidth( vector<vector<Point> > &contours, int &index);
};

#endif // EXPERIMENTFEATURE_H
