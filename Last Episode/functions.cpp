//
//  functions.cpp
//  Last Episode
//
//  Created by Alex Xu on 4/6/17.
//  Copyright © 2017 Alex Xu. All rights reserved.
//

#include "functions.hpp"

MY_MAP buildWordIndex(vector<string> stopWords, vector<string> messages, vector<string> feelings, cppjieba::Jieba jieba) {
    MY_MAP map;
    vector<string> wordVector, wordVec;
    vector<string>::iterator it;
    for (int i = 0; i < messages.size(); i++) {     // Joint "messages" and "feelings".
        wordVector.push_back(messages[i]);
    }
    for (int i = 0; i < feelings.size(); i++) {
        wordVector.push_back(feelings[i]);
    }
//    sort(wordVector[0].begin(), wordVector[0].end());
//    it = unique(wordVector[0].begin(), wordVector[0].end());
//    wordVector[0].erase(it, wordVector[0].end());
//    for (int i = 1; i < wordVector.size(); i++) {
//        sort(wordVector[i].begin(), wordVector[i].end());
//        it = unique(wordVector[i].begin(), wordVector[i].end());
//        wordVector[i].erase(it, wordVector[i].end());
//        set_union(wordVector[i-1].begin(), wordVector[i-1].end(), wordVector[i].begin(), wordVector[i].end(), inserter(wordVec, wordVec.begin()));
//        pthread_t newThread;
//        pthread_create(&newThread, NULL, printProcess, (void *)i);
//    }
    string str = limonp::Join(wordVector.begin(), wordVector.end(), ",");
    jieba.Cut(str, wordVec);
    sort(wordVec.begin(), wordVec.end());
    it = unique(wordVec.begin(), wordVec.end());
    wordVec.erase(it, wordVec.end());
    map = vec2Map(wordVec);
    return map;
}

vector<vector<string>> removeStopWords(vector<string> stopWords, vector<vector<string>> wordVector, bool unique) {
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
            unsigned long lastSize = wordVector[i].size();
            while (lastSize != result.size()) {     /** 此处求wordVector与停用词表差集，每次set_difference只能去除重复词中的一个，所以多次调用直到结果无变化 **/
                lastSize = result.size();
                result.clear();
                set_difference(wordVector[i].begin(), wordVector[i].end(), stopWords.begin(), stopWords.end(), inserter(result, result.begin()));
                wordVector[i] = result;
            }
        }
    }
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
    sort(stopWords.begin(), stopWords.end());
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
    for (int i = 1; i < words.size(); i++) {
        messages.push_back(words[i][18]);
    }
    return messages;
}

vector<string> getFeelings(vector<vector<string>> words) {
    vector<string> feelings;
    for (int i = 1; i < words.size(); i++) {
        feelings.push_back(words[i][19]);
    }
    return feelings;
}

vector<vector<string>> cutWords(vector<string> sentence, cppjieba::Jieba jieba) {
    vector<vector<string>> ctRst;
    vector<string> temp;
    cout << "Start cutting words..." << endl;
    for (int i = 0; i < sentence.size(); i++) {
        jieba.Cut(sentence[i], temp);
        ctRst.push_back(temp);
    }
    vector<vector<vector<string>>> result;
    result.push_back(ctRst);
    return ctRst;
}

vector<vector<vector<string>>> cutWords(vector<string> sentence, vector<string> sentence2, cppjieba::Jieba jieba) {
    vector<vector<string>> ctRst, ctRst2;
    vector<string> temp, temp2;
    cout << "Start cutting messages..." << endl;
    for (int i = 0; i < sentence.size(); i++) {
        jieba.Cut(sentence[i], temp);
        ctRst.push_back(temp);
    }
    cout << "Start cutting feelings..." << endl;
    for (int i = 0; i < sentence2.size(); i++) {
        jieba.Cut(sentence2[i], temp2);
        ctRst2.push_back(temp2);
    }
    vector<vector<vector<string>>> result;
    result.push_back(ctRst);
    result.push_back(ctRst2);
    return result;
}

vector<vector<string>> readData() {
    // Read data from csv to words.
    cout << "Reading data from file..." << endl;
    vector<vector<string>> words;
    string line, field, tempField = "";
    ifstream in;
    in.open("data.csv");
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
    
    return words;
}
