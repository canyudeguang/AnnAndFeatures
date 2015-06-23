#ifndef HISTOGRAMFEATURES_H
#define HISTOGRAMFEATURES_H

#include "featureextractor.h"
#include "histogram.h"
#include "lib_support/cvSupport.h"


/**
  Extracts features from histogram (just raw values from bins so far)
  Usage:

  AnnTrainer
  */

class HistogramFeatures : public FeatureExtractor
{
public:
    HistogramFeatures();
    virtual Mat_<float> getFeature(Mat &image);
    void setNumberOfBins(int bins);

    Mat_<float> derivateHistogram(Mat_<float> &Histogram);
    Mat_<float> derivateHistogram(vector<float> &Histogram);

    vector<float> lowpass(vector<float> & src, float xm1);
private:
    int numberOfBins;
};

#endif // HISTOGRAMFEATURES_H
