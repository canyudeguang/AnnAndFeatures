#ifndef LABELEXTRACTOR_H
#define LABELEXTRACTOR_H

#include <vector>
#include <string>

#include "lib_support/support.h"

using namespace std;
class LabelExtractor
{
public:

    static const unsigned char LBL_REST, LBL_UNKNOWN;

    static const string str_LBL_UNKNOWN;
    static const string str_LBL_REST;

    LabelExtractor();
    ~LabelExtractor();

    /**
     * @brief setLabels sets String values for the labels. Each label is one class for the classification.
     * @param vec_labels
     */
    void setLabels(vector<string> vec_labels);
    /**
     * @brief setLabels set labels from string in format "LABEL1_LABEL1_LABEL3...whatever"
     * @param string_labels
     */
    void setLabels(string string_labels);

    int getLabelIndex(string label);
    string getLabelString(uint index);
    /**
     * @brief extractLabelsFromFiles extracts uchar labels from filenames
     * @param filenames
     * @return
     */
    vector<unsigned char> extractLabelsFromFiles(vector<string> &filenames);

    unsigned char findStrLabelInString(string &filename);

    string printLblNames(bool print2cout = true);
    void printAll();


    vector<string> mstr_Labels;
    vector<unsigned char> mLabels;
    int number_of_classes;

    bool hasRestMember;
};

#endif // LABELEXTRACTOR_H
