//
//  functions.hpp
//  Last Episode
//
//  Created by Alex Xu on 4/6/17.
//  Copyright © 2017 Alex Xu. All rights reserved.
//

#ifndef functions_hpp
#define functions_hpp

#include "cppjieba/Jieba.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <pthread.h>
using namespace std;
typedef map<string, int> MY_MAP;

const char* const DICT_PATH = "dict/jieba.dict.utf8";
const char* const HMM_PATH = "dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "dict/user.dict.utf8";
const char* const IDF_PATH = "dict/idf.utf8";
const char* const STOP_WORD_PATH = "dict/stop_words.utf8";

vector<vector<string>> readData();  // Get data from data source.

vector<string> getMessages(vector<vector<string>> words);   // Get messages from source data.

vector<string> getFeelings(vector<vector<string>> words);   // Get feelings from source data.

vector<vector<string>> cutWords(vector<string> sentence, string name);   // Cut words.

MY_MAP vec2Map(vector<string> vector);  // Transform vector to map.

vector<string> map2Vec(MY_MAP map);     //Transform map to vector.

vector<string> getStopWords();    // Get stop words from dict.

vector<vector<string>> removeStopWords(vector<string> stopWords, vector<vector<string>> wordVector, bool unique, string name);   // Remove stop words in wordVector.

MY_MAP buildWordIndex(vector<string> stopWords, vector<string> messages, vector<string> feelings); // Build word index.

vector<vector<int>> word2Index(MY_MAP wordIndex, vector<vector<string>> wordVector, string name);  // Transform wordVector to indexVector.

vector<vector<double>> getTF(vector<vector<int>> sntncIndex, MY_MAP wordIndex, string name); // Calculate word frequency in every document, return all documents.

#endif /* functions_hpp */
