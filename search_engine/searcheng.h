#ifndef SEARCHENG_H
#define SEARCHENG_H

#include <map>
#include <vector>
#include <string>
#include "webpage.h"
#include "pageparser.h"
#include "wpscombiner.h"





/**
 * Provides parsing and indexing of search terms as well as search operations.
 * Provides webpage display and retrieval.
 */
class SearchEng {
public:

    SearchEng(PageParser* noExtensionParser);

    ~SearchEng();

    void register_parser(const std::string& extension, PageParser* parser);

    void read_pages_from_index(const std::string& index_file);

    WebPage* retrieve_page(const std::string& page_name) const;

    void display_page(std::ostream& ostr, const std::string& page_name) const;

    WebPageSet search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const;

    std::vector<std::pair<WebPage*, double> > pageRank(const WebPageSet& in_pages);

	std::map<WebPage*, WebPageSet> helperExpandedCandidate( const WebPageSet& in_pages);

    int helperFindIndex( std::vector<std::pair<WebPage*, double> > vec, WebPage* webpageptr  );


private:
    void read_page(const std::string& filename);

	std::map<std::string, PageParser*> registeredParsers;
	PageParser* noExtensionParser_;
	std::map<std::string, WebPage*> filenameWebpageMap;
	std::map<std::string, WebPageSet> wordsWebpageMap;
	std::set<WebPage*> dynamicSet;
	std::map<WebPage*, WebPageSet> pageGraph;
	

    


};

#endif
