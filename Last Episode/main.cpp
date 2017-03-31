//
//  main.cpp
//  Last Episode
//
//  Created by Alex Xu on 3/30/17.
//  Copyright © 2017 Alex Xu. All rights reserved.
//

#include <fstream>
#include <string>
#include "cppjieba/Jieba.hpp"
using namespace std;
using namespace cppjieba;

const char* const DICT_PATH = "dict/jieba.dict.utf8";
const char* const HMM_PATH = "dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "dict/user.dict.utf8";
const char* const IDF_PATH = "dict/idf.utf8";
const char* const STOP_WORD_PATH = "dict/stop_words.utf8";

int main() {
    Jieba jieba(DICT_PATH,
                HMM_PATH,
                USER_DICT_PATH,
                IDF_PATH,
                STOP_WORD_PATH);
    vector<vector<string>> words;
    
    // Read data from csv to words.
    cout << "Reading data from file..." << endl;
    string line, field, tempField = "";
    ifstream in;
    in.open("data.csv");
    if (in.is_open()) {
        int lineNum = 0;
        bool phraseStarted = 0;  // In case that a phrase contains ','
//        bool phraseFinished = 0;
        vector<string> rowContext;
        while (getline(in, line)) {
            lineNum++;
            if (lineNum == 383116) {
//                cout << "here\n";
            }
            cout << "Reading line " << lineNum;
            istringstream stream(line);
            
            while (getline(stream, field, ',')) {
                cout << ".";
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
                            if (lineNum == 37141) {
//                                cout << "tempField\n================\n" << tempField << endl;
//                                cout << "rowContext[19]\n================\n" << rowContext[19] << endl;
                            }
                            tempField.clear();
                        } else {
//                            cout << tempField << endl;
                            tempField.append(",");
//                            cout << "==================" << endl;
                            tempField.append(field);
//                            cout << tempField << endl;
//                            cout << "==================" << endl;
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
            cout << endl;
            if (!phraseStarted) {
                words.push_back(rowContext);
                if (rowContext.size() != 20) {
                    cout << "WRONG ROW SIZE!" << endl;
                }
                rowContext.clear();
            }
        }
    }
    
//    int i = 0, j = 0;
//    for (i = 0; i < words; i++) {
//        ;
//    }
    return 0;
}


//#include "cppjieba/Jieba.hpp"
//
//using namespace std;
//
//const char* const DICT_PATH = "dict/jieba.dict.utf8";
//const char* const HMM_PATH = "dict/hmm_model.utf8";
//const char* const USER_DICT_PATH = "dict/user.dict.utf8";
//const char* const IDF_PATH = "dict/idf.utf8";
//const char* const STOP_WORD_PATH = "dict/stop_words.utf8";
//
//int main(int argc, char** argv) {
//    cppjieba::Jieba jieba(DICT_PATH,
//                          HMM_PATH,
//                          USER_DICT_PATH,
//                          IDF_PATH,
//                          STOP_WORD_PATH);
//    vector<string> words;
//    vector<cppjieba::Word> jiebawords;
//    string s;
//    string result;
//    
//    s = "他来到了网易杭研大厦";
//    cout << s << endl;
//    cout << "[demo] Cut With HMM" << endl;
//    jieba.Cut(s, words, true);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    
//    cout << "[demo] Cut Without HMM " << endl;
//    jieba.Cut(s, words, false);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    
//    s = "我来到北京清华大学";
//    cout << s << endl;
//    cout << "[demo] CutAll" << endl;
//    jieba.CutAll(s, words);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    
//    s = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造";
//    cout << s << endl;
//    cout << "[demo] CutForSearch" << endl;
//    jieba.CutForSearch(s, words);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    
//    cout << "[demo] Insert User Word" << endl;
//    jieba.Cut("男默女泪", words);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    jieba.InsertUserWord("男默女泪");
//    jieba.Cut("男默女泪", words);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    
//    cout << "[demo] CutForSearch Word With Offset" << endl;
//    jieba.CutForSearch(s, jiebawords, true);
//    cout << jiebawords << endl;
//    
//    cout << "[demo] Lookup Tag for Single Token" << endl;
//    const int DemoTokenMaxLen = 32;
//    char DemoTokens[][DemoTokenMaxLen] = {"拖拉机", "CEO", "123", "。"};
//    vector<pair<string, string> > LookupTagres(sizeof(DemoTokens) / DemoTokenMaxLen);
//    vector<pair<string, string> >::iterator it;
//    for (it = LookupTagres.begin(); it != LookupTagres.end(); it++) {
//        it->first = DemoTokens[it - LookupTagres.begin()];
//        it->second = jieba.LookupTag(it->first);
//    }
//    cout << LookupTagres << endl;
//    
//    cout << "[demo] Tagging" << endl;
//    vector<pair<string, string> > tagres;
//    s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。";
//    jieba.Tag(s, tagres);
//    cout << s << endl;
//    cout << tagres << endl;;
//    
//    cout << "[demo] Keyword Extraction" << endl;
//    const size_t topk = 5;
//    vector<cppjieba::KeywordExtractor::Word> keywordres;
//    jieba.extractor.Extract(s, keywordres, topk);
//    cout << s << endl;
//    cout << keywordres << endl;
//    return EXIT_SUCCESS;
//}
