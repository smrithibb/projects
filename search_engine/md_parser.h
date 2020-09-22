#ifndef MD_PARSER_H
#define MD_PARSER_H

#include <string>
#include <set>

#include "pageparser.h"

class MDParser : public PageParser {
public:

    void parse(std::string filename,
               std::set<std::string>& allSearchableTerms,
               std::set<std::string>& allOutgoingLinks);


    std::string display_text(std::string filename);

	void crawl(
	const std::map<std::string, PageParser*>& parsers,
	std::string filename,
	std::set<std::string>& processed,
	std::ostream& ofile);


#endif
