#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "util.h"


using namespace std;



SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    

	this->noExtensionParser_ = noExtensionParser;


}


//deallocate registered parsers and webpage*
SearchEng::~SearchEng(){

	for(std::map<std::string, WebPage*>::iterator it = filenameWebpageMap.begin(); 
		it != filenameWebpageMap.end(); ++it){
	
			delete it->second;
	}
	for(std::map<std::string, PageParser*>::iterator it = registeredParsers.begin(); 
		it != registeredParsers.end(); ++it){
			delete it->second;
	}

	delete noExtensionParser_;

}



//register parser with a file extension if it is unused before
void SearchEng::register_parser(const std::string& extension, PageParser* parser){
	if(registeredParsers.find(extension) == registeredParsers.end() ){
		std::string stringEx = extension;
		registeredParsers.insert( std::make_pair( stringEx, parser )  );
	}
}



void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

void SearchEng::read_page(const std::string& filename){
	std::set<std::string> allSearchableTerms;
    std::set<std::string> allOutgoingLinks;
	WebPageSet pageRankSet; 
	

	std::string extensionType = extract_extension(filename);
	std::map<std::string, PageParser*>::iterator extParserIter = registeredParsers.find(extensionType);
	if(extParserIter != registeredParsers.end() ){
		(registeredParsers[extensionType])->parse(filename, allSearchableTerms, allOutgoingLinks);
	}
	
	else{
		noExtensionParser_->parse(filename, allSearchableTerms, allOutgoingLinks);
	}


	std::map<std::string, WebPage*>::iterator findCurrPage = filenameWebpageMap.find(filename);
	if(findCurrPage == filenameWebpageMap.end()){

		WebPage* currPage = new WebPage(filename);
		filenameWebpageMap.insert( std::make_pair(filename, currPage ) );
		for(std::set<std::string>::iterator it=allOutgoingLinks.begin(); it != allOutgoingLinks.end(); ++it){

			if( filenameWebpageMap.find(*it) != filenameWebpageMap.end() ){
		
				currPage->add_outgoing_link( filenameWebpageMap[*it] );
				(filenameWebpageMap.find(*it))->second->add_incoming_link(currPage);
				pageRankSet.insert( filenameWebpageMap[*it]  );		
			}

			else{
			
				WebPage* searchWebpage = new WebPage(*it);
				currPage->add_outgoing_link(searchWebpage);
				searchWebpage->add_incoming_link(currPage);
				filenameWebpageMap.insert( std::make_pair(*it, searchWebpage ) );
				pageRankSet.insert( searchWebpage );	
			}	
		}
		//add the self loop for pagerank
		pageRankSet.insert( currPage );
		//adding it into the map to use in pagerank 
		pageGraph.insert( std::make_pair( currPage, pageRankSet ) );	

		

	}

	else{
		//current webpage IS in the map
		for(std::set<std::string>::iterator it=allOutgoingLinks.begin(); it != allOutgoingLinks.end(); ++it){
			//updates outgoing and incoming links
			if( filenameWebpageMap.find(*it) != filenameWebpageMap.end() ){
				findCurrPage->second->add_outgoing_link((filenameWebpageMap.find(*it))->second);
				filenameWebpageMap.find(*it)->second->add_incoming_link( findCurrPage->second   );
				pageRankSet.insert( filenameWebpageMap[*it]  );	
			}
			else{
				WebPage* searchWebpage = new WebPage(*it);
				findCurrPage->second->add_outgoing_link( searchWebpage );
				searchWebpage->add_incoming_link(findCurrPage->second );
				filenameWebpageMap.insert(std::make_pair(searchWebpage->filename(), searchWebpage));
				pageRankSet.insert( searchWebpage );

			}
		}
		//add the self loop for pagerank
		pageRankSet.insert( findCurrPage->second );
		pageGraph.insert( std::make_pair(findCurrPage->second, pageRankSet ) );
			

	}
	std::set<std::string>::iterator searchableTermIterator;
	for(searchableTermIterator = allSearchableTerms.begin(); 
		searchableTermIterator != allSearchableTerms.end(); ++searchableTermIterator ){
			
		if( wordsWebpageMap.find(*searchableTermIterator) == wordsWebpageMap.end() ){
			WebPageSet pageSet;
			pageSet.insert(filenameWebpageMap.at(filename) );
			wordsWebpageMap.insert( std::make_pair(*searchableTermIterator, pageSet ) );
		}
		else{
			std::map<std::string, WebPage*>::iterator searchIterator = filenameWebpageMap.find(filename);
			wordsWebpageMap[*searchableTermIterator].insert( searchIterator->second );

		}

	}
	
}




WebPage* SearchEng::retrieve_page(const std::string& page_name) const{
	if(filenameWebpageMap.find(page_name) == filenameWebpageMap.end() ){
		return NULL;
	}
	else{
		return (filenameWebpageMap.find(page_name))->second;
	}
}


void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const{

	if(filenameWebpageMap.find(page_name) == filenameWebpageMap.end()  ){
		throw std::logic_error("pagename to display does not exist");
	}

	std::string extensionType = extract_extension(page_name);
	if(registeredParsers.find(extensionType) ==  registeredParsers.end()  ){
		throw std::invalid_argument("No parser registered for extension type");
	}

ostr << (registeredParsers.at(extensionType))->display_text(page_name);
	
}



WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const{
	WebPageSet tempWebPageSet;
	WebPageSet emptySet;

	if(terms.empty() == true){
		return emptySet;
	}

	if(terms.size() == 1){
		if(wordsWebpageMap.find(terms[0]) == wordsWebpageMap.end()){
			return emptySet;
		}
		return wordsWebpageMap.at( terms[0] );
	}

	if(wordsWebpageMap.find(terms[0]) != wordsWebpageMap.end()){
		tempWebPageSet = (wordsWebpageMap.at(   terms[0]   )  );
	}


	for(unsigned int i=1; i < terms.size(); i++){

		if( wordsWebpageMap.find(terms[i]) == wordsWebpageMap.end()  ){
			tempWebPageSet  = combiner->combine( tempWebPageSet,  emptySet );

		}

		else{
			WebPageSet tempCompareSet = wordsWebpageMap.at( terms[i]);
			tempWebPageSet  = combiner->combine( tempWebPageSet,  tempCompareSet );

		}
	
	}


	return tempWebPageSet;
}



std::vector<std::pair<WebPage*, double> > SearchEng::pageRank(const WebPageSet& in_pages){
	std::map<WebPage*, WebPageSet> expandedCandidates = helperExpandedCandidate(in_pages);

	//filtering out incoming and outgoing links
	std::map<WebPage*, WebPageSet> actualIncomingLinks;
	std::map<WebPage*, WebPageSet> actualOutgoingLinks;

	for(std::map<WebPage*, WebPageSet>::iterator CandIT = expandedCandidates.begin(); 
		CandIT != expandedCandidates.end(); ++CandIT){
			WebPageSet newIncomingLinks;
			WebPageSet newOutgoingLinks;
			WebPageSet originalIncoming = (CandIT->first)->incoming_links();
			WebPageSet originalOutgoing = (CandIT->first)->outgoing_links();
			for( WebPageSet::iterator it = originalIncoming.begin(); it != originalIncoming.end(); ++it  ){
				//adding the incoming links that are in the expanded candidate set
				if( expandedCandidates.find(*it) != expandedCandidates.end()   ){
					newIncomingLinks.insert(*it);
				}
			}
			//add self loop
			newIncomingLinks.insert(CandIT->first);
			actualIncomingLinks.insert(std::make_pair( CandIT->first, newIncomingLinks ));
			for( WebPageSet::iterator it = originalOutgoing.begin(); it != originalOutgoing.end(); ++it  ){
				if( expandedCandidates.find(*it) != expandedCandidates.end()   ){
					newOutgoingLinks.insert(*it);
				}
			}
			//add self loop
			newOutgoingLinks.insert(CandIT->first);
			actualOutgoingLinks.insert(std::make_pair( CandIT->first, newOutgoingLinks ));
	}


	std::vector<std::pair<WebPage*, double> > rankVec;
	std::vector<std::pair<WebPage*, double> > tempVec;
	int N = expandedCandidates.size();
		for( std::map<WebPage*, WebPageSet>::iterator candIT = expandedCandidates.begin(); 
		candIT != expandedCandidates.end(); ++candIT ){
			rankVec.push_back(std::make_pair( candIT->first, (1.0/ N )  )  );
			tempVec.push_back(std::make_pair( candIT->first, (1.0/ N )  )  );	
	}


	int t = 20;
	double e = 0.15;
	double newPr;

		for(int i=0; i < t; i++){ 
			int index; 
			for(std::map<WebPage*, WebPageSet>::iterator candIT = expandedCandidates.begin(); 
				candIT != expandedCandidates.end(); ++candIT){

				double outSum = 0.0;
				for( WebPageSet::iterator it = (actualIncomingLinks[candIT->first]).begin(); 
					it != (actualIncomingLinks[candIT->first]).end(); ++it ){

						int forIndex = helperFindIndex(rankVec, *it);
						double Pr = rankVec[forIndex].second;
						WebPageSet actualOutDeg = actualOutgoingLinks[*it];
						outSum = outSum + (Pr / (actualOutDeg.size() ));
				}
				

				
				newPr = ( e / N ) + ( (1.0 - e) * ( outSum ) );

				index = helperFindIndex(rankVec, (candIT->first));
				tempVec[index].second = newPr;

		}

		for( unsigned int i=0; i < tempVec.size(); i++  ){
			rankVec[i].first = tempVec[i].first;
			rankVec[i].second = tempVec[i].second;
		}

	}

	return rankVec;
}



int SearchEng::helperFindIndex( std::vector<std::pair<WebPage*, double> > vec, WebPage* webpageptr  ){
	int indexNOTexist = -1;
	for(int i = 0; i < (int)vec.size(); i++  ){
		if( vec[i].first == webpageptr ){
			return i;
		}
	}
	return indexNOTexist;
}

//helper function for page rank to get the expanded candidate set from the input set
std::map<WebPage*, WebPageSet> SearchEng::helperExpandedCandidate( const WebPageSet& in_pages ){

	//get the expanded candidate set of webpages
	WebPageSet allCandidatesSet; 
	std::map<WebPage*, WebPageSet> expandedCandidate;
	for( std::set<WebPage*>::const_iterator it = in_pages.begin(); it != in_pages.end(); ++it ){
		allCandidatesSet.insert(*it);

		//find the outgoing & incoming links of the things in in_pages to set to be added to the expanded candidate
		WebPageSet outLinksToAdd = (*it)->outgoing_links();
		WebPageSet inLinksToAdd = (*it)->incoming_links();
		for( std::set<WebPage*>::iterator outIt = outLinksToAdd.begin(); outIt != outLinksToAdd.end(); ++outIt ){
			if( allCandidatesSet.end() == allCandidatesSet.find( *outIt) ){
				allCandidatesSet.insert(*outIt);

			}
		}

		for( std::set<WebPage*>::iterator inIt = inLinksToAdd.begin(); inIt != inLinksToAdd.end(); ++inIt ){
			if( allCandidatesSet.end() == allCandidatesSet.find( *inIt) ){
				allCandidatesSet.insert(*inIt);


			
			}
		}
	
	}


	//to actually insert into the expanded candidate
	for(std::map<WebPage*, WebPageSet>::iterator ReadPageIT=pageGraph.begin(); 
	ReadPageIT != pageGraph.end(); ++ReadPageIT){
		WebPageSet tempSet = ReadPageIT->second;
		WebPageSet tempToAddSet;
		if(allCandidatesSet.find(ReadPageIT->first) != allCandidatesSet.end()   ){
			for(std::set<WebPage*>::iterator setIter = tempSet.begin(); setIter != tempSet.end(); ++setIter ){
				if( allCandidatesSet.find(*setIter ) != allCandidatesSet.end() ){
					//need to add this into exapnded candidate data structure
					if(tempToAddSet.find(*setIter) == tempToAddSet.end() )	{
						tempToAddSet.insert(*setIter);
					}
				
				}
			}
		}
		if(tempToAddSet.empty() == false){
			//adding to the actual expanded candidate data structure
			if(  expandedCandidate.find(ReadPageIT->first  ) ==  expandedCandidate.end()){
				expandedCandidate.insert( std::make_pair( ReadPageIT->first, tempToAddSet  )   );
			}
			

		}

	}

	return expandedCandidate;
}