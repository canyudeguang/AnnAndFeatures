#include "cvSupport.h"


cvSupport::PixUnderCursor::PixUnderCursor(string windowName, Mat &img){
    // set object data
    this->win_name = windowName;
    this->img = img;
    // create named window
    cv::namedWindow(this->win_name,CV_WINDOW_AUTOSIZE);
    // set callback
    setMouseCallback(this->win_name, cvSupport::onMouse, this);

}


void cvSupport::onMouse(int event, int x, int y, int flags, void *userData){
    PixUnderCursor *p = (PixUnderCursor * ) userData;
    p->print(x,y);
}

void cvSupport::PixUnderCursor::show_img(){
    cv::imshow(this->win_name,this->img);
    cv::waitKey(0);
}

void cvSupport::PixUnderCursor::print(int x, int y){
    if(this->img.channels() == 3){
        cv::Vec3b pix = this->img.at<Vec3b>(y,x);
        cout << pix << endl;
    }
    else if(this->img.channels() == 1) {
        uchar pix = this->img.at<uchar>(y,x);
        cout << int(pix) << endl;
    }
    else{
        cout << " No sé " << endl;
    }
}

vector<uchar> cvSupport::pixsUnderLine(Mat &image, Point A, Point B){
    vector<uchar> pixsLine;

    // do a line segment in tmp_image
    Mat_<uchar> mask = Mat::zeros(image.size(), CV_8UC1);
    line(mask,A,B,Scalar(255));

    // create tmp_image
    Mat_<uchar> img_tmp;
    image.copyTo(img_tmp);

    for(int y = 0; y < img_tmp.rows; ++y){
        for(int x = 0; x < img_tmp.cols; ++x){
            if( mask(y,x) == 255){
                pixsLine.push_back(img_tmp(y,x));
            }
        }
    }

    // Apply Mask
    img_tmp.setTo(0);
    image.copyTo(img_tmp,mask);

    return pixsLine;
}

int cvSupport::numOfSegments(vector<uchar> &line){

    int number = 0;
    bool hasStart = false;
    bool hasEnd = false;

    for(int i = 0; i < line.size(); ++i){
        if(line[i] == 255){
            hasStart = true;
        }

        if(hasStart && line[i] == 0){
            hasStart = false;
            number++;
        }
    }
    return number;

}

int cvSupport::numberOf(vector<uchar> &line, uchar pix){
    int cnt = 0;
    for(int i = 0; i < line.size(); ++i){
        if( line[i] == pix){
            cnt++;
        }
    }
    return cnt;
}

Mat cvSupport::rotateImg(Mat &img, double angle, Point center){

    // if empty center point = rotate around center of the image
    if(center.x == 0 && center.y == 0){
        center = Point(img.cols/2, img.rows/2);
    }

    Mat rot_mat = getRotationMatrix2D(center,angle,1.0); // 1.0 = no scale;

    warpAffine(img,img,rot_mat,img.size());

    return rot_mat;
}

Mat cvSupport::translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
    return trans_mat;
}

Point cvSupport::rotatePoint2D(Point p, Mat M){
    Point r;
    r.x = M.at<double>(0,0) * p.x + M.at<double>(0,1)*p.y + M.at<double>(0,2);
    r.y = M.at<double>(1,0) * p.x + M.at<double>(1,1)*p.y + M.at<double>(1,2);
// @ TODO why -(..) ? prolly because of sign creation of head tilt
    if(r.x < 0) r.x = -r.x;
    if(r.y < 0) r.y = -r.y;
    return r;
}

void cvSupport::imageBrowser(vector<string> &images){
    for(uint i = 0; i < images.size(); ++i){
        Mat img = imread(images[i],CV_LOAD_IMAGE_COLOR);
        if(img.data){
            string imgname = Support::getFileName(images[i],true);
            string winname = "Browser: " + Support::getFilePath(images[i]);

            Mat img2show =  cvSupport::addRows(img,20,cvSupport::BORDER_TOP);
            putText(img2show, imgname, Point(0,15), CV_FONT_HERSHEY_PLAIN,1.0,Scalar(255,255,255));
            imshow(winname,img2show);

            indexBrowser(i,images.size());

        }

    }
}

/**
  Keyboard controller for Browser - n = next, b = back, q,ESC to quit
  */
int cvSupport::indexBrowser(uint &index, uint size){


    int event = 0;

    while(event == 0){

        int key = waitKey();
        if(key == 'n'){
            if( index == size-1){
               index--;
            }
            event = 1;
        }
        else if(key == 'b'){
            if(index > 0){
                index--;
                index--;
            }
            else{
                index--;
            }
            event = 1;
        }
        else if(key == 'q' || key == 27){ // 27 == ESC
            index = size;
            event = -1;
        }
//        else{
//            index--;
//            event = 1;
//        }
    }

    return event;
}

Mat cvSupport::addRows(Mat &img, int rows2add, BORDER_PLACE border){

    Mat img_with_border;

    int top, bottom, left, right;
    top = left = bottom = right = 0;

    switch(border){
    case BORDER_TOP:
        top = rows2add;
        break;
    case BORDER_BOTTOM:
        bottom = rows2add;
        break;
    case BORDER_LEFT:
        left = rows2add;
        break;
    case BORDER_RIGHT:
        right = rows2add;
        break;
    }


    copyMakeBorder(img,img_with_border,
                   top,bottom,left, right,
                   0.1, // type (if 1 -> copy last row image?)
                   Scalar(0,0,0));
    return img_with_border;
}


/**
 * @brief cvSupport::angleBottom calculates angle between the linesegment given by two points and the border of the image (in other words the tilt of the line)
 * @param l (not necessary Left) point of the linesegment
 * @param r (not necessary Right) point of the linesegment
 * @return angle in degrees between the linesegment and bottom line of the image
 */
double cvSupport::angleBottom(Point l, Point r){


    // points for the line
    vector<Point> doublePoint;
    doublePoint.push_back(l);
    doublePoint.push_back(r);

    // declaring the output
    Vec4f out;

    fitLine(doublePoint,
            out,
            CV_DIST_L2, // distance type
            0, // 0 optimal value of Numerical parameter C
            0.1, // reps - for accuracy 0.1
            0.1); // aeps - for accuracy 0.1 are good default values


    // collinear vector with the image borderline
    Vec4f V1(1,0,0,0);

    double tilt = acos(V1.dot(out))*180/CV_PI;

    int tiltsign = 1;
    if(out[3] > out[3]+out[1]){
        tiltsign = -1;
    }


    return tiltsign*tilt;

}
cv::Mat cvSupport::copyFromImage(Mat &img, vector<Point> &points){
    // create mask
    Mat mask = Mat::zeros(img.size(), CV_8UC1);
    // fill mask with white
    fillConvexPoly(mask,points,cv::Scalar(255));
    // Extract region using mask for region
    Mat imageROI;
    img.copyTo(imageROI, mask); // 'image' is the image you used to compute the contours.
    // Mat maskROI = mask(roi); // Save this if you want a mask for pixels within the contour in contourRegion.
    return imageROI;
}


cv::Mat cvSupport::drawHistogram(vector<float> hist_values, int w, int h, Scalar color_BG, Scalar color_Hist){

    if(w <= hist_values.size()){ // minimal size of histogram
        w = hist_values.size();
    }

    cv::Mat histImg(h, w, CV_8UC3, color_BG); // blank black background

    int histSize = hist_values.size();

    // Normalize results
    Mat normalized;
    normalize(hist_values, normalized, 0, histImg.rows, NORM_MINMAX, -1, Mat());

    // width of bin
    int bin_w = cvRound ( (double) w/histSize);

    // draw bins as rectangles
    for(int i = 0; i < histSize; ++i ){
        rectangle(histImg, Point(i*bin_w, h), Point(i*bin_w + bin_w,  h - cvRound( normalized.at<float>(i)) ), color_Hist,1,8,0);
    }

    return histImg;
}

cv::Mat cvSupport::drawHistogram(vector<float> hist_values, int val, int range, int w, int h){

    cv::Scalar color_BG = Scalar(0,0,0);
    cv::Scalar color_Hist = Scalar(128,0,128);
    cv::Scalar color_val = Scalar(0,0,255);
    if(w <= hist_values.size()){ // minimal size of histogram
        w = hist_values.size();
    }

    cv::Mat histImg(h, w, CV_8UC3, color_BG); // blank black background

    int histSize = hist_values.size();

    // Normalize results
    Mat normalized;
    normalize(hist_values, normalized, 0, histImg.rows, NORM_MINMAX, -1, Mat());

    // width of bin
    int bin_w = cvRound ( (double) w/histSize);

    // draw bins as rectangles
    for(int i = 0; i < histSize; ++i ){

        int diff = abs(val - hist_values[i]);

        if( diff <= range ){
            rectangle(histImg, Point(i*bin_w, h), Point(i*bin_w + bin_w,  h - cvRound( normalized.at<float>(i)) ), color_val,1,8,0);
        }
        else{
           rectangle(histImg, Point(i*bin_w, h), Point(i*bin_w + bin_w,  h - cvRound( normalized.at<float>(i)) ), color_Hist,1,8,0);

        }
    }


    return histImg;
}

void cvSupport::show(const char* name, Mat &image, Size size){
    Mat resized;
    resize(image,resized,size);
    imshow(name,resized);
}

void cvSupport::show(const char* name, cv::Mat & image, double sizeModifier){
    Mat resized;
    resize(image,resized,cv::Size(sizeModifier*image.cols, sizeModifier*image.rows));
    imshow(name,resized);
}

cv::Mat cvSupport::subImage(Mat &img, Point tl, Point br){
    cv::Mat subimg = img(Rect(tl,br));
    return subimg;
}


cvSupport::RGBHash::RGBHash(uchar r, uchar g, uchar b){
    this->init(r,g,b);
}

cvSupport::RGBHash::RGBHash(cv::Vec3b Vec3bgr){
    this->init(Vec3bgr[2], Vec3bgr[1], Vec3bgr[0]);

}

void cvSupport::RGBHash::init(uchar r, uchar g, uchar b){
    this->R = r;
    this->G = g;
    this->B = b;

    this->hash = 0;
    this->hash = this->hash | (this->R << 16);
    this->hash = this->hash | (this->G <<8);
    this->hash = this->hash | this->B;
}
string cvSupport::RGBHash::print(){
    cout << "(" << int(this->R) << ", " << int(this->G) << ", " << int(this->B) << ") #" << uint(this->hash) << endl;
    return "";
}
string cvSupport::RGBHash::rgbPrint(){
    cout << int(this->R) << " " << int(this->G) << " " << int(this->B) << endl;
    return "";
}

double cvSupport::RGBHash::distance(const cvSupport::RGBHash &rgbh){
    cv::Point3f thisPoint(this->R, this->G, this->B);
    cv::Point3f elsePoint(rgbh.R, rgbh.G, rgbh.B);
    return cv::norm(thisPoint - elsePoint);
}

cvSupport::RGBHashCoord::RGBHashCoord(cv::Vec3b Vec3bgr, int x, int y) : cvSupport::RGBHash(Vec3bgr){
    this->x = x;
    this->y = y;
}

//Override
string cvSupport::RGBHashCoord::print(){
    cout << "[" << int(this->x) << ", " << int(this->y)  << "] " << "(";
    cout << int(this->R) << ", " << int(this->G) << ", " << int(this->B) << ")" << endl;
    return "";
}

void cvSupport::setPixColor(Mat &img, int x, int y, Scalar color){
    img.at<cv::Vec3b>(y,x)[0] = color[0];
    img.at<cv::Vec3b>(y,x)[1] = color[1];
    img.at<cv::Vec3b>(y,x)[2] = color[2];
}


bool cvSupport::RGBHash::operator <(const cvSupport::RGBHash & rgbh){
    return (this->hash < rgbh.hash);
}

void cvSupport::colorReduce(Mat &img, int divisor){

//    for(int x = 0; x < img.cols; ++x){
//        for(int y = 0; y < img.rows; ++y){
//        }
//    }
}


cvSupport::LineParametric::LineParametric(Point a, Point b){
    this->a = a;
    this->b = b;
    cout << a << b << endl;
    this->SlopeVector();
    this->NormalVector();
    cout << this->slope << this->normal << endl;
}

cvSupport::LineParametric::LineParametric(Point a, Vec2b slope, int lenght){
    this->a = a;
    this->slope = slope;

    this->b.x = this->a.x + lenght * this->slope[0];
    this->b.y = this->a.y + lenght * this->slope[1];

    this->NormalVector();

}

cv::Vec2b cvSupport::LineParametric::SlopeVector(){

    this->slope[0] = this->b.x - this->a.x;
    this->slope[1] = this->b.y - this->a.y;

    return this->slope;
}

cv::Vec2b cvSupport::LineParametric::NormalVector(){
    this->normal[0] = - this->slope[1];
    this->normal[1] = this->slope[0];
    return this->normal;
}

void cvSupport::LineParametric::draw(Mat &img, Scalar color, int thick){
    line(img,this->a, this->b, color, thick);
}
