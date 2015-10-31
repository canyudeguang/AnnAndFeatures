#include "myann.h"


////////////////////////////////////////////////////////////////////////////////////
/// Ctor and D tor
////////////////////////////////////////////////////////////////////////////////////

const double myANN::DEFAULT_EPS = 0.00000001;
const double myANN::DEFAULT_BP1 = 0.1;
const double myANN::DEFAULT_BP2 = 0.1;
const int myANN::DEFAULT_ITER = 100;

myANN::myANN()
{
    // Setting default Values;
    setIterations(myANN::DEFAULT_ITER);
    this->bp_param1 = myANN::DEFAULT_BP1;
    this->bp_param2 = myANN::DEFAULT_BP2;
    this->eps = myANN::DEFAULT_EPS;
}
//==============================================================================
myANN::~myANN()
{
    delete this->nnetwork;
}

string myANN::name(){
    return this->C_ANN;
}

////////////////////////////////////////////////////////////////////////////////////
/// Settings and init
////////////////////////////////////////////////////////////////////////////////////

/* Sets only vector which will represent layout of the NN
    How many hidden layers = nn_layers.size
    for each element in nn_layers is number of neurons in particular layer
*/
void myANN::setLayers(vector<string> layers){

    for(int i = 0; i < layers.size(); ++i){
        this->nn_layers.push_back(std::stoi(layers[i]));
    }

    if(this->nn_layers.size() > 0){
        this->layers = Mat(1, this->nn_layers.size()+2, CV_32SC1);
        this->layers.at<int>(0,0) = this->attributesPerSample; // input
        this->layers.at<int>(0,this->nn_layers.size()+1) = this->numberOfClasses; // output
        // fill the layers settings
        for(int i = 0; i < this->nn_layers.size();++i){
            this->layers.at<int>(0,i+1) = this->nn_layers[i];
        }
    }
    // Default Value
    else{
        this->layers = Mat(1, 3, CV_32SC1);
        this->layers.at<int>(0,0) = attributesPerSample; // input
        this->layers.at<int>(0,1) = 2; // hidden
        this->layers.at<int>(0,2) = numberOfClasses; // output
    }
#ifdef INFO
    cout << "ANN INFO: layout: " << this->layers << endl;
#endif
}


void myANN::setIterations(int iter){
    this->iters = iter;
}


////////////////////////////////////////////////////////////////////////////////////
/// Loading and saving
////////////////////////////////////////////////////////////////////////////////////

int myANN::loadFromParams(string params){
    if( params[0] == '{' && params[params.length()-1] == '}'){
        string subparams = params.substr(1,params.length()-2); // -2 beacuase first and last chars will be gone
        vector<string> v_subparams = Support::splitString(subparams,'[');
        string settings(v_subparams[0]);

        if(v_subparams.size() > 1){
            string layout(v_subparams[1]);
            layout = layout.substr(0,layout.length()-1);
            vector<string> v_layout = Support::splitString(layout,',');
            this->setLayers(v_layout);
        }
        else{
            this->setLayers(vector<string>());
        }

        vector<string> atoms = Support::splitString(settings,',');
        this->setIterations(stoi(atoms[0]));
        ///@TODO now is defaults
//        this->bp_param1 = stod(atoms[1]);
//        this->bp_param2 = stod(atoms[2]);
#ifdef INFO
        cout << "ANN INFO: iters=" << this->iters << ", eps=" << this->eps << ", bp1=" << this->bp_param1;
        cout << ", bp2=" << this->bp_param2 << ", features_count=" << this->attributesPerSample;
        cout << ", classes=" << this->numberOfClasses << endl;
#endif
    }
    else{
        return -1;
    }
    return 0;
}

void myANN::processFileLoad(string filename){
    vector<string> parts = Support::splitString(filename,'_');
    for(int i = 0; i < parts.size(); ++i){
        string part = parts[i];
        if(part[0] >= '0' && part[0] <= '9'){
            if(this->iters == DEFAULT_ITER){
                setIterations(stoi(part));
            }
        }
    }

    //layers from loaded network
    cv::Mat layers(this->nnetwork->get_layer_sizes());
    vector<int> net_layers;
    for(int i = 0; i < layers.cols; ++i){
        net_layers.push_back(layers.at<int>(0,i));
    }

    this->setClassesVecLen(net_layers[net_layers.size()-1], net_layers[0]);

    vector<string> str_inner_layers;
    for(int i = 1; i < net_layers.size()-1; ++i){
        str_inner_layers.push_back(to_string(net_layers[i]));
    }
    this->setLayers(str_inner_layers);
}

//==============================================================================
int myANN::loadFromFile(const char *filename){
#ifdef INFO
    cout << "ANN INFO: opening=" << filename << endl;
#endif
    nnetwork = new CvANN_MLP();
    nnetwork->load(filename);
    this->processFileLoad(string(filename));
    return -1;
}
//==============================================================================
int myANN::save2file(const char *filename){
    this->nnetwork->save(filename);
#ifdef INFO
    cout << "ANN INFO: saving=" << filename << endl;
#endif
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////
/// Training
////////////////////////////////////////////////////////////////////////////////////
void myANN::train(Mat_<float> &trainData, vector<uchar> &labels)
{
#ifdef DEBUG
    cout << "ANN DEBUG: training..." << endl;
#endif
    //int attributesPerSample = trainData.cols;
    //int numberOfClasses = numClasses;
    int numberOfSamples = trainData.rows;


    // Create the network using a sigmoid function
    //--------------------------------------------------------------------------
    nnetwork = new CvANN_MLP();
    nnetwork->create(layers, CvANN_MLP::SIGMOID_SYM, 1.0, 1.0);

    // Prepare labels in required format
    //--------------------------------------------------------------------------
    Mat_<float> trainLabels = Mat_<uchar>::zeros(numberOfSamples, this->numberOfClasses);

    // Fill in the matrix with labels
    for(int i = 0; i < numberOfSamples; i++) {
        trainLabels(i, labels[i]) = 1.0;
    }

    // Set the training parameters
    //--------------------------------------------------------------------------
    CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams(
        // Terminate the training after either 1000 iterations or a very small
        // change in the network weights below the specified value
        cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, this->iters, this->eps),

        // Use back-propagation for training
        CvANN_MLP_TrainParams::BACKPROP,

        // Coefficients for back-propagation training
        this->bp_param1,
        this->bp_param2
    );


//    //cout << params.bp_dw_scale << " train" << endl;
//    //cout << params.bp_moment_scale << " train" << endl;
//    //cout << params.rp_dw0 << " " <<params.rp_dw_max << " " << params.rp_dw_min << "  " << params.rp_dw_plus << " " <<params.rp_dw_minus << endl;
//    // Train the neural netwdouble dork
//    //--------------------------------------------------------------------------
    int iters = nnetwork->train(trainData, trainLabels, Mat(), Mat(), params);

    //cout <<  nnetwork->get_layer_count() <<  " " << cv::Mat(nnetwork->get_layer_sizes()) << endl;
#ifdef INFO
    cout << "ANN INFO: Number of iterations: " << iters << endl;
#endif
}

////////////////////////////////////////////////////////////////////////////////////
/// Predict interface
////////////////////////////////////////////////////////////////////////////////////
vector<uchar> myANN::predict(Mat_<float> &testData)
{
    int numberOfSamples = testData.rows;

    Mat_<float> classifResult(1, this->numberOfClasses);
    vector<uchar> predictedLabels(numberOfSamples);

    Mat_<float> predictions;

    for(int i = 0; i < numberOfSamples; i++) {
        nnetwork->predict(testData.row(i), classifResult);
#ifdef DDD
        cout << classifResult << endl;
#endif
        Point2i max_loc;
        minMaxLoc(classifResult, 0, 0, 0, &max_loc);

        // add row into predictions
        predictions.push_back(classifResult);
        //predictedLabels.push_back(maxI);
        predictedLabels[i] = static_cast<unsigned char>(max_loc.x);
    }
    return predictedLabels;
}
//==============================================================================
uchar myANN::predictResponse(cv::Mat_<float> &testData){
    uchar response = -1;

    if(testData.rows != 1){
        cerr << "invalid sample" << endl;
        return -1;
    }
    else{
        // @TODO
        // get one single responsecv
    }
    return response;
}
//==============================================================================
void myANN::showGraph(int featuresNum)
{
}


string myANN::getStrSettings(){
    string settings("");
    settings += to_string(this->iters) + "_";

    ///@NOTE BP are defaults
//    string bp1 = to_string(this->bp_param1);
//    bp1 = bp1.substr(0,3);
//    bp1.erase(std::remove(bp1.begin(), bp1.end(), '.'), bp1.end());
//    string bp2 = to_string(this->bp_param2);
//    bp2 = bp2.substr(0,3);
//    bp2.erase(std::remove(bp2.begin(), bp2.end(), '.'), bp2.end());

//    settings+= bp1 + "_" + bp2+"_";

    for(int i = 0; i < this->nn_layers.size(); ++i){
        settings += "_"+to_string(this->nn_layers[i]);
    }
    settings+="_";

    return settings;
}


