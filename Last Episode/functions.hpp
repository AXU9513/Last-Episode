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
using namespace std;

vector<vector<string>> readData(vector<vector<string>> words);

vector<string> getMessages(vector<vector<string>> words);

vector<string> getFeelings(vector<vector<string>> words);

#endif /* functions_hpp */
