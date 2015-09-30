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

Mat_<float> HistogramFeatures::getFeatureOld(Mat &image){

    cv::Mat src;
    cvtColor(image,src,CV_BGR2GRAY);

    blur(src,src,Size(5,5));
   // equalizeHist(src,src);

    //Histogram histovalues(image, this->numberOfBins);
    Histogram histovalues;
    histovalues.setGrayImage(src, this->numberOfBins);
    histovalues.histGrey();

//    imshow("histeq",src);


    //get histo bins in vector
    vector<float> histbins = histovalues.getFeatureHisto();

    Mat_<float> resultFeatures = Mat_<float>(1,histbins.size());
    for(int i = 0; i < histbins.size(); ++i){
        resultFeatures(0,i) = histbins.at(i);
    }

cout << resultFeatures << endl;

    vector<float> low_pass = lowpass(histbins,0);

    Mat orighist = cvSupport::drawHistogram(low_pass, 1000, 256);
    Mat_<float> deriveHist = derivateHistogram(low_pass);

    //cout << deriveHist << endl;


    Mat derDraw = cvSupport::drawHistogram(deriveHist, 0, 5, 1000,256);

    imshow("hist OOOO gram",orighist);
    imshow("hist gram",derDraw);
    imshow("equaliz", src);
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

Mat_<float> HistogramFeatures::getFeature(Mat &image){
    Mat img_cie;
    image.copyTo(img_cie);
    cvtColor(image,img_cie,CV_BGR2XYZ);

    // Sub image kolem jazyka
    Point tl(image.cols * 0.2, image.rows/2);
    Point br(image.cols * 0.8, image.rows);

    cv::Mat subimage = cvSupport::subImage(img_cie,tl,br);
    cv::Mat sImage;
    image.copyTo(sImage);
    sImage = cvSupport::subImage(sImage,tl,br);


    imshow("SSS CIE", subimage);
    // 1. projet histogram, udelat trojice, seradit podle cetnosti.

    vector<cvSupport::RGBHashCoord> hashes;

    for(int x = 0; x < subimage.cols; ++x){
        for(int y = 0; y < subimage.rows; ++y){
            cvSupport::RGBHashCoord r(subimage.at<Vec3b>(y,x),x,y);
            hashes.push_back(r);
        }
    }


    cvSupport::RGBHashCoord r1(sImage.at<Vec3b>(sImage.rows/2,sImage.cols*2/3),0,0);
    cvSupport::RGBHashCoord r2(sImage.at<Vec3b>(sImage.rows/2,sImage.cols/3),0,0);


    sImage.at<Vec3b>(sImage.rows/2,sImage.cols*2/3)[0] = 255;
    sImage.at<Vec3b>(sImage.rows/2,sImage.cols/3)[0] = 255;

    double tongue_dist = r1.distance(r2);

    cout << tongue_dist << endl;

    for(int i = 0; i < hashes.size(); ++i){
        cvSupport::RGBHashCoord act = hashes.at(i);
        if(r1.distance(act) < tongue_dist){
            act.print();
            //cvSupport::setPixColor(sImage,act.x,act.y,Scalar(0,0,255));
        }
        else if(r2.distance(act) < tongue_dist){
            act.print();
            //cvSupport::setPixColor(sImage,act.x,act.y,Scalar(255,0,0));
        }
    }



    // R 110 - 135
    // B 100 - 250
    // G 110 mensi



//    Vec3b a,b;
//    a = img_cie.at<Vec3b>(0,1);
//    b = img_cie.at<Vec3b>(0,0);

//    blur(img_cie,img_cie,Size(3,3));

//    if ( a == b){
//        cout << "a==b: " << a << b << endl;
//    }
//    else{
//        cout << "a!=b: " << a << b << endl;
//    }





    Histogram h(subimage,10);
    h.histColor();


//    Histogram h(img_cie,128);
//
//    h.histColor(0);

    Mat_<float> resultFeatures;
    return resultFeatures;
}

vector<float> HistogramFeatures::lowpass(vector<float> &src, float xm1){
    vector<float> lowpassed;

    lowpassed.push_back(src[0] + xm1);
    for(size_t i = 1; i < src.size(); ++i){
        lowpassed.push_back(src[i] + src[i-1]);
    }
    return lowpassed;
}

Mat_<float> HistogramFeatures::derivateHistogram(vector<float> &Histogram){

    Mat_<float> mat_values = Mat_<float>(1,Histogram.size());
    for(int i = 0; i < Histogram.size(); ++i){
        mat_values(0,i) = Histogram.at(i);
    }
    return derivateHistogram(mat_values);
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
