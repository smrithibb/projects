#include "cmdhandler.h"
#include "util.h"
#include "combiners.h"
#include "msort.h"
#include "searcheng.h"

using namespace std;

//functor for comparing alphabetically 
struct alphabetical{
	bool operator()( std::pair<WebPage*, double>  word1, std::pair<WebPage*, double>  word2 ){
		std::string first = (word1.first)->filename();
		std::string second = (word2.first)->filename();
		if(first > second){
			return false;
		}
		return true;
	}
};

//functor for sorting by pagerank
struct numerical{
	bool operator()( std::pair<WebPage*, double> num1, std::pair<WebPage*, double>  num2 ){
		double first = num1.second;
		double second = num2.second;

		if(first < second){
			return false;
		}
		return true;
	}
};




QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";

}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}



IncomingHandler::IncomingHandler(){

}

IncomingHandler::IncomingHandler(Handler* next)
	: Handler(next)
{


}

bool IncomingHandler::canHandle(const std::string& cmd) const{
	return cmd == "INCOMING";
}
Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
	string pagename;
	WebPage* page;
	WebPageSet incomingLinkSet;
	instr >> pagename;
	page = eng->retrieve_page(pagename);

	if(page == NULL){
		std::cout << "page doesnt exist in incoming handler" << std::endl;
		return HANDLER_ERROR;
	}
	
	incomingLinkSet = page->incoming_links();
	display_hits(incomingLinkSet, ostr);
	return HANDLER_OK;

}

OutgoingHandler::OutgoingHandler()
{

}
OutgoingHandler::OutgoingHandler(Handler* next)
	: Handler(next)
{

}

bool OutgoingHandler::canHandle(const std::string& cmd) const{
	return cmd == "OUTGOING";
}
Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
	WebPage* page;
	
	string pagename;
	instr >> pagename;
	
	page = eng->retrieve_page(pagename);
	if(page == NULL){
		return HANDLER_ERROR;
	}
	WebPageSet outgoingLinkSet = page->outgoing_links();
	display_hits(outgoingLinkSet, ostr);
	
	return HANDLER_OK;

}


ANDHandler::ANDHandler() {

}

ANDHandler::ANDHandler(Handler *next)
	 : Handler(next)
{
	
}

bool ANDHandler::canHandle(const std::string& cmd) const{
	return cmd == "AND";
}

Handler::HANDLER_STATUS_T ANDHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
	ANDWebPageSetCombiner* combiner = new ANDWebPageSetCombiner();
	std::vector<std::string> searchTerms;
	WebPageSet pagesToDisplay;
	string term;
	while(instr >> term){
		searchTerms.push_back(conv_to_lower(term));
	}
	if(searchTerms.empty() ){
		return HANDLER_OK;
	}
	pagesToDisplay = eng->search(searchTerms, combiner);
	display_hits(pagesToDisplay, ostr);
	delete combiner;
	return HANDLER_OK;
	
}

ORHandler::ORHandler() {

}

ORHandler::ORHandler(Handler *next)
	 : Handler(next)
{
	
	
}

bool ORHandler::canHandle(const std::string& cmd) const{
	return cmd == "OR";
}

Handler::HANDLER_STATUS_T ORHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
	ORWebPageSetCombiner* combiner = new ORWebPageSetCombiner();
	std::vector<std::string> searchTerms;
	WebPageSet pagesToDisplay;
	string term;
	while(instr >> term){
		searchTerms.push_back(conv_to_lower(term));
	}
	if(searchTerms.empty() ){
		return HANDLER_OK;
	}
	pagesToDisplay = eng->search(searchTerms, combiner);
	display_hits(pagesToDisplay, ostr);
	delete combiner;
	return HANDLER_OK;

}


DIFFHandler::DIFFHandler() {

}

DIFFHandler::DIFFHandler(Handler *next)
	 : Handler(next)
{
	
	
}

bool DIFFHandler::canHandle(const std::string& cmd) const{
	return cmd == "DIFF";
}

Handler::HANDLER_STATUS_T DIFFHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
	DiffWebPageSetCombiner* combiner = new DiffWebPageSetCombiner();
	std::vector<std::string> searchTerms;
	WebPageSet pagesToDisplay;
	string term;
	while(instr >> term){
		searchTerms.push_back(conv_to_lower(term));
	}
	if(searchTerms.empty() ){
		return HANDLER_OK;
	}
	pagesToDisplay = eng->search(searchTerms, combiner);
	display_hits(pagesToDisplay, ostr);
	delete combiner;
	return HANDLER_OK;
	
}


PRANDHandler::PRANDHandler(){

}

PRANDHandler::PRANDHandler(Handler *next)
	 : Handler(next)
{
	
	
}

bool PRANDHandler::canHandle(const std::string& cmd) const{
	return cmd == "PRAND";
}

Handler::HANDLER_STATUS_T PRANDHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){

	char sortChar;
	instr >> sortChar;
	if(sortChar != 'r' && sortChar != 'n'){
		return HANDLER_ERROR;
	}
	
	ANDWebPageSetCombiner* combiner = new ANDWebPageSetCombiner();
	std::vector<std::string> searchTerms;
	WebPageSet pagesToDisplay;
	string term;
	while(instr >> term){
		searchTerms.push_back(conv_to_lower(term));
	}
	if(searchTerms.empty() ){
		ostr << "0" << std::endl;
		return HANDLER_OK;
	}
	pagesToDisplay = eng->search(searchTerms, combiner);
	
	std::vector<std::pair<WebPage*, double> > pageRankVec = eng->pageRank(pagesToDisplay);

	if(sortChar == 'r'){
		//sort by page rank
		numerical n;
		mergeSort(pageRankVec, n);
	}

	else if(sortChar == 'n'){
		//sort by alphabetical order
		alphabetical a;
		mergeSort(pageRankVec, a);
	}

	display_ranked_hits( pageRankVec, ostr  );
	delete combiner;
	return HANDLER_OK;

}

PRORHandler::PRORHandler(){

}

PRORHandler::PRORHandler(Handler *next)
	 : Handler(next)
{
	
	
}

bool PRORHandler::canHandle(const std::string& cmd) const{
	return cmd == "PROR";
}

Handler::HANDLER_STATUS_T PRORHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
	char sortChar;
	instr >> sortChar;
	if(sortChar != 'r' && sortChar != 'n'){
		return HANDLER_ERROR;
	}
	ORWebPageSetCombiner* combiner = new ORWebPageSetCombiner();
	std::vector<std::string> searchTerms;
	WebPageSet pagesToDisplay;
	string term;
	while(instr >> term){
		searchTerms.push_back(conv_to_lower(term));
	}
	if(searchTerms.empty() ){
		ostr << "0" << std::endl;
		return HANDLER_OK;
	}

	pagesToDisplay = eng->search(searchTerms, combiner);
	std::vector<std::pair<WebPage*, double> > pageRankVec = eng->pageRank(pagesToDisplay);

	if(sortChar == 'r'){
		//sort by page rank
		numerical n;
		mergeSort(pageRankVec, n);
	}

	else if(sortChar == 'n'){
		//sort by alphabetical order
		alphabetical a;
		mergeSort(pageRankVec, a);
	}

	display_ranked_hits( pageRankVec, ostr  );
	delete combiner;
	return HANDLER_OK;
}


