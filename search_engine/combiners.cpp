#include "searcheng.h"
#include "combiners.h"

//return set of WebPage* with files in both setA and setB
WebPageSet ANDWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet AndSet;
	for(std::set<WebPage*>::iterator it = setA.begin(); it != setA.end(); ++it){
		std::set<WebPage*>::iterator itB = setB.find(*it);
		if(itB != setB.end() ){
			if(AndSet.find(*it) == AndSet.end()){
				AndSet.insert(*it);
			}	
		}
	}
	return AndSet;

}

//return a set of WebPage* with all files in either setA or setB
WebPageSet ORWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet toAddTo;
	for(std::set<WebPage*>::iterator it = setA.begin(); it != setA.end(); ++it){
		if(  toAddTo.find(*it) == toAddTo.end()  ){
			toAddTo.insert(*it);
		}
	}

	for(std::set<WebPage*>::iterator it = setB.begin(); it != setB.end(); ++it){
		
		if(toAddTo.find(*it) == toAddTo.end() ){
			toAddTo.insert(*it);		
		}
	
	}
	return toAddTo;	
}


//return set of WebPage* with only words in setA
WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet returnSet;
	for(std::set<WebPage*>::iterator it = setA.begin(); it != setA.end(); ++it){
		if(  returnSet.find(*it) == returnSet.end()  ){
			returnSet.insert(*it);
		}
	}

	for(std::set<WebPage*>::iterator it = setB.begin(); it != setB.end(); ++it){
		
		if(returnSet.find(*it) != returnSet.end() ){
			//erase if a webPage is in returnSet already
			returnSet.erase( *it  );
			
		}
	}
	return returnSet;
}
