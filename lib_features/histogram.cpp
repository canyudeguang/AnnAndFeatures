#include "histogram.h"


Three::Three(int b1, int b2, int b3){
    b[0] = b1;
    b[1] = b2;
    b[2] = b3;
}
ostream& operator<<(ostream& os, const Three& three){
    os << three.b[0] << " ";
    os << three.b[1] << " ";
    os << three.b[2] << endl;
    return os;
}

void Threes::addValue(int b1, int b2, int b3){

    this->featureVec.push_back(Three(b1,b2,b3));

}

int Threes::highestIndex(int byte){
    int max = 0;
    int index = 0;
    for(int i = 0; i < featureVec.size(); ++i ){
        if( featureVec.at(i).b[byte] > max){
            max = featureVec.at(i).b[byte];
            index = i;
        }
    }
    return index;
}

int Threes::lowestIndex(int byte){
    int min = 0;
    if( featureVec.size() > 0){
        min = featureVec.at(0).b[byte];
    }

    int index = 0;
    for(int i = 0; i < featureVec.size(); ++i ){
        if( featureVec.at(i).b[byte] < min){
            min = featureVec.at(i).b[byte];
            index = i;
        }
    }
    return index;
}

int Threes::front(int n, int byte){

    int sum = 0;
    for(int i = 0; i < n; ++i){
        sum += featureVec.at(i).b[byte];
    }

    return sum;
}

int Threes::tail(int n, int byte){
    int start = featureVec.size() - n;

    if(start < 0) start = 0;

    int sum = 0;
    for(int i = start; i < featureVec.size(); ++i){
        sum += featureVec.at(i).b[byte];
    }

    return sum;
}

ostream& operator<<(ostream& os, const Threes& threes)
{
    os << endl;
    for(int i = 0; i < threes.featureVec.size(); ++i ){
        os << "[";
        os << threes.featureVec.at(i).b[0] << " ";
        os << threes.featureVec.at(i).b[1] << " ";
        os << threes.featureVec.at(i).b[2] << "] " << endl;
    }
    return os;
}





Histogram::Histogram(){
    defineWindowsNames();
}

Histogram::Histogram(string file, int bins)
{
    defineWindowsNames();

    //load color image
    srcColor = imread(file, CV_LOAD_IMAGE_ANYCOLOR);
    imName = file;

    if(srcColor.data){
    // Image is loaded
    // Create Grayscale srcImage
        cvtColor(srcColor, srcGray, CV_BGR2GRAY);
        isSrc = true;
    }
    else{
        cerr << "can not read: " << file << endl;
        isSrc = false;
    }

    histSize = bins;

}

void Histogram::loadImg(string file, int bins){
    //load color image
    srcColor = imread(file, CV_LOAD_IMAGE_ANYCOLOR);
    imName = file;

    if(srcColor.data){
    // Image is loaded
    // Create Grayscale srcImage
        cvtColor(srcColor, srcGray, CV_BGR2GRAY);
        isSrc = true;
    }
    else{
        cerr << "can not read: " << file << endl;
        isSrc = false;
    }

    histSize = bins;
}

Histogram::Histogram(Mat image, int bins)
{
    defineWindowsNames();

    //load color image
    srcColor = image;
    imName = "image";

    cvtColor(srcColor, srcGray, CV_BGR2GRAY);
    isSrc = true;

    histSize = bins;
}

void valuer(int event, int x, int y, int flags, void *data){
    Mat* hsv = (Mat *)data;
    Vec3b pix=hsv->at<Vec3b>(y,x);
    int H=pix.val[0];
    int S=pix.val[1];
    int V=pix.val[2];
    cout << x << " x " << y << "    ";
    cout << H << " " << S << "  " << V << endl;
}

void Histogram::defineWindowsNames(){
    HISTOGRAM_NAME_WINDOW_HISTOGRAM = "Histogram";
    HISTOGRAM_NAME_WINDOW_HISTOGRAM_COLOR = "Histogram_color";
    HISTOGRAM_NAME_WINDOW_IMAGE = "Image";

    HIST_NAME[HISTO_TYPE_RGB] = "RGB";
    HIST_NAME[HISTO_TYPE_HSV] = "HSV";
    HIST_NAME[HISTO_TYPE_HSL] = "HSL";
    HIST_NAME[HISTO_TYPE_HSV_FULL] = "HSV_FULL";
    HIST_NAME[HISTO_TYPE_HSL_FULL] = "HSL_FULL";
}



void Histogram::HistogramCalc(string imgpath, int type, int bins, int isShow){
    Mat srcImg = imread(imgpath, CV_LOAD_IMAGE_ANYCOLOR);
    if(!srcImg.data) return; // RETURN no data!

    Mat hist1, hist2, hist3; // Mat for each space usually RGB



    Mat workImg;

    Scalar color1[NUM_OF_HISTS] = {Scalar(255,0,0), Scalar(255,0,255), Scalar(255,0,255), Scalar(255,0,255), Scalar(255,0,255)  };
    Scalar color2[NUM_OF_HISTS] = {Scalar(0,255,0), Scalar(0,128,255), Scalar(128,128,128), Scalar(0,128,255), Scalar(128,128,128) };
    Scalar color3[NUM_OF_HISTS] = {Scalar(0,0,255), Scalar(128,128,128), Scalar(0,128,255), Scalar(128,128,128), Scalar(0,128,255)};
    // convert to desired color space
    // from now on work with workImg
    switch(type){
    case HISTO_TYPE_HSL:
        cvtColor(srcImg, workImg, CV_BGR2HLS);
        break;
    case HISTO_TYPE_HSV:
        cvtColor(srcImg, workImg, CV_BGR2HSV);
        break;
    case HISTO_TYPE_HSV_FULL:
        cvtColor(srcImg, workImg, CV_BGR2HSV_FULL);
        break;
    case HISTO_TYPE_HSL_FULL:
        cvtColor(srcImg, workImg, CV_BGR2HLS_FULL);
        break;
    default:
        srcImg.copyTo(workImg);
        break;
    }


    bool uniform = true; // same size of bins
    bool accumulate = false; // clear the histogram from the beginning

    vector<Mat> planes; // planes three channels basically
    split(workImg, planes); // split into rgb in planes;

    // set range
    float range[] = {0, 256};
    const float* histRange = {range};
    int histBins = bins;

    // BGR
    calcHist(&planes[0], 1, 0, Mat(), hist1, 1, &histBins, &histRange, uniform, accumulate);
    calcHist(&planes[1], 1, 0, Mat(), hist2, 1, &histBins, &histRange, uniform, accumulate);
    calcHist(&planes[2], 1, 0, Mat(), hist3, 1, &histBins, &histRange, uniform, accumulate);



    HistogramValues[type].featureVec.clear();

    // normalize
    if(isShow > 0){
        //to draw histogram
        // histogram image size
        int hist_h = 360;
        int hist_w = 2*hist_h;

        Mat img2show;
        img2show.create(hist_h, hist_w, CV_8UC3);
        img2show = Mat::zeros(hist_h, hist_w, CV_8UC3);

        Mat norm_hist1, norm_hist2, norm_hist3; // normalized Mat - to represent histogram normalized

        normalize(hist1, norm_hist1, 0, hist_h, NORM_MINMAX, -1, Mat());
        normalize(hist2, norm_hist2, 0, hist_h, NORM_MINMAX, -1, Mat());
        normalize(hist3, norm_hist3, 0, hist_h, NORM_MINMAX, -1, Mat());

        int bin_w = cvRound ( (double) hist_w/bins);
        int width_sub = cvRound((double) bin_w/4);
        int l_width = 2;

        for(int i = 0; i < bins; ++i){
                Point p1,p2;
                p1 = Point( i * bin_w + l_width, hist_h);
                p2 = Point(i*bin_w + width_sub,  hist_h - cvRound( norm_hist1.at<float>(i)) );
                rectangle(img2show, p1,p2 , color1[type], 2, 8, 0);

                p1 = Point(i*bin_w + width_sub +l_width  ,hist_h);
                p2 = Point(i*bin_w + width_sub*2,  hist_h - cvRound( norm_hist2.at<float>(i)));
                rectangle(img2show, p1,p2 , color2[type], 2, 8, 0);

                p1 = Point(i*bin_w + width_sub*2 + l_width,hist_h);
                p2 = Point(i*bin_w + width_sub*3,  hist_h - cvRound( norm_hist3.at<float>(i)) );
                rectangle(img2show, p1,p2 , color3[type], 2, 8, 0);

                if(type == HISTO_TYPE_RGB){
                    HistogramValues[type].addValue(hist3.at<float>(i), hist2.at<float>(i), hist1.at<float>(i));
                }
                else{
                    HistogramValues[type].addValue(hist1.at<int>(i), hist2.at<int>(i), hist3.at<int>(i));
                }
        }


        HistogramImg[type] = img2show;
        namedWindow(HIST_NAME[type], CV_WINDOW_AUTOSIZE);
        // Set mouse callback, to get values
        //setMouseCallback(HIST_NAME[type], valuer, &HistogramImg[type]);
        imshow(HIST_NAME[type],  HistogramImg[type]);
    }
    else{
        // if all the planes has same number of values
        if(hist1.rows == hist2.rows && hist2.rows == hist3.rows){
            for(int i = 0; i < bins; ++i){
                if(type == HISTO_TYPE_RGB){
                    HistogramValues[type].addValue(hist3.at<float>(i), hist2.at<float>(i), hist1.at<float>(i));
                }
                else{
                    HistogramValues[type].addValue(hist1.at<int>(i), hist2.at<int>(i), hist3.at<int>(i));
                }

            }
        }
    }




}

void Histogram::histColor(int isshow){
    if(!srcColor.data){
        cerr << "No input file is loaded" << endl;
        return;
    }
    else{

        Mat rhist, bhist, ghist;
        Mat nnorm_rhist, nnorm_bhist, nnorm_ghist;
        // number of bins

        // set range, upper boundary is exclusive
        float range[] = {0, 256};
        const float* histRange = {range};


        bool uniform = true; // same size of bins
        bool accumulate = false; // clear the histogram from the beginning

        const int channels[] = {0,1};

        // split color image int bgr planes
        vector<Mat> bgr;
        split(srcColor, bgr);

        
        // make b g r histograms
        calcHist( &bgr[0], 1, 0, Mat(), nnorm_bhist, 1, &histSize, &histRange, uniform, accumulate);
        calcHist( &bgr[1], 1, 0, Mat(), nnorm_ghist, 1, &histSize, &histRange, uniform, accumulate);
        calcHist( &bgr[2], 1, 0, Mat(), nnorm_rhist, 1, &histSize, &histRange, uniform, accumulate);

        // Draw the histogram
        int hist_h = 512; int hist_w = 2*512;

        resHistClr.create(hist_h, hist_w, CV_8UC3);
        resHistClr = Mat::zeros(hist_h, hist_w, CV_8UC3);

        // Normalize results
        normalize(nnorm_bhist, bhist, 0, resHistClr.rows, NORM_MINMAX, -1, Mat());
        normalize(nnorm_ghist, ghist, 0, resHistClr.rows, NORM_MINMAX, -1, Mat());
        normalize(nnorm_rhist, rhist, 0, resHistClr.rows, NORM_MINMAX, -1, Mat());

        // width of bin
        int bin_w = cvRound ( (double) hist_w/histSize);
        int width_sub = cvRound((double) bin_w/4);
        int l_width = 2;


        R.clear();
        B.clear();
        G.clear();

        ColorHistogramValues.featureVec.clear();
        for(int i = 0; i < histSize; ++i ){
            rectangle(resHistClr, Point(i*bin_w + l_width,hist_h), Point(i*bin_w + width_sub,  hist_h - cvRound( rhist.at<float>(i)) ), Scalar(0,0,255),l_width,8,0);
            rectangle(resHistClr, Point(i*bin_w + width_sub +l_width  ,hist_h), Point(i*bin_w + width_sub*2,  hist_h - cvRound( ghist.at<float>(i)) ), Scalar(0,255,0),l_width,8,0);
            rectangle(resHistClr, Point(i*bin_w + width_sub*2 + l_width,hist_h), Point(i*bin_w + width_sub*3,  hist_h - cvRound( bhist.at<float>(i)) ), Scalar(255,0,0),l_width,8,0);

            R.push_back(rhist.at<float>(i));
            G.push_back(ghist.at<float>(i));
            B.push_back(bhist.at<float>(i));
            ColorHistogramValues.addValue(nnorm_rhist.at<float>(i), nnorm_ghist.at<float>(i), nnorm_bhist.at<float>(i));
        }
       // cout << rhist << endl;
       // cout << ghist << endl;
       // cout << bhist << endl;

        //cvDestroyWindow(HISTOGRAM_NAME_WINDOW_IMAGE.c_str());
        // Display

        if(isshow > 0){
            // Display
            imshow(HISTOGRAM_NAME_WINDOW_HISTOGRAM_COLOR, resHistClr);
        }



        //namedWindow(imName, CV_WINDOW_FREERATIO);
        //imshow(imName, srcColor);

    }
}

Mat_<float> Histogram::getRGBfeatures(){
    int hist3size = 3*histSize;
    Mat_<float> resultFeatures = Mat_<float>(1,hist3size);


    int f = 0;
    for(int i = 0; i < R.size() ; ++i,++f){
        resultFeatures(0,f) = R.at(i);
    }
    for(int i = 0; i < G.size() ; ++i,++f){
        resultFeatures(0,f) = G.at(i);
    }
    for(int i = 0; i < B.size() ; ++i,++f){
        resultFeatures(0,f) = B.at(i);
    }
    return resultFeatures;
}

void Histogram::histHSL(int isshow){
    if(!srcColor.data){
        cerr << "No input file is loaded" << endl;
        return;
    }
    else{
        Mat rhist, bhist, ghist;
        Mat nnorm_rhist, nnorm_bhist, nnorm_ghist;

        Mat bgrsource;
        srcColor.copyTo(bgrsource);
        srcColor.setTo(0);
        cvtColor(bgrsource, srcColor, CV_BGR2HLS);
        // number of bins

        // set range, upper boundary is exclusive
        float range[] = {0, 256};
        const float* histRange = {range};


        bool uniform = true; // same size of bins
        bool accumulate = false; // clear the histogram from the beginning

        const int channels[] = {0,1};

        // split color image int bgr planes
        vector<Mat> bgr;
        split(srcColor, bgr);


        // make b g r histograms
        calcHist( &bgr[0], 1, 0, Mat(), nnorm_bhist, 1, &histSize, &histRange, uniform, accumulate);
        calcHist( &bgr[1], 1, 0, Mat(), nnorm_ghist, 1, &histSize, &histRange, uniform, accumulate);
        calcHist( &bgr[2], 1, 0, Mat(), nnorm_rhist, 1, &histSize, &histRange, uniform, accumulate);

        // Draw the histogram
        int hist_h = 512; int hist_w = 2*512;

        resHistHSL.create(hist_h, hist_w, CV_8UC3);
        resHistHSL = Mat::zeros(hist_h, hist_w, CV_8UC3);

        // Normalize results
        normalize(nnorm_bhist, bhist, 0, resHistHSL.rows, NORM_MINMAX, -1, Mat());
        normalize(nnorm_ghist, ghist, 0, resHistHSL.rows, NORM_MINMAX, -1, Mat());
        normalize(nnorm_rhist, rhist, 0, resHistHSL.rows, NORM_MINMAX, -1, Mat());

        // width of bin
        int bin_w = cvRound ( (double) hist_w/histSize);

        // Draw
//        for(int i = 1; i < histSize; i++){
//            line(histColorImg, Point( bin_w*(i-1), hist_h - cvRound( bhist.at<float>(i-1))),
//                 Point(bin_w*(i), hist_h - cvRound(bhist.at<float>(i))), Scalar(255,0,0), 1, 8, 0 );
//            line(histColorImg, Point( bin_w*(i-1), hist_h - cvRound( ghist.at<float>(i-1))),
//                Point(bin_w*(i), hist_h - cvRound(ghist.at<float>(i))), Scalar(0,255,0), 1, 8, 0 );
//            line(histColorImg, Point( bin_w*(i-1), hist_h - cvRound( rhist.at<float>(i-1))),
//                 Point(bin_w*(i), hist_h - cvRound(rhist.at<float>(i))), Scalar(0,0,255), 1, 8, 0 );
//        }


        int width_sub = cvRound((double) bin_w/4);
        int l_width = 2;

        ColorHistogramValues.featureVec.clear();
        for(int i = 0; i < histSize; ++i ){
            rectangle(resHistHSL, Point(i*bin_w + l_width,hist_h), Point(i*bin_w + width_sub,  hist_h - cvRound( rhist.at<float>(i)) ), Scalar(128,128,128),l_width,8,0);
            rectangle(resHistHSL, Point(i*bin_w + width_sub +l_width  ,hist_h), Point(i*bin_w + width_sub*2,  hist_h - cvRound( ghist.at<float>(i)) ), Scalar(255,255,0),l_width,8,0);
            rectangle(resHistHSL, Point(i*bin_w + width_sub*2 + l_width,hist_h), Point(i*bin_w + width_sub*3,  hist_h - cvRound( bhist.at<float>(i)) ), Scalar(0,255,128),l_width,8,0);
            ColorHistogramValues.addValue(nnorm_rhist.at<float>(i), nnorm_ghist.at<float>(i), nnorm_bhist.at<float>(i));
        }

       // cout << rhist << endl;
       // cout << ghist << endl;
       // cout << bhist << endl;

        //cvDestroyWindow(HISTOGRAM_NAME_WINDOW_IMAGE.c_str());
        // Display

        if(isshow > 0){
            // Display
            imshow(HISTOGRAM_NAME_WINDOW_HISTOGRAM+"HSL", resHistHSL);
        }


        //namedWindow(imName, CV_WINDOW_FREERATIO);
        //imshow(imName, srcColor);

    }
}

void Histogram::histHSV(int isshow){
    if(!srcColor.data){
        cerr << "No input file is loaded" << endl;
        return;
    }
    else{
        Mat rhist, bhist, ghist;
        Mat nnorm_rhist, nnorm_bhist, nnorm_ghist;

        Mat bgrsource;
        srcColor.copyTo(bgrsource);
        srcColor.setTo(0);
        cvtColor(bgrsource, srcColor, CV_BGR2HSV);
        // number of bins

        // set range, upper boundary is exclusive
        float range[] = {0, 256};
        const float* histRange = {range};


        bool uniform = true; // same size of bins
        bool accumulate = false; // clear the histogram from the beginning

        const int channels[] = {0,1};

        // split color image int bgr planes
        vector<Mat> bgr;
        split(srcColor, bgr);


        // make b g r histograms
        calcHist( &bgr[0], 1, 0, Mat(), nnorm_bhist, 1, &histSize, &histRange, uniform, accumulate);
        calcHist( &bgr[1], 1, 0, Mat(), nnorm_ghist, 1, &histSize, &histRange, uniform, accumulate);
        calcHist( &bgr[2], 1, 0, Mat(), nnorm_rhist, 1, &histSize, &histRange, uniform, accumulate);

        // Draw the histogram
        int hist_h = 512; int hist_w = 2*512;

        resHistHSV.create(hist_h, hist_w, CV_8UC3);
        resHistHSV = Mat::zeros(hist_h, hist_w, CV_8UC3);

        // Normalize results
        normalize(nnorm_bhist, bhist, 0, resHistHSV.rows, NORM_MINMAX, -1, Mat());
        normalize(nnorm_ghist, ghist, 0, resHistHSV.rows, NORM_MINMAX, -1, Mat());
        normalize(nnorm_rhist, rhist, 0, resHistHSV.rows, NORM_MINMAX, -1, Mat());

        // width of bin
        int bin_w = cvRound ( (double) hist_w/histSize);

        // Draw
//        for(int i = 1; i < histSize; i++){
//            line(histColorImg, Point( bin_w*(i-1), hist_h - cvRound( bhist.at<float>(i-1))),
//                 Point(bin_w*(i), hist_h - cvRound(bhist.at<float>(i))), Scalar(255,0,0), 1, 8, 0 );
//            line(histColorImg, Point( bin_w*(i-1), hist_h - cvRound( ghist.at<float>(i-1))),
//                Point(bin_w*(i), hist_h - cvRound(ghist.at<float>(i))), Scalar(0,255,0), 1, 8, 0 );
//            line(histColorImg, Point( bin_w*(i-1), hist_h - cvRound( rhist.at<float>(i-1))),
//                 Point(bin_w*(i), hist_h - cvRound(rhist.at<float>(i))), Scalar(0,0,255), 1, 8, 0 );
//        }


        int width_sub = cvRound((double) bin_w/4);
        int l_width = 2;

        ColorHistogramValues.featureVec.clear();
        for(int i = 0; i < histSize; ++i ){
            rectangle(resHistHSV, Point(i*bin_w + l_width,hist_h), Point(i*bin_w + width_sub,  hist_h - cvRound( rhist.at<float>(i)) ), Scalar(128,128,128),l_width,8,0);
            rectangle(resHistHSV, Point(i*bin_w + width_sub +l_width  ,hist_h), Point(i*bin_w + width_sub*2,  hist_h - cvRound( ghist.at<float>(i)) ), Scalar(255,255,0),l_width,8,0);
            rectangle(resHistHSV, Point(i*bin_w + width_sub*2 + l_width,hist_h), Point(i*bin_w + width_sub*3,  hist_h - cvRound( bhist.at<float>(i)) ), Scalar(0,255,128),l_width,8,0);

            ColorHistogramValues.addValue(nnorm_rhist.at<float>(i), nnorm_ghist.at<float>(i), nnorm_bhist.at<float>(i));
        }

       // cout << rhist << endl;
       // cout << ghist << endl;
       // cout << bhist << endl;

        //cvDestroyWindow(HISTOGRAM_NAME_WINDOW_IMAGE.c_str());
        // Display

        if(isshow > 0){
            // Display
            imshow(HISTOGRAM_NAME_WINDOW_HISTOGRAM + "HSV", resHistHSV);
        }


        //namedWindow(imName, CV_WINDOW_FREERATIO);
        //imshow(imName, srcColor);

    }
}

void Histogram::setGrayImage(cv::Mat &src, int bins){
    defineWindowsNames();

    src.copyTo(this->srcGray);
    isSrc = true;
    histSize = bins;
}

void Histogram::histGrey(int isshow){
    if(!srcGray.data){
        cerr << "No input file is loaded" << endl;
        return;
    }
    else{
        Mat histogram;

        float range[] = {0, 256}; // set range, upper boundary is exclusive
        const float* histRange = {range};

        bool uniform = true; // same size of bins
        bool accumulate = false; // clear the histogram from the beginning

        const int channels[] = {0,1};


        calcHist( &srcGray, 1, channels, Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate);

        // Draw the histogram
        int hist_h = 512; int hist_w = 512;
        Mat histImg(hist_w, hist_h, CV_8UC3, Scalar(0,0,0));

        Mat normalized;
        // Normalize results
        normalize(histogram, normalized, 0, histImg.rows, NORM_MINMAX, -1, Mat());

        // width of bin
        int bin_w = cvRound ( (double) hist_w/histSize);


        histValues.clear();
        for(int i = 0; i < histSize; ++i ){
            rectangle(histImg, Point(i*bin_w,hist_h), Point(i*bin_w + bin_w,  hist_h - cvRound( normalized.at<float>(i)) ), Scalar(128,128,128),1,8,0);
            histValues.push_back(histogram.at<float>(i));
        }

//        for( vector<float>::iterator i = histValues.begin(); i != histValues.end(); ++i){
//            cout << *i << ',';
//        }


        if(isshow > 0){
            // Display
            imshow(HISTOGRAM_NAME_WINDOW_HISTOGRAM, histImg);
        }

    }

}
vector<float> Histogram::getFeatureHisto(){
//    cout << "GET FEATERE " << histValues.size() << endl;
//    for( vector<float>::iterator i = histValues.begin(); i != histValues.end(); ++i){
//        cout << *i << ',';
//    }
//    cout << endl;
    return histValues;
}

cv::Mat Histogram::showHistogram(Mat_<float> &histogram){
    //to draw histogram
    // histogram image size

    // Draw the histogram
    int hist_h = 512; int hist_w = 512;
    Mat histImg(hist_w, hist_h, CV_8UC3, Scalar(0,0,0));

    Mat normalized;
    // Normalize results
    normalize(histogram, normalized, 0, histImg.rows, NORM_MINMAX, -1, Mat());

    // width of bin
    int bin_w = cvRound ( (double) hist_w/histogram.cols);


    for(int i = 0; i < histogram.cols; ++i ){
        if( abs(histogram(0,i) - 2.0) <= 2 ){
            rectangle(histImg, Point(i*bin_w,hist_h), Point(i*bin_w + bin_w,  hist_h - cvRound( normalized.at<float>(i)) ), Scalar(0,0,128),1,8,0);

        }
        else{
            rectangle(histImg, Point(i*bin_w,hist_h), Point(i*bin_w + bin_w,  hist_h - cvRound( normalized.at<float>(i)) ), Scalar(128,128,128),1,8,0);

        }
    }

    return histImg;
}
