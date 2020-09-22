#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <string>
#include <iostream>
#include <vector>
#include <set>

class WebPage;

typedef std::set<WebPage*> WebPageSet;
typedef std::set<std::string> StringSet;

/****** THIS CLASS IS COMPLETED AND NEED NOT BE MODIFIED **********/

class WebPage
{
public:

    WebPage();

    WebPage(std::string filename);


    ~WebPage();


    void filename(std::string fname);

    std::string filename() const;

   
    void all_terms(const StringSet& words);

    StringSet all_terms() const;

 
    void add_incoming_link(WebPage* wp);


    WebPageSet incoming_links() const;


    void add_outgoing_link(WebPage* wp);


    WebPageSet outgoing_links() const;



private:
    std::vector<std::string> strip(std::string word);
    WebPageSet in_links_;
    WebPageSet out_links_;
    StringSet terms_;
    std::string filename_;

};
#endif
