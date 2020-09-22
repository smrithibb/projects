#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h"
#include "pageparser.h"
#include "webpage.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }


	ifstream wfile(argv[1]);
	std::set<std::string> allOutgoingLinks;
	std::set<std::string> processed;
	std::string linkLine;
	std::ofstream ofile;
	ofile.open(argv[2]);

	//make map of parsers we are using
	std::map<std::string, PageParser*> parsers;
	parsers.insert( std::make_pair("txt", new TXTParser)  );
	parsers.insert( std::make_pair("md", new MDParser)  );

	while(getline(wfile, linkLine)){
		std::string extensionType = extract_extension(linkLine);
		std::map<std::string, PageParser*>::iterator it = parsers.find(extensionType);
		if(it != parsers.end() ){
			PageParser* thisParser = it->second;
			thisParser->crawl( parsers, linkLine, processed, ofile );
		}
		
	}

	
	for(std::map<std::string, PageParser*>::iterator it = parsers.begin(); 
		it != parsers.end(); ++it){
			delete it->second;
	}

}





