#include "ann.h"

// Constructor
//==============================================================================
ANN::ANN(){
    // default number of classes is two
    this->numberOfClasses = 2;
}
//==============================================================================
void ANN::setLabels(string labels[], int numOfClasses){
    for(int i = 0; i < numOfClasses; i++){
        this->strLabels.push_back(labels[i]);
    }
    this->numberOfClasses = numOfClasses;

#if DEBUG
 cout << this->numberOfClasses <<  " Labels set: ";
 for(uint i = 0; i < this->strLabels.size(); i++) cout << this->strLabels[i] << ":" << i << ", ";
 cout << endl;
#endif
}
//==============================================================================
int ANN::getLabelIndex(string label){
    return find(this->strLabels.begin(), this->strLabels.end(), label) - this->strLabels.begin();
}
//==============================================================================
string ANN::getLabelString(uint iLabel){
    if(iLabel >= numberOfClasses || iLabel < 0) return "null";
    return this->strLabels[iLabel];
}
//==============================================================================
vector<uchar> ANN::extLabelFromFileName(vector<string> &fileNames){
    vector<uchar> labels;
    for(size_t i = 0; i < fileNames.size(); ++i){
        uchar foundLabel = this->findStrLabel(fileNames[i]);
       // if(foundLabel){
        //@Must fill all labels even those are wrong - unknown label perhaps (-1)
            labels.push_back(foundLabel);
       // }
    }
    return labels;
}
//==============================================================================
uchar ANN::findStrLabel(const string & filename){
    //find for each label
    for(size_t i = 0; i < this->strLabels.size(); ++i){
        if(filename.find(this->strLabels[i]) != string::npos){
            return i;
        }
    }
    return -1;
}

uchar ANN::getEYE_CLOSE(){
    return ANN::EYE_CLOSE;
}
uchar ANN::getEYE_OPEN(){
    return ANN::EYE_OPEN;
}



const string ANN::LABEL_OPEN = "OPEN";
const string ANN::LABEL_CLOSED = "CLOSED";
const string ANN::LABEL_NONE = "BAD LABEL";

string ANN::labelToString(uchar label){
    string str;
    if(label == ANN::EYE_CLOSE){
        str = ANN::LABEL_CLOSED;
    }
    else if(label == ANN::EYE_OPEN){
        str = ANN::LABEL_OPEN;
    }
    else{
        str = ANN::LABEL_NONE;
    }
    return str;
}


// Vector of size how many numbers, each number represents how many neurons are in each layer
// eg [1,1,3] means 3 layers 1 neuron in 1st 1 in 2nd 3 in 3rd
void ANN::setParameters(int inputs, vector<int> & lay, int output){

    int defaultSize = 2;

    int netsize = defaultSize + lay.size();

    Mat layers = Mat(1, netsize, CV_32SC1);
    layers.at<int>(0,0) = inputs; // input

    for(int i = 0; i < lay.size(); i++){
       layers.at<int>(0,i+1) = lay[i]; // hidden layer = number of neurons
    }

    layers.at<int>(0, netsize-1) = output; // output

    cout << "Layers: " << layers << endl;

    // Create the network using a sigmoid function
    //--------------------------------------------------------------------------
    nnetwork = new CvANN_MLP();
    nnetwork->create(layers, CvANN_MLP::SIGMOID_SYM, 1.0, 1.0);
}

void ANN::parametricTrainMouth(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int iter, vector<int> & nodes, int hidden){
    // Prepare labels in required format
    //--------------------------------------------------------------------------
    Mat_<float> trainLabels = Mat_<uchar>::zeros(trainData.rows, ANN::MOUTH_CLASSES);

    // Fill in the matrix with labels
    for(int i = 0; i < trainData.rows; i++) {
        trainLabels(i, labels[i]) = 1.0;
    }


    // Set the training parameters
    //--------------------------------------------------------------------------
    CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams(
        // Terminate the training after either 1000 iterations or a very small
        // change in the network weights below the specified value
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, iter, 0.00000001),

        // Use back-propagation for training
        CvANN_MLP_TrainParams::BACKPROP,

        // Coefficients for back-propagation training
        0.1,
        0.1
    );

    // Train the neural network
    //--------------------------------------------------------------------------
    int iters = nnetwork->train(trainData, trainLabels, Mat(), Mat(), params);

    cout << "Number of iterations: " << iters << endl;

    stringstream ss;
    ss << iters << "_" << hidden << "_hid_";
    for(int i = 0; i < nodes.size(); i++){
        ss << nodes[i] << "_";
    }
    ss <<"nodes";
    string name = ss.str();
    name="mouth_iter_"+name;
    this->saveTofile((char*)name.c_str());
}

void ANN::parametricTrain(cv::Mat_<float> &trainData, std::vector<uchar> &labels, int iter, vector<int> & nodes, int hidden){
    // Prepare labels in required format
    //--------------------------------------------------------------------------
    Mat_<float> trainLabels = Mat_<uchar>::zeros(trainData.rows, ANN::CLASSES);
    // Fill in the matrix with labels
    for(int i = 0; i < trainData.rows; i++) {
        trainLabels(i, labels[i]) = 1.0;
    }

    //cout << trainLabels << endl;

    // Set the training parameters
    //--------------------------------------------------------------------------
    CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams(
        // Terminate the training after either 1000 iterations or a very small
        // change in the network weights below the specified value
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, iter, 0.00000001),

        // Use back-propagation for training
        CvANN_MLP_TrainParams::BACKPROP,

        // Coefficients for back-propagation training
        0.1,
        0.1
    );

    // Train the neural network
    //--------------------------------------------------------------------------
    int iters = nnetwork->train(trainData, trainLabels, Mat(), Mat(), params);

    cout << "Number of iterations: " << iters << endl;

    stringstream ss;
    ss << iters << "_" << hidden << "_hid_";
    for(int i = 0; i < nodes.size(); i++){
        ss << nodes[i] << "_";
    }
    ss <<"nodes";
    string name = ss.str();
    name="iter_"+name;
    //this->saveTofile((char*)name.c_str());
}

//==============================================================================
void ANN::train(Mat_<float> &trainData, vector<uchar> &labels, int numClasses)
{
    cout << "ANN training..." << endl;

    attributesPerSample = trainData.cols;
    numberOfClasses = numClasses;
    int numberOfSamples = trainData.rows;

    Mat layers = Mat(1, 3, CV_32SC1);
    layers.at<int>(0,0) = attributesPerSample; // input
    layers.at<int>(0,1) = 2; // hidden
    layers.at<int>(0,2) = numberOfClasses; // output


    // Create the network using a sigmoid function
    //--------------------------------------------------------------------------
    nnetwork = new CvANN_MLP();
    nnetwork->create(layers, CvANN_MLP::SIGMOID_SYM, 1.0, 1.0);


    // Prepare labels in required format
    //--------------------------------------------------------------------------
    Mat_<float> trainLabels = Mat_<uchar>::zeros(numberOfSamples, numberOfClasses);

    // Fill in the matrix with labels
    for(int i = 0; i < numberOfSamples; i++) {
        trainLabels(i, labels[i]) = 1.0;
    }

    // Set the training parameters
    //--------------------------------------------------------------------------
    CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams(
        // Terminate the training after either 1000 iterations or a very small
        // change in the network weights below the specified value
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 10, 0.001),

        // Use back-propagation for training
        CvANN_MLP_TrainParams::BACKPROP,

        // Coefficients for back-propagation training
        0.1,
        0.1
    );

    // Train the neural network
    //--------------------------------------------------------------------------
    int iters = nnetwork->train(trainData, trainLabels, Mat(), Mat(), params);

    cout << "Number of iterations: " << iters << endl;
}


//==============================================================================
vector<uchar> ANN::predict(Mat_<float> &testData)
{
    int numberOfSamples = testData.rows;

    Mat_<float> classifResult(1, numberOfClasses);
    vector<uchar> predictedLabels(numberOfSamples);

    for(int i = 0; i < numberOfSamples; i++) {

        nnetwork->predict(testData.row(i), classifResult);

        Point2i max_loc;
        minMaxLoc(classifResult, 0, 0, 0, &max_loc);

        // add row into predictions
        predictions.push_back(classifResult);

        predictedLabels[i] = static_cast<unsigned char>(max_loc.x);
    }
    predictLabels.insert(predictLabels.end(),predictedLabels.begin(), predictedLabels.end());

    return predictedLabels;
}
//==============================================================================
vector<uchar> ANN::predictMouth(Mat_<float> &testData)
{
    int numberOfSamples = testData.rows;

    Mat_<float> classifResult(1, (int)MOUTH_CLASSES);
    vector<uchar> predictedLabels(numberOfSamples);

    for(int i = 0; i < numberOfSamples; i++) {

        nnetwork->predict(testData.row(i), classifResult);

        Point2i max_loc;
        minMaxLoc(classifResult, 0, 0, 0, &max_loc);

        // add row into predictions
        predictions.push_back(classifResult);

        predictedLabels[i] = static_cast<unsigned char>(max_loc.x) + 2;
    }
    predictLabels.insert(predictLabels.end(),predictedLabels.begin(), predictedLabels.end());

    return predictedLabels;
}
//==============================================================================
Mat_<float> ANN::getResponses(){
    return predictions;
}
vector<uchar> ANN::getPredictLabels(){
    return this->predictLabels;
}

void ANN::showGraph(int featuresNum)
{
}
//==============================================================================
uchar ANN::predictResponse(cv::Mat_<float> &testData){
    uchar response = -1;

    if(testData.rows != 1){
        cerr << "invalid sample" << endl;
        return -1;
    }
    else{
        // @TODO
        // get one single response
    }
    return response;
}

void ANN::loadFromFile(const char *filename){
    cout << "Opening .yml file : " << filename << endl;
    string f(filename);
    //f += ".yml";
    nnetwork = new CvANN_MLP();
    nnetwork->load(f.c_str());
}

void ANN::saveTofile(char *filename){
    string f(filename);
    f += ".yml";
    FileStorage fs(f, FileStorage::WRITE);
    nnetwork->write(*fs, filename);
    cout << "Saving classifier file: " << filename << endl;
}
