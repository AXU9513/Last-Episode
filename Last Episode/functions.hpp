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

vector<vector<string>> readData();  // Get data from data source.

vector<string> getMessages(vector<vector<string>> words);   // Get messages from source data.

vector<string> getFeelings(vector<vector<string>> words);   // Get feelings from source data.

vector<vector<vector<string>>> cutWords(vector<string> sentence, vector<string> sentence2, cppjieba::Jieba jieba);  // Cut words in messages and feelings.

MY_MAP vec2Map(vector<string> vector);  // Transform vector to map.

vector<string> map2Vec(MY_MAP map);     //Transform map to vector.

vector<string> getStopWords();    // Get stop words from dict.

vector<vector<string>> removeStopWords(vector<string> stopWords, vector<vector<string>> wordVector, bool unique);   // Remove stop words in wordVector.

MY_MAP buildWordIndex(vector<string> stopWords, vector<string> messages, vector<string> feelings, cppjieba::Jieba jieba); // Build word index.

#endif /* functions_hpp */
