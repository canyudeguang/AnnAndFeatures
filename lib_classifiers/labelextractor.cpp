#include "labelextractor.h"


const string LabelExtractor::str_LBL_REST = "REST";
const string LabelExtractor::str_LBL_UNKNOWN = "UNKNOWN";

const unsigned char LabelExtractor::LBL_REST = 222;
const unsigned char LabelExtractor::LBL_UNKNOWN = 255;

LabelExtractor::LabelExtractor()
{
    this->hasRestMember = false;
    this->hasUnknownMember = false;
    this->unknown_count = 0;
    this->total_count = 0;
}

LabelExtractor::~LabelExtractor()
{

}

string LabelExtractor::printLblNames(bool print2cout){
    string toPrint;

    if(this->label_counts.size() > 0){
        int total_count;
        for(int i = 0; i < this->label_counts.size(); ++i){

        }
    }

    for(int i = 0; i < this->mstr_Labels.size(); ++i){
        toPrint.append(mstr_Labels[i]);
        if(this->label_counts.size() > 0){
            toPrint.append(" [");
            toPrint.append(to_string(this->label_counts[i]) + "]");
        }
        if(i < this->mstr_Labels.size() - 1){
            toPrint.append(", ");
        }
    }

    if(this->total_count > 0){
        toPrint.insert(0,"Labels ["+ to_string(this->total_count) +"]:\t{");
    }
    else{
        toPrint.insert(0,"Labels:\t\t{");
    }
    toPrint.append("} #" + to_string(mstr_Labels.size()));

    if(this->hasUnknownMember){
        toPrint.append(" UNKNOWN [" + to_string(this->unknown_count) + "]");
    }
    if(print2cout){
        cout << toPrint << endl;
    }

    return toPrint;
}

void LabelExtractor::printAll(){
    for(int i = 0; i < this->mLabels.size(); i++){
        cout << this->getLabelString(this->mLabels[i]) << ":\t[" << int(mLabels[i]) <<"]" << endl;
    }
}

string LabelExtractor::getLabelString(unsigned char index){
    if(index == LabelExtractor::LBL_UNKNOWN){
        return LabelExtractor::str_LBL_UNKNOWN;
    }
    else if(this->hasRestMember && index == LabelExtractor::LBL_REST){
        return LabelExtractor::str_LBL_REST;
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

unsigned char LabelExtractor::getIndexOfLabel(string label){
    for(int i = 0; i < mstr_Labels.size(); ++i){
        if( label == mstr_Labels[i]){
            return i;
        }
    }
    return LabelExtractor::LBL_UNKNOWN;
}

// Finds label name from mstr_labels in string and retur its index from mstrLabels
unsigned char LabelExtractor::findStrLabelInString(string &filename){
    for(size_t i = 0; i < this->mstr_Labels.size(); ++i){
        if(filename.find(this->mstr_Labels[i]) != string::npos){
            return i;
        }
    }
    if(this->hasRestMember){
        return this->getIndexOfLabel(LabelExtractor::str_LBL_REST);
    }
    return LabelExtractor::LBL_UNKNOWN;
}

vector<unsigned char> LabelExtractor::extractLabelsFromFiles(vector<string> &filenames){

    this->mLabels.clear();

    this->label_counts = vector<int>(this->mstr_Labels.size());
    this->label_counts.assign(this->label_counts.size(),0);

    for(size_t i = 0; i < filenames.size(); ++i){
        unsigned char foundLabel = this->findStrLabelInString(filenames[i]);
        this->mLabels.push_back(foundLabel);
        this->total_count ++;
        if(foundLabel == LBL_UNKNOWN){
            this->hasUnknownMember = true;
            unknown_count++;
        }
        this->label_counts[foundLabel]++;
    }

    return this->mLabels;
}


