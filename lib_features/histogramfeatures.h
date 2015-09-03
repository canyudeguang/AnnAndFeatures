#ifndef HISTOGRAMFEATURES_H
#define HISTOGRAMFEATURES_H

#include "featureextractor.h"
#include "histogram.h"


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
    string name();
    void setNumberOfBins(int bins);

    Mat_<float> derivateHistogram(Mat_<float> &Histogram);
private:
    int numberOfBins;
};

#endif // HISTOGRAMFEATURES_H
