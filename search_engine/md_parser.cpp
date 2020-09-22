#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;


void MDParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    ifstream wfile(filename.c_str());
    if(!wfile) {
        throw invalid_argument("Bad webpage filename in MDParser::parse()");
    }

    allSearchableTerms.clear();
    allOutgoingLinks.clear();
    PARSE_STATE_T state = NORMALTEXT;

    string term = "";
    string link = "";

    char c = wfile.get();

    while(!wfile.fail())
    {
        if(state == NORMALTEXT)
        {
            if(isalnum( c ) ){
                term = term + c;

            }


            else if( isalnum(c) == false ){

					if(term.empty() == false){
						term = conv_to_lower(term);
                		allSearchableTerms.insert(term);
                		term = "";
					}
					
					if(c == '['){
                    	state = LINKTEXT;
               		}


            }
			
			int h = wfile.peek();
			if (h == EOF) {
				if (wfile.eof() ){
					term = conv_to_lower(term);
					if(term.empty() == false){
						allSearchableTerms.insert(term);
						term = "";

					}


				}
			}
	

        }
        else if (state == LINKTEXT)
        {

            if( isalnum( c ) == false ){
				if(term.empty() == false){
				    term = conv_to_lower(term);
                	allSearchableTerms.insert(term);
               	 	term = "";

				}


                if (c == ']'){
					int peekChar = wfile.peek();
					if(peekChar == '('){
						state = ISLINK;
					}

                	
                }

            }
            
            else{
                term += c;
				int h = wfile.peek();
				if (h == EOF) {
 					if (wfile.eof() ){
						term = conv_to_lower(term);
                		allSearchableTerms.insert(term);
                		term = "";

					 }
				}
            }
            

        }
        else if( state == ISLINK )
        { 
			if( c == '('){
				state = LINKURL;
			}
			else{
				state = NORMALTEXT;
			}

        }
        else
        {
			if(c != ')'){
				link = link + c;


			}
			
			else{
				//c == ')'
                allOutgoingLinks.insert(link);
                link = "";
				state = NORMALTEXT;
			}

        }
        // Attempt to get another character from the file.
        c = wfile.get();
    }

    wfile.close();
}

std::string MDParser::display_text(std::string filename)
{
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    PARSE_STATE_T state = NORMALTEXT;

    char c = wfile.get();

    while (!wfile.fail()) {
        if (state == NORMALTEXT)
        {

            if (c == '[')
            {
                state = LINKTEXT;
            }
            retval += c;
        }
        else if (state == LINKTEXT)
        {
            if (c == ']')
            {
                state = ISLINK;
            }
            retval += c;
        }
        else if (state == ISLINK) {
            if (c == '(') {
                state = LINKURL;
            }
            else
            {
                state = NORMALTEXT;
                retval += c;
            }
        }
        // Else we are in the LINKURL state.
        else
        {
            if (c == ')')
            {
                state = NORMALTEXT;
            }
        }
        c = wfile.get();
    }
    return retval;
}

//crawls through webpages and uses DFS
void  MDParser::crawl(
	const std::map<std::string, PageParser*>& parsers,
	std::string filename,
	std::set<std::string>& processed,
	std::ostream& ofile){
		std::set<std::string> allSearchableTerms;
		std::set<std::string> allOutgoingLinks;
		std::string extensionType = extract_extension(filename);
		std::map<std::string, PageParser*>::const_iterator extParserIter = parsers.find(extensionType);
		if(extParserIter != parsers.end() ){
			(parsers.at(extensionType) )->parse(filename, allSearchableTerms, allOutgoingLinks);
		

			//printing the filename and including it into process before any DFS
			if(processed.find(filename) == processed.end()){
					processed.insert(filename);
					ofile << filename << std::endl;

			}

			//DFS part (still under if statement)
			for(std::set<std::string>::iterator it=allOutgoingLinks.begin(); it != allOutgoingLinks.end(); ++it){
				if(processed.find(*it) == processed.end()){
					//not in processed yet
					processed.insert(*it);
					ofile << *it << std::endl;

					std::string NewExtensionType = extract_extension(*it);
					std::map<std::string, PageParser*>::const_iterator parserIt = parsers.find(NewExtensionType);
					if(parserIt != parsers.end() ){
						//a linked file with no supported parser should be ignored and skipped
						//thus, only execute crawl function with supported parsers
						PageParser* thisParser = parserIt->second;
						std::string tempString = *it;
						thisParser->crawl( parsers, tempString, processed, ofile );
					}
					
				}
					
			}
		}
} 


