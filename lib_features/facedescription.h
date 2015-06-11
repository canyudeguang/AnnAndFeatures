#ifndef FACEDESCRIPTION_H
#define FACEDESCRIPTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "lib_support/cvSupport.h"

#include <string>

#include <iostream>

#define FEATURES 4
#define MOUTH_STATES 6

using namespace std;
using namespace cv;

static const string M_O = "OPEN";
static const string M_C = "CLOSED";
static const string M_T = "TRUE";
static const string M_F = "FALSE";


static const string MOUTH_STATE_STR[] = {"DEFAULT", "SMILE", "TONGUE", "TEETH", "OPEN", "OPEN2"};


class DoublePoint{
public:
    DoublePoint();
    void setL_XY(int x, int y);
    void setR_XY(int x, int y);
    void setPointL_XY(Point left);
    void setPointR_XY(Point right);

    void rotate(Mat m);
    void draw(cv::Mat img, cv::Scalar color);

    Point getLPoint();
    Point getRPoint();

    Point getCenter();

    double distance();
private:
    Point left,right;
};


class Eye: public DoublePoint{
public:
    Eye();
    Eye(bool open_state);
    Eye(Point l, Point r, bool open_state);
    Eye(Point l, Point r);
    void setOpen(bool open);
    void setState(string state);
    bool getState();

    string state();
private:

    bool open;
    //! Eye stripe calculation
};

class Mouth: public Eye{
public:
    Mouth();
    Mouth(bool open, bool teeth, bool smile, bool tongue);

    // Inherited
    void setPointL_XY(Point left);
    void setPointR_XY(Point right);

    string openState();
    string smileState();
    string teethState();
    string tongueState();
    bool Teeth, Smile, Tongue;
};



class MouthSimple: public DoublePoint{
public:

    MouthSimple();
    MouthSimple(Point l, Point r);
    // Inherited

    void setState(int statenum);
    void setState(string state);
    int getState();
    string getStrState();

    // Just void - Don't Use!
    void setOpen(bool o);
    bool Teeth,Tongue,Smile;

private:
    int state;
};

class FaceSimple{
public:
    FaceSimple();
    FaceSimple(Eye reye, Eye leye, MouthSimple mouth);
    Eye getReye();
    Eye getLeye();
    MouthSimple getMouth();
    bool isEmpty();
    void draw(cv::Mat &img, cv::Scalar color);

    int width();
    int height();
    int size();


private:
    bool _isEmpty;
    MouthSimple _mouth;
    Eye _rEye;
    Eye _lEye;
};

class FaceState{
public:
    FaceState();

    void openFromFile(string filepath);
    void openFromFileRaw(string filepath, string imgPath);
    void setDefault();

    void rotate(Mat rMat);

    void draw(Mat &img, cv::Scalar color = cv::Scalar(0,200,100));

    Mat getOutputImg();

    Mat getIcon(int num);


    Eye lEye;
    Eye rEye;
    cv::Point nose;
    MouthSimple mouth;
    double tilt;
    int qtilt;
    enum MOUTH_STATE {DEFAULT, SMILE, TONGUE, TEETH, OPEN, OPEN2};
    enum Features{EYELO,EYERO,EYESO,EYESC};



private:
    void init();
    Mat bugFace;
    Mat defaultBug;

    Mat features[FEATURES];
    string imgs2load[FEATURES];

    string icons2load[MOUTH_STATES];
    Mat icons[MOUTH_STATES];

    Mat overlayImage(Mat &img1, Mat &img2);

};

class FaceDescription
{
public:
    FaceDescription();
    void setDefault();

    Mat getOutputImg();
    Eye lEye;
    Eye rEye;

    cv::Point nose;

    Mouth mouth;
    double tilt;
private:
    Mat bugFace;
    Mat defaultBug;

    Mat features[FEATURES];
    string imgs2load[FEATURES];

    Mat overlayImage(Mat &img1, Mat &img2);
    enum Features{EYELO,EYERO,EYESO,EYESC, SMILE, TEETH, TONGUE, MOUTHO, MOUTHC};
};



#endif // FACEDESCRIPTION_H
