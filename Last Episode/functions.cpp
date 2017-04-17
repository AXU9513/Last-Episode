//
//  functions.cpp
//  Last Episode
//
//  Created by Alex Xu on 4/6/17.
//  Copyright © 2017 Alex Xu. All rights reserved.
//

#include "functions.hpp"

MatrixXd calTFIDF(vector<vector<int>> sntncIndex, int dicLength, string name){
    cout << "Start calculate TF matrix for "<< name << "..." << endl;
    int sentenceLength = (int)sntncIndex.size();
    vector<bool> count(dicLength, false);       // Whether a artical contains a particular word.
    vector<int> articalCount(dicLength, 0);     // Number of articals which contain the particular word.
    vector<vector <double> > tfMatrix(sentenceLength ,vector<double>(dicLength, 0.0));  // Init a n*m matrix with all elements equal to 0
    for (int i = 0; i < sentenceLength; i++) {
        for (int j = 0; j < sntncIndex[i].size(); j++) {
            tfMatrix[i][sntncIndex[i][j]]++;    // Count the times a word appears in artical i.
            count[sntncIndex[i][j]] = true;
        }
        for (int k = 0; k < tfMatrix[i].size(); k++) {  // tf = (Times of appearence)/(numbers in wordIndex)
            if (tfMatrix[i][k] != 0) {
                tfMatrix[i][k] = (double)tfMatrix[i][k]/(double)tfMatrix[i].size();
            }
        }
        for (int i = 0; i < dicLength; i++) {
            if (count[i]) {
                articalCount[i]++;
                count[i] = false;
            }
        }
    }
    cout << "   done." << endl;
    
    cout << "Start calculate IDF value for " << name << "..." << endl;
    int N = (int)tfMatrix.size();    // Total number of articals.
    vector<double> IDF(dicLength, 0);
    for (int i = 0; i < dicLength; i++) {   // idf = (Number of articals)/(Number of articals which contains the paticular word)
        IDF[i] = log10(((double)N + 0.01)/((double)articalCount[i] + 0.01));
    }
    cout << "   done." << endl;
    
    cout << "Start calculate TF*IDF value for " << name << "..." << endl;
//    vector<vector<double>> tfidfMatrix(sentenceLength ,vector<double>(dicLength, 0.0));
    MatrixXd tfidfMatrix = MatrixXd::Constant(sentenceLength, dicLength, 0.0);
    for (int i = 0; i < tfMatrix.size(); i++) {
        for (int j = 0; j < tfMatrix[i].size(); j++) {
//            tfidfMatrix[i][j] = tfMatrix[i][j] * IDF[j];
            tfidfMatrix(i, j) = tfMatrix[i][j] * IDF[j];
        }
    }
    cout << "   done." << endl;
    return tfidfMatrix; // Each elements in "tfidfMatrix" represents the weight of a word in the given artical.
}

vector<vector<int>> word2Index(MY_MAP map, vector<vector<string>> wordVector, string name) {
    vector<vector<int>> indexVector;
    indexVector.reserve(wordVector.size());
    
    cout << "Replacing word with index in " << name << "..." << endl;
    for (int i = 0; i < wordVector.size(); i++) {
        vector<int> tempVector;
        tempVector.reserve(wordVector[i].size());
        for (int j = 0; j < wordVector[i].size(); j++) {
            MY_MAP::iterator it = map.find(wordVector[i][j]);
            if (it == map.end()) {
                cout << "wordVector[i][i] not found in wordIndex." << endl;
            } else {
                tempVector.push_back(map[wordVector[i][j]]);
            }
        }
        indexVector.push_back(tempVector);
    }
    cout << "   done." << endl;
    return indexVector;
}

MY_MAP buildWordIndex(vector<string> stopWords, vector<string> messages, vector<string> feelings) {
    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          USER_DICT_PATH,
                          IDF_PATH,
                          STOP_WORD_PATH);
    MY_MAP map;
    vector<string> wordVector, wordVec;
    wordVector.reserve(messages.size()+feelings.size());
    vector<string>::iterator it;
    cout << "Prepare to build word index." << endl;
    cout << "       Connecting sentences..." << endl;
    for (int i = 0; i < messages.size(); i++) {     // Joint "messages" and "feelings".
        wordVector.push_back(messages[i]);
    }
    for (int i = 0; i < feelings.size(); i++) {
        wordVector.push_back(feelings[i]);
    }
    string str = limonp::Join(wordVector.begin(), wordVector.end(), ",");
    cout << "       Cutting sentences to words..." << endl;
    jieba.Cut(str, wordVec);
    sort(wordVec.begin(), wordVec.end());
    it = unique(wordVec.begin(), wordVec.end());
    wordVec.erase(it, wordVec.end());
    cout << "       Building..." << endl;
    map = vec2Map(wordVec);
    cout << "   done." << endl;
    return map;
}

vector<vector<string>> removeStopWords(vector<string> stopWords, vector<vector<string>> wordVector, bool unique, string name) {
    cout << "Removing stop words in " << name << "..." << endl;
    if (unique) {   // No repeat words in wordVector
        for (int i = 0; i < wordVector.size(); i++) {
            sort(wordVector[i].begin(), wordVector[i].end());
            vector<string> result;
            set_difference(wordVector[i].begin(), wordVector[i].end(), stopWords.begin(), stopWords.end(), inserter(result, result.begin()));
            wordVector[i] = result;
        }
    } else {    // Have repeat words in wordVector
        for (int i = 0; i < wordVector.size(); i++) {
            sort(wordVector[i].begin(), wordVector[i].end());
            vector<string> result;
            result.reserve(wordVector[i].size());
            unsigned long lastSize = wordVector[i].size();
            while (lastSize != result.size()) {     /** 此处求wordVector与停用词表差集，每次set_difference只能去除重复词中的一个，所以多次调用直到结果无变化 **/
                lastSize = result.size();
                result.clear();
                set_difference(wordVector[i].begin(), wordVector[i].end(), stopWords.begin(), stopWords.end(), inserter(result, result.begin()));
                wordVector[i] = result;
            }
        }
    }
    cout << "   done." << endl;
    return wordVector;
}

vector<string> getStopWords() {
    vector<string> stopWords;
    // Read stop_words from dict to stopWords.
    cout << "Reading stop_words from file..." << endl;
    string line;
    ifstream in;
    in.open("dict/stop_words.utf8");
    if (in.is_open()) {
        while (getline(in, line)) {
            istringstream stream(line);
            stopWords.push_back(line);
        }
    }
    stopWords.push_back("\r");
    sort(stopWords.begin(), stopWords.end());
    cout << "   done." << endl;
    return stopWords;
}

vector<string> map2Vec(MY_MAP map) {
    vector<string> sentence;
    MY_MAP::iterator it;
    for (it = map.begin(); it != map.end(); it++) {
        sentence.push_back(it->first);
    }
    return sentence;
}

map<string, int> vec2Map(vector<string> vector) {
    MY_MAP map;
    string lastWord;
    for (int i = 0; i < vector.size(); i++) {
        if (map.size() && !lastWord.compare(vector[i])) {
            continue;
        }
        map.insert(MY_MAP::value_type(vector[i], i));
        lastWord = vector[i];
    }
    return map;
}

vector<string> getMessages(vector<vector<string>> words) {
    vector<string> messages;
    messages.reserve(words.size());
    for (int i = 1; i < words.size(); i++) {
        messages.push_back(words[i][18]);
    }
    return messages;
}

vector<string> getFeelings(vector<vector<string>> words) {
    vector<string> feelings;
    feelings.reserve(words.size());
    for (int i = 1; i < words.size(); i++) {
        feelings.push_back(words[i][19]);
    }
    return feelings;
}

vector<vector<string>> cutWords(vector<string> sentence, string name) {
    vector<vector<string>> ctRst;
    vector<string> temp;
    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          USER_DICT_PATH,
                          IDF_PATH,
                          STOP_WORD_PATH);
    cout << "Start cutting words in "<< name << "..." << endl;
    ctRst.reserve(sentence.size());
    for (int i = 0; i < sentence.size(); i++) {
        jieba.Cut(sentence[i], temp);
        ctRst.push_back(temp);
    }
    cout << "   done." << endl;
    return ctRst;
}

vector<vector<string>> readData() {
    // Read data from csv to words.
    cout << "Reading data from file..." << endl;
    vector<vector<string>> words;
    string line, field, tempField = "";
    ifstream in;
    in.open("test.csv");
    if (in.is_open()) {
        //        int lineNum = 0;
        bool phraseStarted = 0;  // In case that a phrase contains ','
        vector<string> rowContext;
        while (getline(in, line)) {
            //            lineNum++;
            istringstream stream(line);
            
            while (getline(stream, field, ',')) {
                if (!field.substr(0,1).compare("\"") && phraseStarted == 0) {   // if field begin with '\"' and there's no unfinished phrase
                    phraseStarted = 1;
                    tempField = field.substr(1);
                    if (tempField.length() == 1 && tempField.substr(tempField.length()-1, tempField.length()-1).compare("\"")) {
                        ;
                    } else if (!tempField.substr(tempField.length()-2, tempField.length()-1).compare("\"\r")) {
                        tempField = tempField.substr(0, tempField.length()-2);
                        rowContext.push_back(tempField);
                        tempField.clear();
                        phraseStarted = 0;
                    } else if (!tempField.substr(tempField.length()-1, tempField.length()-1).compare("\"")) {
                        tempField = tempField.substr(0, tempField.length()-1);
                        rowContext.push_back(tempField);
                        tempField.clear();
                        phraseStarted = 0;
                    }
                } else if (phraseStarted == 1) {    // if there's any unfinished phrase
                    if (field.length() > 2) {
                        if ((!field.substr(field.length()-1, field.length()-1).compare("\"") || !field.substr(field.length()-2, field.length()-1).compare("\"\r")) && field.substr(field.length()-3, field.length()-1).compare("\"\"\r")) {
                            phraseStarted = 0;
                            tempField += ", ";
                            tempField += field.substr(0, field.length()-1);
                            rowContext.push_back(tempField);
                            tempField.clear();
                        } else {
                            tempField.append(",");
                            tempField.append(field);
                        }
                    } else if (field.length() == 2) {
                        if (!field.substr(field.length()-1, field.length()-1).compare("\"") || !field.compare("\"\r")/* && tempField.substr(0, 1).compare("\"")*/) {
                            phraseStarted = 0;
                            tempField += ", ";
                            tempField += field.substr(0, field.length()-1);
                            rowContext.push_back(tempField);
                            tempField.clear();
                        } else {
                            tempField.append(",");
                            tempField.append(field);
                        }
                    } else {
                        if (!field.compare("\"")) {
                            rowContext.push_back(tempField);
                            tempField.clear();
                            phraseStarted = 0;
                        } else {
                            tempField.append(",");
                            tempField.append(field);
                        }
                    }
                } else if (phraseStarted == 0) {
                    tempField.append(field);
                    rowContext.push_back(tempField);
                    tempField.clear();
                } else {
                    cout << "******* ERROR *******" << endl;
                }
            }
            if (!phraseStarted) {
                words.push_back(rowContext);
                if (rowContext.size() != 20) {
                    cout << "WRONG ROW SIZE!" << endl;
                }
                rowContext.clear();
            }
        }
    }
    cout << "   done." << endl;
    
    return words;
}
