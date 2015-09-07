#include "featurespicker.h"

FeaturesPicker::FeaturesPicker()
{

}

FeaturesPicker::~FeaturesPicker()
{

}

void printSet(vector<vector <int> > & set){
    for(int i = 0; i < set.size(); ++i){
        vector<int> subVec = set[i];
        for(int j = 0; j < subVec.size(); ++j){
            cout << subVec[j] << ",";
        }
        cout << endl;
    }
}

//@TODO this is very shit code :D
vector<vector <int> > FeaturesPicker::powerSetIndexes(int size){

    vector<vector <int> > powerSet;

    // power set Binary: it gives me naturally all combinations :D
    for (int i = 0; i < (1 << size);++i){
        vector<int> subSet;
        for( int j = 0; j < size; ++j){
            if(i & (1 << j)){ // mask the bits in the binary number
                subSet.push_back(j);
            }
        }
        powerSet.push_back(subSet);
    }
    //printSet(powerSet);

    return powerSet;
}


vector<int> FeaturesPicker::getIndexesFromArguments(string args){

    vector<string> indexes = Support::splitString(args,',');
    vector<int> iIndexes;
    for(int i = 0; i < indexes.size();++i){
        iIndexes.push_back(stoi(indexes[i]));
    }
    return iIndexes;
}
