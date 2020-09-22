#ifndef GITINT_H
#define GITINT_H
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>


typedef int CommitIdx;

struct CommitObj {
    std::string msg_;
    std::map<std::string, int> diffs_;
    CommitIdx parent_;

    
    CommitObj(
        const std::string& msg,
        std::map<std::string, int> diffs,
        CommitIdx parent) :
        msg_(msg), diffs_(diffs), parent_(parent)
    {}
};

class GitInt {
public:
   
    GitInt();

    void print_menu() const;

    bool process_command(std::string cmd_line);

    void create(const std::string& filename, int value);

    void edit(const std::string& filename, int value); 

    void display(const std::string& filename) const;

    void display_all() const;

    void display_commit(CommitIdx commit) const;

    void add(std::string filename);

    void commit(std::string message);

    void create_tag(const std::string& tagname, CommitIdx commit);

    void tags() const;

    bool checkout(CommitIdx commitIndex);

    bool checkout(std::string tag);

    void log() const;

    void diff(CommitIdx to) const;

    void diff(CommitIdx from, CommitIdx to) const;


    bool valid_commit(CommitIdx commit) const;
    
private:


    std::map<std::string, int> buildState(CommitIdx from, CommitIdx to = 0) const;

    void display_helper(const std::map<std::string, int>& dat) const;

    void log_helper(CommitIdx commit_num, const std::string& log_message) const;


	std::map<std::string, int> fileValMap;
	std::set<std::string> addSet;
	CommitIdx head;
	//vector of commit objects
	std::vector<CommitObj> commitVec;
	std::map<std::string, CommitIdx> tagMap;
	std::vector<std::string> tagVec;
	
};

#endif
