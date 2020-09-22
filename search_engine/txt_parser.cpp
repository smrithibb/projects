#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "txt_parser.h"
#include "util.h"


using namespace std;


void TXTParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }

    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    string term = "";
    string link = "";

    // Get the first character from the file.
    char c = wfile.get();

    // Continue reading from the file until input fails.
    while(!wfile.fail())
    {
        if (!isalnum(c))
        {
            if(term != "")
            {
                term = conv_to_lower(term);
                allSearchableTerms.insert(term);
            }
            term = "";
        }
        else
        {
            term += c;
        }

        // Attempt to get another character from the file.
        c = wfile.get();
    }
  
    if(term != "")
    {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }

    wfile.close();
}

std::string TXTParser::display_text(std::string filename)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    char c = wfile.get();

    // Continue reading from the file until input fails.
    while (!wfile.fail()) {
        retval += c;
        c = wfile.get();
    }
    return retval;
}



//crawls a file with .txt extension
void TXTParser::crawl(
	const std::map<std::string, PageParser*>& parsers,
	std::string filename,
	std::set<std::string>& processed,
	std::ostream& ofile){

		//start of function
		std::set<std::string> allSearchableTerms;
		std::set<std::string> allOutgoingLinks;
		std::string extensionType = extract_extension(filename);
		std::map<std::string, PageParser*>::const_iterator extParserIter = parsers.find(extensionType);
		if(extParserIter != parsers.end() ){
			(parsers.at(extensionType) )->parse(filename, allSearchableTerms, allOutgoingLinks);
	
		    //dont need to do DFS because txt has no outgoing links
			if(processed.find(filename) == processed.end()){
				processed.insert(filename);
				ofile << filename << std::endl;
			}


		}

} 

