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

    cv::Mat src;
    cvtColor(image,src,CV_BGR2GRAY);

    Histogram histovalues(image, this->numberOfBins);
    histovalues.histGrey();

//    imshow("histeq",src);


    //get histo bins in vector
    vector<float> histbins = histovalues.getFeatureHisto();

    Mat_<float> resultFeatures = Mat_<float>(1,histbins.size());

    for(int i = 0; i < histbins.size(); ++i){
        resultFeatures(0,i) = histbins.at(i);
    }

//    Mat orighist = histovalues.showHistogram(resultFeatures);
//    imshow("orig",orighist);

//    //blur(resultFeatures,resultFeatures,Size(15,15));

//    Mat_<float> derivate = this->derivateHistogram(resultFeatures);



//    Mat derHist = histovalues.showHistogram(derivate);
//    Mat hist = histovalues.showHistogram(resultFeatures);

//    imshow("b",derHist);
//    imshow("a",hist);cvWaitKey(0);

    return resultFeatures;
}

Mat_<float> HistogramFeatures::derivateHistogram(Mat_<float> &Histogram){

       float tmp1, tmp2;
       Mat_<float> firstDer;
       Histogram.copyTo(firstDer);

       firstDer = cv::Mat::zeros(Histogram.rows,Histogram.cols,Histogram.type());

       firstDer(0,0) = Histogram(0,1) - Histogram(0,0);

       for(int i = 1; i < Histogram.cols; ++i){
           tmp1 = Histogram(0,i) - Histogram(0,i-1);
           tmp2 = Histogram(0,i+1) - Histogram(0,i);

           firstDer(0,i) = (tmp2 + tmp1)/2;
       }
       firstDer(0,Histogram.cols-1) = Histogram(0,Histogram.cols-1) - Histogram(0,Histogram.cols-2);

       return firstDer;
//       int i;
//       float tmp1, tmp2;

//       //compute derivative first index
//       firstderv[0] = data[1]-data[0];

//       for(i=1; i<nx-2; i++)  //compute first derivative
//       {
//           tmp1 = data[i]-data[i-1];
//           tmp2 = data[i+1]-data[i];
//           firstderv[i] = (tmp2+tmp1)/2;
//       }

//       //compute derivative last index
//       firstderv[nx-1] = data[nx-1]-data[nx-2];


}
