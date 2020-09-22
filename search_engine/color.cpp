#include <iostream>
#include <fstream>
#include "avlbst.h"

#include <sstream>
#include <map>
#include <set>

using namespace std;


typedef AVLTree<char, int> ANSWER_MAP_T;
void printSolution(ANSWER_MAP_T& answer, std::ostream& ostr);
void readingInput( std::ifstream& ifs, std::map<char, set<char>> & countryMap);
bool assigningColors( std::map<char, set<char>> &countryMap, ANSWER_MAP_T &colorMap, std::map<char, set<char>> ::iterator iter);
bool isSameSize(  std::map<char, set<char>>  &countryMap, ANSWER_MAP_T &colorMap );

int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Please supply a map input file and output filename" << endl;
        return 1;
    }

	std::ifstream inFile(argv[1]);
	std::map<char, set<char>> countryMap; //map of country and the countries touching it
	ANSWER_MAP_T colorMap; //map of the color assignment
	readingInput(inFile, countryMap);
	assigningColors( countryMap, colorMap, countryMap.begin()  );
	
	std::ofstream os(argv[2]);
	// Call printSolution to output the answers to the output file
	printSolution( colorMap, os);
	os.close();


    return 0;
}


void printSolution(ANSWER_MAP_T& answer, std::ostream& os)
{
    for(ANSWER_MAP_T::iterator it = answer.begin();
            it!= answer.end();
            ++it) {
        os << it->first << " " << it->second << endl;
    }
}


//reads in the input file to make the 2D array from the input file
//fills in a map data structure that tells which countries are adjacent
void readingInput( std::ifstream& ifs,std::map<char, set<char>> & countryMap){
	std::string line;
	int numCountries;
	int numRows;
	int numCols;
	getline( ifs, line  );
	std::stringstream ss( line );
	ss >> numCountries >> numRows >> numCols;
	
	//making the 2D array from the file input
	char inputMap[numRows][numCols];
	for(int i=0; i < numRows; i++){
		getline(ifs, line);
		stringstream mapVals(line);
		
		for( int j=0; j < numCols; j++  ){
			char charVal;
			mapVals >> charVal;
			inputMap[i][j] = charVal;  
		}
	}
	ifs.close();

	//making the map
	for( int i=0; i < numRows; i++  ){
		for(int j=0; j < numCols; j++){
			if(  countryMap.find( inputMap[i][j] ) == countryMap.end() ){
				//inserting the key and empty set into the map
				set<char> countrySet;
				countryMap.insert( std::make_pair( inputMap[i][j], countrySet  )  );
			}

			if( i+1 >= 0 && i+1 < numRows  ){
				//valid to look one row above current row
				if( (inputMap[i][j] != inputMap[i+1][j])){
					//this country is not yet in the map's set and it borders a different country than itself
					countryMap[inputMap[i][j] ].insert(inputMap[i+1][j]   );

				}
			}

			if( i-1 >= 0 && i-1 < numRows  ){
				//valid to look one row below current row
				if( (inputMap[i][j] != inputMap[i-1][j]) ){
					//this country is not yet in the map's set and it borders a different country than itself
			
						countryMap[inputMap[i][j]].insert( inputMap[i-1][j]  );

				}

			}

			if( j+1 >= 0 && j+1 < numCols  ){
				//valid to look one column to the right 
				if( (inputMap[i][j] != inputMap[i][j+1]) ){
					//this country is not yet in the map's set and it borders a different country than itself
					countryMap[ inputMap[i][j] ].insert(inputMap[i][j+1]   );

				}
			}

			if( j-1 >= 0 && j-1 < numCols  ){
				//valid to look one col to the left
				if( (inputMap[i][j] != inputMap[i][j-1]) ){
					//this country is not yet in the map's set and it borders a different country than itself
					countryMap[ inputMap[i][j] ].insert(inputMap[i][j-1]   );

				}
			}
		}
	}

}

bool assigningColors( std::map<char, set<char>> &countryMap, ANSWER_MAP_T &colorMap, std::map<char, set<char>> ::iterator iter  ){
	if( isSameSize( countryMap, colorMap ) ){
		//have valid color assignment for each country in the input map
		return true;
	}
	
	for(int i=1; i <=4; i++){
		//iterate through the 4 possible colors

		//checks to make sure none of the adjacent countries have the same color
		bool possibleColor = true;
		for( set<char>::iterator adjIter = (iter->second).begin();
			 adjIter !=  (iter->second).end(); ++adjIter   ){
				//iterate through set of countries adjacent
				if( colorMap.find(*adjIter) != colorMap.end()  ){
					//country adjacent has a color
					bool notEqualColor = ( i != colorMap[*adjIter]   );
					//in order to make sure its true for ALL adjacent countries:
					if(possibleColor == true){
						possibleColor = notEqualColor;
					}
					
					if(possibleColor == false){
						break;
					}
					

				}
		}
		//recursing part		
		if( possibleColor  ){
			char charval = iter->first;
			colorMap.insert(std::make_pair(charval, i));
			std::map<char, set<char>> ::iterator newIter = iter;
			bool isTrue = assigningColors( countryMap, colorMap, ++newIter );
		
			if(true == isTrue){
				return true;
			}		
			
			if(!isTrue){
				colorMap.remove(iter->first);
				
			}
			
			
		}
	}
				
	return false;
}

bool isSameSize( std::map<char, set<char>> &countryMap, ANSWER_MAP_T &colorMap ){
	int countryMapNum = 0;
	for( std::map<char, set<char>> ::iterator it = countryMap.begin(); it != countryMap.end(); ++it  ){
		countryMapNum++;
	}	

	int colorMapNum = 0;
	for( AVLTree<char, int> ::iterator it = colorMap.begin(); it != colorMap.end(); ++it  ){
		colorMapNum++;
	}	

	return (countryMapNum == colorMapNum);
}
