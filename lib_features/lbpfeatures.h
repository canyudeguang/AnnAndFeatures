#ifndef LBPFEATURES_H
#define LBPFEATURES_H

#include "featureextractor.h"
#include "lib_support/support.h"


#include "queue"

class LBPFeatures : public FeatureExtractor
{
public:
    LBPFeatures();
    ~LBPFeatures();

    static const int DEF_HEIGHT = 64;
    static const int DEF_WIDTH = 128;
    string name();
    cv::Mat_<float> getFeature(cv::Mat &image);

    void setSize(int width, int height);

    static void Lbp8(cv::Mat &src, cv::Mat &dst, int neighRadius = 1);
    static void Lbp32(cv::Mat &src, cv::Mat &dst, int neighRadius = 2);
    static void Lbp64(cv::Mat &src, cv::Mat &dst, int neighRadius = 3);

    static unsigned long NeighBits(int neighRadius, cv::Mat & src, int _x, int _y);


    cv::Mat_<float> showPeaks(cv::Mat &src, cv::Mat &dst, int thres);
private:
    cv::Size imgSize;
    cv::Mat _ImgGray;
};

#endif // LBPFEATURES_H
