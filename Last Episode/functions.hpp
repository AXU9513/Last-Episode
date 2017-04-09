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
using namespace std;
typedef map<string, int> MY_MAP;

vector<vector<string>> readData(vector<vector<string>> words);

vector<string> getMessages(vector<vector<string>> words);

vector<string> getFeelings(vector<vector<string>> words);

vector<vector<vector<string>>> cutWords(vector<string> sentence, vector<string> sentence2, cppjieba::Jieba jieba);

MY_MAP vec2Map(vector<string> vector);

vector<string> map2Vec(MY_MAP map);

#endif /* functions_hpp */
