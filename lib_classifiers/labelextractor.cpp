#include "labelextractor.h"


const string LabelExtractor::str_LBL_REST = "REST";
const string LabelExtractor::str_LBL_UNKNOWN = "UNKNOWN";

const unsigned char LabelExtractor::LBL_REST = 222;
const unsigned char LabelExtractor::LBL_UNKNOWN = 255;

LabelExtractor::LabelExtractor()
{
    this->hasRestMember = false;
}

LabelExtractor::~LabelExtractor()
{

}

string LabelExtractor::printLblNames(bool print2cout){
    string toPrint;

    for(int i = 0; i < this->mstr_Labels.size(); ++i){
        toPrint.append(mstr_Labels[i]);
        if(i < this->mstr_Labels.size() - 1){
            toPrint.append(", ");
        }
    }
    if(print2cout){
        cout << toPrint << endl;
    }

    toPrint.insert(0,"Labels:\t{");
    toPrint.append("}");

    return toPrint;
}

void LabelExtractor::printAll(){
    for(int i = 0; i < this->mLabels.size(); i++){
        cout << this->getLabelString(this->mLabels[i]) << ":\t[" << int(mLabels[i]) <<"]" << endl;
    }
}

string LabelExtractor::getLabelString(uint index){
    if(index == LabelExtractor::LBL_UNKNOWN){
        return LabelExtractor::str_LBL_UNKNOWN;
    }
    return this->mstr_Labels[index];
}

void LabelExtractor::setLabels(string string_labels){

    string labels = Support::getFileName(string_labels);
    vector<string> parts = Support::splitString(labels, '_');

    this->mstr_Labels.clear();

    for(int i = 0; i < parts.size(); ++i){
        string part = parts[i];
        // Is Label
        if(part[0] >= 'A' && part[0] <= 'Z'){
            if(part == LabelExtractor::str_LBL_REST){
                this->hasRestMember = true;
            }
            this->mstr_Labels.push_back(part);
        }
    }

    this->number_of_classes = this->mstr_Labels.size();
}

unsigned char LabelExtractor::findStrLabelInString(string &filename){
    for(size_t i = 0; i < this->mstr_Labels.size(); ++i){
        if(filename.find(this->mstr_Labels[i]) != string::npos){
            return i;
        }
    }
    if(this->hasRestMember){
        return LabelExtractor::LBL_REST;
    }
    return LabelExtractor::LBL_UNKNOWN;
}

vector<unsigned char> LabelExtractor::extractLabelsFromFiles(vector<string> &filenames){

    this->mLabels.clear();
    for(size_t i = 0; i < filenames.size(); ++i){
        unsigned char foundLabel = this->findStrLabelInString(filenames[i]);
        this->mLabels.push_back(foundLabel);
    }

    return this->mLabels;
}


