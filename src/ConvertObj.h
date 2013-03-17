/*
 *  Copyright 2012 Thomas Gerbet
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file ConvertObj.h
 * \author Thomas Gerbet
 *
 * Convertisseur de fichier OBJ en array C
 */

#ifndef CONVERTOBJ_H_
#define CONVERTOBJ_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <vector>

#include <cstdlib>
#include <cstring>

void syntaxError();
std::pair<std::vector<std::vector<float> >, std::map<std::string, std::vector<std::vector<int> > > > readObj(std::ifstream& streamFile);
void writeStruct(std::string name, std::pair<std::vector<std::vector<float> >, std::map<std::string, std::vector<std::vector<int> > > > global);


#endif /* CONVERTOBJ_H_ */
