#include "facedescription.h"


DoublePoint::DoublePoint(){

}

DoublePoint::DoublePoint(Point left, Point right){
    this->left = left;
    this->right = right;
}

void DoublePoint::setL_XY(int x, int y){
    this->left = Point(x,y);
}
void DoublePoint::setR_XY(int x, int y){
    this->right = Point(x,y);
}
void DoublePoint::setPointL_XY(Point left){
    this->left = left;
}
void DoublePoint::setPointR_XY(Point right){
    this->right = right;
}

Point DoublePoint::getLPoint(){
    return left;
}
Point DoublePoint::getRPoint(){
    return right;
}

void DoublePoint::rotate(Mat m){
    setPointL_XY(cvSupport::rotatePoint2D(getLPoint(),m));
    setPointR_XY(cvSupport::rotatePoint2D(getRPoint(),m));
}

void DoublePoint::draw(Mat img, cv::Scalar color){
    circle(img,getLPoint(),2,color,2);
    circle(img,getRPoint(),2,color,2);
    circle(img,getCenter(),2,Scalar(200,200,255),1);
}

Point DoublePoint::getCenter(){
    int x_min = (this->left.x < this->right.x) ? this->left.x : this->right.x;
    int y_min = (this->left.y < this->right.y) ? this->left.y : this->right.y;

    Point center = Point(
    x_min+
    abs(this->right.x - this->left.x)/2,
    y_min +
    abs(this->right.y - this->left.y)/2);
    return center;
}

double DoublePoint::distance(){
    return cv::norm(this->left - this->right);
}

Eye::Eye(){
    open = true;
}

Eye::Eye(bool state){
    setOpen(state);
}

Eye::Eye(Point l, Point r){
    setPointL_XY(l);
    setPointR_XY(r);
}

Eye::Eye(Point l, Point r, bool state){
    setPointL_XY(l);
    setPointR_XY(r);
    setOpen(state);
}

string Eye::state(){
    if(open){
        return M_O;
    }
    else{
        return M_C;
    }
}


void Eye::setOpen(bool open){
    this->open = open;
}

bool Eye::getState(){
    return this->open;
}

void Eye::setState(string state){
    if(state == M_O){
        this->open = true;
    }
    else{
        this->open = false;
    }
}


MouthSimple::MouthSimple(){
    this->state = FaceState::DEFAULT;
}

MouthSimple::MouthSimple(Point l, Point r){
    this->setPointL_XY(l);
    this->setPointR_XY(r);
    this->state = FaceState::DEFAULT;
}


void MouthSimple::setState(int statenum){
    this->state = statenum;
}

void MouthSimple::setState(string state){
    for(int i = 0; i < MOUTH_STATES; i++){
        if(state == MOUTH_STATE_STR[i]){
            this->state = i;
            return;
        }
    }
}

int MouthSimple::getState(){
    return this->state;
}

string MouthSimple::getStrState(){
    return MOUTH_STATE_STR[this->state];
}

void MouthSimple::setOpen(bool o){

}






// *
// Mouth class declaration
// *
Mouth::Mouth(){
    Eye::setOpen(false);
    Teeth = false;
    Smile = false;
    Tongue = false;
}

Mouth::Mouth(bool open, bool teeth, bool smile, bool tongue):Eye(open){
}

void Mouth::setPointL_XY(Point left){
    Eye::setPointL_XY(left);
}
void Mouth::setPointR_XY(Point right){
    Eye::setPointR_XY(right);
}

string Mouth::openState(){
    if(Eye::getState()){
        return M_O;
    }
    else{
        return M_C;
    }
}

string Mouth::smileState(){
    if(Smile){
        return M_T;
    }
    else{
        return M_F;
    }
}


string Mouth::tongueState(){
    if(Tongue){
        return M_T;
    }
    else{
        return M_F;
    }
}

string Mouth::teethState(){
    if(Teeth){
        return M_T;
    }
    else{
        return M_F;
    }
}

/////////////////////////////////////////////
/// \brief FaceSimple
///////////////////////////////////////
FaceSimple::FaceSimple(Eye reye, Eye leye, MouthSimple mouth){
    this->_lEye = leye;
    this->_rEye = reye;
    this->_mouth = mouth;
    this->_isEmpty = false;
}
FaceSimple::FaceSimple(){
    this->_isEmpty = true;
}
bool FaceSimple::isEmpty(){
    return this->_isEmpty;
}
void FaceSimple::draw(cv::Mat &img, cv::Scalar color){
    _lEye.draw(img,color);
    _rEye.draw(img,color);
    _mouth.draw(img,color);
}

int FaceSimple::width(){
    return _rEye.getRPoint().x -_lEye.getLPoint().x;
}

int FaceSimple::height(){
    int higher = _lEye.getCenter().y;
    if(_rEye.getCenter().y < higher){
        higher = _rEye.getCenter().y;
    }
    return _mouth.getCenter().y - higher;
}

int FaceSimple::size(){
    return this->width()*this->height();
}

Eye FaceSimple::getLeye(){
    return this->_lEye;
}

Eye FaceSimple::getReye(){
    return this->_rEye;
}

MouthSimple FaceSimple::getMouth(){
    return this->_mouth;
}

FaceState::FaceState(){
  //  init();
}

FaceState::FaceState(Eye leye, Eye reye, MouthSimple mouth){
    this->lEye = leye;
    this->rEye = reye;
    this->mouth = mouth;
}

void FaceState::draw(Mat &img, cv::Scalar color){
    mouth.draw(img, color);
    lEye.draw(img, color);
    rEye.draw(img, color);
}

void FaceState::rotate(Mat rMat){
    mouth.rotate(rMat);
    lEye.rotate(rMat);
    rEye.rotate(rMat);
}

void FaceState::openFromFile(string filepath){
    //init();
    FileStorage fs(filepath,FileStorage::READ);

    Point leyel,leyer,reyel,reyer,nose,mouthl,mouthr;
    string leyes, reyes, mstate, tilt;


    if(fs.isOpened()){

        //cout << "Anotation File: " << filepath << endl;


        fs["LeftEye l"] >> leyel;
        fs["LeftEye r"] >> leyer;
        fs["LeftEye o"] >> leyes;

        this->lEye = Eye(leyel,leyer);
        this->lEye.setState(leyes);

        fs["RightEye l"] >>  reyel;
        fs["RightEye r"] >>  reyer;
        fs["RightEye o"] >> reyes;

        fs["Mouth l"] >> mouthl;
        fs["Mouth r"] >> mouthr;
        fs["Mouth"] >> mstate;

        this->rEye = Eye(reyel,reyer);
        this->rEye.setState(reyes);


        this->mouth = MouthSimple(mouthl,mouthr);
        this->mouth.setState(mstate);


        fs["Nose"] >> nose;
        fs["Tilt"] >> tilt;
    }
}

void FaceState::openFromFileRaw(string filepath, string imgPath){
    FileStorage fs(filepath,FileStorage::READ);

    Point leyel,leyer,reyel,reyer,nose,mouthl,mouthr;
    string leyes, reyes, mstate, tilt;

    if(fs.isOpened()){
       // cout << "Open File: " << filepath << "\tImageFile: " << imgPath << endl;

        // Left eye
        // -------------------------------------------------------------
        fs["LeftEye l"] >> leyel;
        fs["LeftEye r"] >> leyer;
        fs["LeftEye o"] >> leyes;
        int state = (leyes == "OPEN") ? true : false;
        // create LeftEye
        this->lEye = Eye(leyel, leyer, state);

        // Right Eye
        // --------------------------------------------------------------
        fs["RightEye l"] >>  reyel;
        fs["RightEye r"] >>  reyer;
        fs["RightEye o"] >> reyes;
        state = (reyes == "OPEN") ? true : false;
        // Create RightEye
        this->rEye = Eye(reyel, reyer, state);



//        fs["Nose"] >> nose;

        fs["Mouth l"] >> mouthl;
        fs["Mouth r"] >> mouthr;
        fs["Mouth"] >> mstate;
        this->mouth.setState(mstate);
        this->mouth.setPointL_XY(mouthl);
        this->mouth.setPointR_XY(mouthr);


        // calc Tilt
        // ----------------------------------------------------------------
        // prepare vector with eyes
        Vec4f out;
        vector<Point> eyes;
        // Push eye points
        eyes.push_back(this->lEye.getLPoint());
        eyes.push_back(this->lEye.getRPoint());
        eyes.push_back(this->rEye.getLPoint());
        eyes.push_back(this->rEye.getRPoint());
        // fit line from eyes points
        fitLine(eyes, out,CV_DIST_L2,0,0.1,0.1 );

        // sign correction
        int tiltsign = 1;
        if(out[3] > out[3]+out[1]){
            tiltsign = -1;
        }
        // dotproduct with collinear vector of image edge (both vectors size of 1)
        Vec4f V1(1,0,0,0);
        double headtilt = acos(V1.dot(out))*180/CV_PI;
        this->tilt = headtilt * tiltsign;
    }
}



void FaceState::init(){
    bugFace = imread("bug/head.png");
    if(bugFace.empty()){
        cerr << "Can not load bugFace" << endl;
    }
    bugFace.copyTo(defaultBug);

    imgs2load[0] = "bug/leye_o.png";
    imgs2load[1] = "bug/reye_o.png";
    imgs2load[2] = "bug/eyes_o.png";
    imgs2load[3] = "bug/eyes_c.png";

    icons2load[0] = "bug/default.png";
    icons2load[1] = "bug/smile.png";
    icons2load[2] = "bug/tongue.png";
    icons2load[3] = "bug/teeth.png";
    icons2load[4] = "bug/mouth_o.png";
    icons2load[5] = "bug/mouth_o2.png";


    for(int i = 0; i < FEATURES; ++i){
        features[i] = imread(imgs2load[i]);
        if(features[i].empty()){
            cerr << "error: FACE STATE - Can not load image: " << imgs2load[i] << endl;
        }
    }

    for(int i = 0; i < MOUTH_STATES; ++i){
        icons[i] = imread(icons2load[i]);
        if(icons[i].empty()){
            cerr << "error: FACE STATE - Can not load image: " << icons2load[i] << endl;
        }
    }
}


void FaceState::setDefault(){
    mouth.setState(DEFAULT);
    lEye.setOpen(true);
    rEye.setOpen(true);
}

Mat FaceState::getOutputImg(){
    defaultBug.copyTo(bugFace);

    if(lEye.getState() && rEye.getState()){
        bugFace = overlayImage(bugFace,features[EYESO]);
    }
    else if(lEye.getState() && !rEye.getState()){
        bugFace = overlayImage(bugFace,features[EYELO]);
    }
    else if(!lEye.getState() && rEye.getState()){
        bugFace = overlayImage(bugFace,features[EYERO]);
    }
    else{
         bugFace = overlayImage(bugFace,features[EYESC]);
    }
    return bugFace;
}

Mat FaceState::getIcon(int num){
    return icons[num];
}

Mat FaceState::overlayImage(Mat &img1, Mat &img2){
    for(int y=0;y<img2.rows;y++)
        for(int x=0;x<img2.cols;x++)
        {
            Vec3b pix = img2.at<Vec3b>(y,x);
            if(pix[0] > 0 && pix[1] > 0 && pix[2] > 0){
               img1.at<Vec3b>(y,x) = pix;
            }
        }
    return img1;
}


FaceDescription::FaceDescription()
{
    bugFace = imread("../../../bug/head.png");
    if(bugFace.empty()){
        cerr << "Can not load bugFace" << endl;
    }
    bugFace.copyTo(defaultBug);

    imgs2load[0] = "../../../bug/leye_o.png";
    imgs2load[1] = "../../../bug/reye_o.png";
    imgs2load[2] = "../../../bug/eyes_o.png";
    imgs2load[3] = "../../../bug/eyes_c.png";
    imgs2load[4] = "../../../bug/mouth_s.png";
    imgs2load[5] = "../../../bug/teeth.png";
    imgs2load[6] = "../../../bug/tongue.png";
    imgs2load[7] = "../../../bug/mouth_o.png";
    imgs2load[8] = "../../../bug/mouth_c.png";


    for(int i = 0; i < FEATURES; ++i){
        features[i] = imread(imgs2load[i]);
        if(features[i].empty()){
            cerr << "Can not load image: " << imgs2load[i] << endl;
        }
    }
}

void FaceDescription::setDefault(){
    mouth.Teeth = false;
    mouth.Smile = false;
    mouth.Tongue = false;

    mouth.setOpen(false);
    lEye.setOpen(true);
    rEye.setOpen(true);
}

Mat FaceDescription::getOutputImg(){

    defaultBug.copyTo(bugFace);

    if(lEye.getState() && rEye.getState()){
        bugFace = overlayImage(bugFace,features[EYESO]);
    }
    else if(lEye.getState() && !rEye.getState()){
        bugFace = overlayImage(bugFace,features[EYELO]);
    }
    else if(!lEye.getState() && rEye.getState()){
        bugFace = overlayImage(bugFace,features[EYERO]);
    }
    else{
         bugFace = overlayImage(bugFace,features[EYESC]);
    }

    if(mouth.getState()){
        bugFace = overlayImage(bugFace,features[MOUTHO]);
    }
    else{
        bugFace = overlayImage(bugFace,features[MOUTHC]);
    }

    if(mouth.Smile){
        bugFace = overlayImage(bugFace,features[SMILE]);
    }
    if(mouth.Teeth){
        bugFace = overlayImage(bugFace,features[TEETH]);
    }
    if(mouth.Tongue){
        bugFace = overlayImage(bugFace,features[TONGUE]);
    }
    return bugFace;
}

Mat FaceDescription::overlayImage(Mat &img1, Mat &img2){
    for(int y=0;y<img2.rows;y++)
        for(int x=0;x<img2.cols;x++)
        {
            Vec3b pix = img2.at<Vec3b>(y,x);
            if(pix[0] > 0 && pix[1] > 0 && pix[2] > 0){
               img1.at<Vec3b>(y,x) = pix;
            }
        }
    return img1;
}


