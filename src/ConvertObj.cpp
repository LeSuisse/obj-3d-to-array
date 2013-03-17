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
 * \file ConvertObj.cpp
 * \author Thomas Gerbet
 *
 * Convertisseur de fichier OBJ en array C
 */

#include "ConvertObj.h"

int main(int argc, char **argv) {
	//Vérification du nombre d'arguments
	if(argc != 5) {
		syntaxError();
	}

	std::string in, out;

	//On récupère les informations fournies au programme
	int i = 1;
	while(i < argc) {
		if(strcmp(argv[i], "-in") == 0) {
			in = argv[++i];
		}
		else if(strcmp(argv[i], "-out") == 0) {
			out = argv[++i];
		}
		else {
			syntaxError();
		}
		i++;
	}

	std::ifstream streamFile(in.c_str(), std::ifstream::in);
	//On réalise la conversion
	writeStruct(out, readObj(streamFile));
	streamFile.close();

	return 0;
}

std::pair<std::vector<std::vector<float> >, std::map<std::string, std::vector<std::vector<int> > > > readObj(std::ifstream& streamFile) {
	std::map<std::string, std::pair<std::vector<std::vector<int> >, std::vector<std::vector<float> > > > global;
	std::pair<std::vector<std::vector<float> >, std::map<std::string, std::vector<std::vector<int> > > > global2;

	std::vector< std::vector<float> > lstPoints;
	std::map<std::string, std::vector<std::vector<int> > > mapFacette;

	if(streamFile.is_open()) {
		enum TypeLigne { UNDEFINED, OBJET, POINT, FACE };
		TypeLigne type = UNDEFINED;

		std::vector<std::vector<int> > lstFacettes;
		std::string objet;

		std::vector<int> facette;
		std::vector<float> point;

		//On parcourt le fichier ligne par ligne
		std::string line;
		while(std::getline(streamFile, line)) {
			//Parcourt de chaque ligne
			std::stringstream streamLine(line);
			std::string partLine;
			while(streamLine >> partLine) {
				//On récupére le type de la ligne à traiter
				if(partLine == "o") {
					type = OBJET;

					//Si un objet est déjà en cours de traitement on stocke ses données
					if(objet != "") {
						mapFacette[objet] = lstFacettes;
						lstFacettes.clear();
					}
				}
				else if(partLine == "v") {
					type = POINT;
				}
				else if(partLine == "f") {
					type = FACE;
				}
				else {
					//On effectue les traitements sur la ligne
					switch (type) {
					case OBJET:
						objet = partLine;
						break;
					case POINT:
					{
						//On stocke la liste des points
						std::stringstream streamNumber(partLine);
						float posPoint;
						streamNumber >> posPoint;
						point.push_back(posPoint);
					}
					break;
					case FACE:
					{
						std::stringstream streamNumber(partLine);
						int partFace;
						streamNumber >> partFace;
						facette.push_back(partFace);
					}
					break;
					default:
						break;
					}
				}
			}

			//On enregistre la facette ou le point en fin de ligne si besoin
			switch(type) {
			case POINT:
				lstPoints.push_back(point);
				point.clear();
				break;
			case FACE:
				lstFacettes.push_back(facette);
				facette.clear();
				break;
			default:
				break;

			}
			type = UNDEFINED;
		}
		//Enregistrement du dernier objet
		//global[objet] = std::pair<std::vector<std::vector<int> >, std::vector<std::vector<float> > > (lstFacettes, lstPoints);
		mapFacette[objet] = lstFacettes;
	}

	//return global;
	return std::pair<std::vector<std::vector<float> >, std::map<std::string, std::vector<std::vector<int> > > >(lstPoints, mapFacette);
}


void writeStruct(std::string name, std::pair<std::vector<std::vector<float> >, std::map<std::string, std::vector<std::vector<int> > > > global) {
	std::ofstream streamNewFile((name+".h").c_str());

	//On récupère la liste des points
	std::vector<std::vector<float> > points = global.first;

	for(std::map<std::string, std::vector<std::vector<int> > >::const_iterator it = global.second.begin(); it != global.second.end(); it++) {
		//Début du tableau de point d'un objet
		streamNewFile << "const static float " << it->first << "[] = {" << std::endl;

		//Parcours des facettes
		for(std::vector<std::vector<int> >::const_iterator facetteIt = it->second.begin(); facetteIt < it->second.end(); facetteIt++) {
			int nbPointFace = (*facetteIt).size();
			//Dans le cadre d'une face à 4 points, on la créée à l'aide de deux faces de 3 trois points
			if(nbPointFace == 4) {
				int cpt = 0;
				std::string tabPoints[4];
				std::stringstream ssPoint;

				//Pour chaque point de la face on va chercher la position des points correspondants
				for(std::vector<int>::const_iterator pointFaceIt = (*facetteIt).begin(); pointFaceIt < (*facetteIt).end(); pointFaceIt++) {
					ssPoint << "\t";
					for(std::vector<float>::const_iterator posPointIt = points[(*pointFaceIt)-1].begin(); posPointIt < points[(*pointFaceIt)-1].end(); posPointIt++) {
						ssPoint << *posPointIt << ", ";
					}
					ssPoint << std::endl;
					tabPoints[cpt++] = ssPoint.str();
					ssPoint.str("");
					ssPoint.clear();
				}

				//Création de la première face à 3 points
				streamNewFile << tabPoints[0] << tabPoints[1] << tabPoints[2];
				//Création de la deuxième face à 3 points
				streamNewFile << tabPoints[0] << tabPoints[2] << tabPoints[3];
			}
			else if(nbPointFace == 3) {
				//Pour chaque point de la face on va chercher la position des points correspondants
				for(std::vector<int>::const_iterator pointFaceIt = (*facetteIt).begin(); pointFaceIt < (*facetteIt).end(); pointFaceIt++) {
					streamNewFile << "\t";
					for(std::vector<float>::const_iterator posPointIt = points[(*pointFaceIt)-1].begin(); posPointIt < points[(*pointFaceIt)-1].end(); posPointIt++) {
						streamNewFile << *posPointIt << ", ";
					}
					streamNewFile << std::endl;
				}
			}
		}
		//Fin du tableau de point d'un objet
		streamNewFile << "};" << std::endl << std::endl;
	}
	//Fermeture du flux
	streamNewFile.close();
}

void syntaxError() {
	std::cerr << "Usage : -in <entree> -out <sortie>" << std::endl;
	exit(1);
}
