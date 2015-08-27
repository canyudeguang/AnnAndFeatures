#include "brightfeature.h"

BrightFeature::BrightFeature()
{

}

BrightFeature::~BrightFeature()
{

}


std::vector<cv::Point> BrightFeature::findBrightest(Mat &img){
    Mat_<uchar> gray(img);
    int brightest = 0;
    for(int x = 0; x < gray.cols; ++x){
        for(int y = 0; y < gray.rows; ++y){
            int pix = gray(y,x);
            if(pix > brightest ){
                brightest = pix;
            }
        }
    }
    for(int x = 0; x < gray.cols; ++x){
        for(int y = 0; y < gray.rows; ++y){
            int pix = gray(y,x);
            if(pix == brightest ){
                this->bPoints.push_back(cv::Point(x,y));
            }
        }
    }
    return this->bPoints;
}

void BrightFeature::showBrightest(Mat &img){

    Mat copy;
    img.copyTo(copy);
    for(int i = 0; i < this->bPoints.size(); ++i){
        circle(copy,this->bPoints[i],1,Scalar(0,0,255));
    }
    imshow("brightest",copy);
}

cv::Mat BrightFeature::labClahe(Mat &img){
    cv::Mat lab_image;
    cv::cvtColor(img, lab_image, CV_BGR2Lab);

    // Extract the L channel
    std::vector<cv::Mat> lab_planes(3);
    cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]
    // apply the CLAHE algorithm to the L channel
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4);
    cv::Mat dst;
    clahe->apply(lab_planes[0], dst);
    // Merge the the color planes back into an Lab image
    dst.copyTo(lab_planes[0]);
    cv::merge(lab_planes, lab_image);

    // convert back to RGB
    cv::Mat image_clahe;
    cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

    return image_clahe;
}

cv::Mat_<float> BrightFeature::sobel(Mat &image){
    cv::Rect roi = Rect(image.cols/2 - image.cols/4,image.rows/2 - image.rows/4, image.cols/2, image.rows/2);

    cv::Mat roiImg = image(roi);

    cv::Mat gray;
    cvtColor(roiImg,gray, CV_BGR2GRAY);
    Mat Dx,Dy;


    Sobel(gray,Dx,CV_64F,1,0,3);
    Sobel(gray,Dy,CV_64F,0,1,3);
    //findBrightest(gray);

    resize(Dx,Dx,Size(64,32));
    resize(Dy,Dy,Size(64,32));

    imshow("dx",Dx);
    imshow("dy",Dy);
    Dy.push_back(Dx);
    Dy = Dy.reshape(0,1);

    cv::Mat_<float> Features;
    Features.push_back(Dy);


    return Features;
}


cv::Mat BrightFeature::claheHist(Mat &img){
        Ptr<CLAHE> cl = createCLAHE();
        cl->setClipLimit(1);
        cl->setTilesGridSize(Size(3,3));
        Mat dst;
        cl->apply(img,dst);


        dst.copyTo(this->_clahe);

//        Histogram h;
//        h.setGrayImage(this->_clahe,256);
//        h.histGrey(1);

        return dst;
}


vector<vector<Point> > BrightFeature::getContours(cv::Mat &img){
    cv::Mat image;
    img.copyTo(image);
    vector<vector<Point> > contours;
    findContours(image,contours,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//    Mat drawing1;
//    img.copyTo(drawing1);
//          for( int i = 0; i< contours.size(); i++ )
//             {
//               Scalar color = Scalar( 128,128,128 );
//               drawContours( drawing1, contours, i, color, 2, 8);
//             }
//          imshow( "Contours1", drawing1 );
    return contours;
}

 cv::Mat_<float> BrightFeature::myThres(Mat &img,int l, int h){

    Mat_<uchar> uImage;
    Mat_<uchar> wImage;
    img.copyTo(uImage);
    img.copyTo(wImage);

    //cout << l << " " <<h << endl;


    for(int x = 0; x < img.cols; ++x){
        for(int y = 0; y < img.rows; ++y){
            int val = (int)uImage(y,x);
            if( val <= l){
                uImage(y,x) = 255;
                wImage(y,x) = 0;
            }
            else if( val >= h){
                uImage(y,x) = 0;
                wImage(y,x) = 255;
            }
            else{
                uImage(y,x) = 0;
                wImage(y,x) = 0;
            }
        }
    }

    imshow("tholded",uImage);
    imshow("tholde1d",wImage);

    vector<vector<Point> > uContours = getContours(uImage);

    // find the biggest contour:
    double maxArea = 0;
    int index = 0;
    for(int i =0; i < uContours.size(); ++i){
        double area = contourArea(uContours[i]);
        if(area > maxArea){
            maxArea = area;
            index = i;
        }
    }
    Rect r = boundingRect(uContours[index]);

    Point centerMass =Point(r.tl().x + r.width/2, r.tl().y + r.height/2);
    Point centerImg = Point(img.cols/2, img.rows/2);
    double distance = cv::norm(centerMass - centerImg);

    cv::Mat_<float> Features(1,3);
    Features(0,0) = distance;
    Features(0,1) = (double)img.cols/r.width;
    Features(0,2) = (double)img.rows/r.height;

    return Features;

//    // Find contours
//    vector<vector<Point> > c1,c2;
//    vector<Vec4i> h1,h2;
//    findContours(uImage,c1,h1,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//    findContours(wImage,c2,h2,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

//    /// Draw contours
//      Mat drawing1;
//      Mat drawing2;
//      img.copyTo(drawing1);
//      img.copyTo(drawing2);

//    for(int i =0; i < c1.size();++i){
//        Rect rect = boundingRect(c1[i]);

//        rectangle(drawing1,rect,Scalar(128,128,128),2);
//        cout << rect << endl;
//        cout << rect.area() << " " << rect.width << " " << rect.height << endl;
//    }



////      for( int i = 0; i< c1.size(); i++ )
////         {
////           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
////           drawContours( drawing1, c1, i, color, 2, 8, h1, 0, Point() );
////         }
////      for( int i = 0; i< c2.size(); i++ )
////         {
////           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
////           drawContours( drawing2, c2, i, color, 2, 8, h2, 0, Point() );
////         }

//      /// Show in a window
//      namedWindow( "Contours1", CV_WINDOW_AUTOSIZE );
//      imshow( "Contours1", drawing1 );
//      namedWindow( "Contours2", CV_WINDOW_AUTOSIZE );
//      imshow( "Contours2", drawing2 );


}

int BrightFeature::findNfulledInHistogram(Mat &grImage, int start, int stop, int N, int countThres){
    Histogram h;
    h.setGrayImage(grImage,256);
    h.histGrey();

    int treshCnt = 0;
    int treshVal = 0;
    vector<float> f = h.getFeatureHisto();

    if(start <= stop){
        for(int i = start; i < stop;++i){
            if(treshCnt >= N){
                break;
            }
            if(f[i] > countThres){
                treshVal=i;
                treshCnt++;
            }
        }
    }
    else{
        for(int i = start; i > stop;--i){
            if(treshCnt >= N){
                break;
            }
            if(f[i] > countThres){
                treshVal=i;
                treshCnt++;
            }
        }
    }

    return treshVal;
}


cv::Mat_<float> BrightFeature::getFeature(Mat &image){

    // copy original image
    image.copyTo(this->_img);
    // convert to grayscale
    cvtColor(image,this->_gray, CV_BGR2GRAY);
    // equalize histogram
    equalizeHist(this->_gray,this->_eq);
    // equalize CLAHE adaptive
    claheHist(this->_gray);

    int lThres = this->findNfulledInHistogram(this->_gray,0,255,20);
    int hThres = this->findNfulledInHistogram(this->_gray,255,0,40);

   // myThres(this->_gray,lThres,hThres);

//    //@TODO prvni udelat vsude stejnou svetelnost...

   // cvSupport::show("Clae",this->_clahe, 3);


    cv::Mat_<float> Features;
    return myThres(this->_gray,lThres,hThres);
}
