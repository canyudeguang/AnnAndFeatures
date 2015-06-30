#include <iostream>

using namespace std;

#include <algorithm>

#include "lib_support/support.h"
#include "lib_support/cvSupport.h"


#include "lib_features/histogramfeatures.h"
#include "lib_features/cornerfeatures.h"
#include "lib_features/edgefeatures.h"
#include "lib_features/experimentfeature.h"
#include "lib_features/rawfeatures.h"
#include "lib_features/skeletfeatures.h"
#include "lib_features/hogfeatures.h"

#include "lib_ann/ann.h"\


#include "doublefann.h"
#include "fann_cpp.h"

/**
 * In this example we are using images of Eyes as the training set
 * example of file: TRAIN/eyes/00001_RE_CLOSED.jpg
 * Label is written in the file name "CLOSED"
 * FeatureVector is extracted from the image
 */
void train(){
    FANN::neural_net net;
    net.create_standard(2,3,1,6);

    net.train_on_file("output.txt",5000,1000,0.001);
    net.save("mouths_float.net");
}

int main(int argc, char ** argv)
{
    FANN::neural_net net;
    net.create_from_file("mouths_float.net");

    fann_type input[3];
    input[0] = 86;
    input[1] = 80;
    input[2] =103.5;

    fann_type * calc_out = net.run(input);


    printf("MouthTest test (%f,%f,%f) -> %f\n", input[0], input[1], input[2], calc_out[0]);

//       const unsigned int num_input = 3;
//       const unsigned int num_output = 6;
//       const unsigned int num_layers = 1;
//       const unsigned int num_neurons_hidden = 2;
//       const float desired_error = (const float) 0.001;
//       const unsigned int max_epochs = 500000;
//       const unsigned int epochs_between_reports = 1000;

//       struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

//       fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
//       fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

//       fann_train_on_file(ann, "output.txt", max_epochs,
//           epochs_between_reports, desired_error);

//       fann_save(ann, "mouths_float.net");

//       fann_destroy(ann);

       return 0;
}

