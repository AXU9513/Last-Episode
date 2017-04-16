//
//  main.cpp
//  Last Episode
//
//  Created by Alex Xu on 3/30/17.
//  Copyright © 2017 Alex Xu. All rights reserved.
//

#include "functions.hpp"
using namespace std;

int main() {
    vector<vector<string>> words;
    vector<string> messages;
    vector<string> feelings;
    
    // Get data from file.
    words = readData();    // Read data from csv to words.
    messages = getMessages(words);  // Get messages from words.
    feelings = getFeelings(words);  // Get feelings from words.
    
    // Build map<int, string>，like: map<index in vector, sentence>, none repeat.
    MY_MAP mp4Msg, mp4Flg;
    mp4Msg = vec2Map(messages);
    mp4Flg = vec2Map(feelings);
    
    // Vectors with no repeat.
    messages.clear();
    feelings.clear();
    messages = map2Vec(mp4Msg);
    feelings = map2Vec(mp4Flg);
    
    // Read stop words from dictionary.
    vector<string> stopWords;
    stopWords = getStopWords();
    
    // Build word index.
    MY_MAP wordIndex = buildWordIndex(stopWords, messages, feelings);
    
    // Start cutting words.
    vector<vector<string>> ctRst4Msg;
    vector<vector<string>> ctRst4Flg;
    
    vector<vector<string>> result;
    result = cutWords(messages, "messages");
    ctRst4Msg = result;
    result = cutWords(feelings, "feelings");
    ctRst4Flg = result;
    
    // Remove stop words from messages & feelings.
    ctRst4Msg = removeStopWords(stopWords, ctRst4Msg, false, "messages");
    ctRst4Flg = removeStopWords(stopWords, ctRst4Flg, false, "feelings");
    
    // Replace wordVector with indexVector.
    vector<vector<int>> msgIndex;
    vector<vector<int>> flgIndex;
    msgIndex = word2Index(wordIndex, ctRst4Msg, "messages");
    flgIndex = word2Index(wordIndex, ctRst4Flg, "feelings");
    
    // Calculate TF*IDF value.
    vector<vector<double>> tfMatrix4Msg;
    vector<vector<double>> tfMatrix4Flg;
    tfMatrix4Msg = calTFIDF(msgIndex, (int)wordIndex.size(), "messages");
    tfMatrix4Flg = calTFIDF(flgIndex, (int)wordIndex.size(), "feelings");
    
    // TODO: 矩阵分解
    
    
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
//    s = "目前拿到了7个大学offer，求问大家宾大哥大mcp选哪个好啊？\r";
//    cout << s << endl;
//    cout << "[demo] Cut With HMM" << endl;
//    jieba.Cut(s, words, true);
//    cout << limonp::Join(words.begin(), words.end(), "/") << endl;
//    
//    cout << "[demo] Cut Without HMM " << endl;
//    jieba.Cut(s, words, true);
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
