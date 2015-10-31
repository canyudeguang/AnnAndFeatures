#ifndef LABELEXTRACTOR_H
#define LABELEXTRACTOR_H

#include <vector>
#include <string>
#include <algorithm>

#include "lib_support/support.h"

using namespace std;
class LabelExtractor
{
public:

    static const unsigned char LBL_REST, LBL_UNKNOWN;

    static const string str_LBL_UNKNOWN;
    static const string str_LBL_REST;

    LabelExtractor(string Labels_settings, vector<string> & filenames);
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
    string getLabelString(unsigned char index);
    /**
     * @brief extractLabelsFromFiles extracts uchar labels from filenames
     * @param filenames
     * @return
     */
    vector<unsigned char> extractLabelsFromFiles(vector<string> &filenames);

    void removeUnknowns(vector<string> &filenames);

    unsigned char findStrLabelInString(string &filename);
    unsigned char getIndexOfLabel(string label);

    string printLblNames(bool print2cout = true);
    void printAll();
    void printAll(vector<string> & filenames);

    vector<string> mstr_Labels;
    vector<unsigned char> mLabels;
    vector<int> label_counts;
    int number_of_classes;

    bool hasRestMember;
    bool hasUnknownMember;
    int unknown_count;
    int total_count;
private:
    void init();
};

#endif // LABELEXTRACTOR_H
