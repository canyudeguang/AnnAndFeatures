#include "histogramfeatures.h"


HistogramFeatures::HistogramFeatures(){
    this->numberOfBins = 256;
}

void HistogramFeatures::setNumberOfBins(int bins){

    if(bins > 256){
        this->numberOfBins = 256;
    }
    else if(bins < 1){
        this->numberOfBins = 1;
    }
    else{
       this->numberOfBins = bins;
    }

}

Mat_<float> HistogramFeatures::getFeature(Mat &image){
    Histogram histovalues(image, this->numberOfBins);
    histovalues.histGrey();

    //get histo bins in vector
    vector<float> histbins = histovalues.getFeatureHisto();

    Mat_<float> resultFeatures = Mat_<float>(1,histbins.size());

    for(int i = 0; i < histbins.size(); ++i){
        resultFeatures(0,i) = histbins.at(i);
    }
    return resultFeatures;
}
