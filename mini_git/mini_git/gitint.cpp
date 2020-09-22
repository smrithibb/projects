-#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "gitint.h"

using namespace std;

const std::string INVALID_COMMAND = "Invalid command";
const std::string INVALID_OPTION = "Invalid option";
const std::string INVALID_COMMIT_NUMBER = "Invalid commit number";
const std::string LOG_COMMIT_STARTER = "Commit: ";



GitInt::GitInt(){
	std::map<std::string, int> garbageMap;
	CommitObj garbageObject("", garbageMap, -1);
	commitVec.push_back(garbageObject); 
	head = 0;
}

void GitInt::print_menu() const
{
    cout << "Menu:                          " << endl;
    cout << "===============================" << endl;
    cout << "create   filename int-value    " << endl;
    cout << "edit     filename int-value    " << endl;
    cout << "display  (filename)            " << endl;
    cout << "display  commit-num            " << endl;
    cout << "add      file1 (file2 ...)     " << endl;
    cout << "commit   \"log-message\"       " << endl;
    cout << "tag      (-a tag-name)         " << endl;
    cout << "log                            " << endl;
    cout << "checkout commit-num/tag-name   " << endl;
    cout << "diff                           " << endl;
    cout << "diff     commit                " << endl;
    cout << "diff     commit-n commit-m     " << endl;
}


bool GitInt::process_command(std::string cmd_line)
{
    bool quit = false;
    std::stringstream ss(cmd_line);
    std::string cmd;
    ss >> cmd;
    if (ss.fail()) throw std::runtime_error(INVALID_COMMAND);

    if (cmd == "quit") {
        quit = true;
    }
	if(cmd == "create"){
		string filename;
		int value;
		ss >> filename;
		if(ss.fail()){
			throw std::runtime_error(INVALID_COMMAND);
		}
		ss.clear();
		ss >> value;
		if(ss.fail()){
			throw std::runtime_error(INVALID_COMMAND);
		}
		create(filename, value);
		
	}

	else if(cmd == "edit"){
		string filename;
		int value;
		ss >> filename;
		if(ss.fail() ){
			throw std::runtime_error(INVALID_COMMAND);
		}
		
		ss >> value;
		if(ss.fail()){
			throw std::runtime_error(INVALID_COMMAND);
		}
		edit(filename, value);
	
	}

	else if(cmd == "display"){

		CommitIdx value;
		ss >> value;
		if(! ss.fail() ){
			display_commit(value);
	
		}

		else{
			ss.clear();
			string filename;
			ss >> filename;
			if( ! ss.fail() ){
				display(filename);
			}
			else{
				display_all();
			
			}

		}

	}
	else if (cmd == "add"){
		
		string filename;
		while( ss >> filename ){
			add(filename);
		}
		
	}

	else if (cmd == "commit"){
		
		string emptyString;
		string fullString;
		string actualString;
		char firstQuote;
		ss >> firstQuote;
		if(firstQuote != '"'){
			throw std::runtime_error(INVALID_COMMAND);
		}

		std::getline(ss, fullString);
		char tempChar = '"';
		if((int)(fullString.find(tempChar)) == -1){
			//if there is no second "
			throw std::runtime_error(INVALID_COMMAND);
			
		}

	
		if(ss.fail() ){
		
			throw std::runtime_error(INVALID_COMMAND);
			
		}
		stringstream newSS(fullString);
		std::getline(newSS, actualString, '"');
		
		if(actualString.empty() ){
			throw std::runtime_error(INVALID_COMMAND);
		}
		
		commit(actualString);

}

	else if(cmd == "tag"){
		//to call tags() or tag(tagname)
		string firstPart;
		string tagName;
		
		ss >> firstPart;
		if ( ss.fail()  ){
			tags();
		}
		

		else{
			ss.clear();
			if(firstPart != "-a"){
				throw std::runtime_error(INVALID_COMMAND);
			}
			ss >> tagName;
			if(!ss){
				throw std::runtime_error(INVALID_COMMAND);
			}
	
			create_tag(tagName, head);
		}
		

	}

	else if(cmd == "log"){
		log();
	}

	else if(cmd == "checkout"){
		CommitIdx value;
		ss >> value;
		
		if(! ss.fail() ){
			checkout(value);
			return quit;
	
	
		}
		ss.clear();
		
		string tag;
		ss >> tag;
		if( ! ss.fail() ){
			checkout(tag);
			return quit;
		
		}
		throw std::runtime_error(INVALID_COMMAND);

	}

	else if("diff" == cmd){
		//3 options for diff call
		CommitIdx valueOne;
		CommitIdx valueTwo;

		ss >> valueOne;
		if (! ss.fail() ){
			ss >> valueTwo;

			if (! ss.fail() ){
				//if there are 2 ints in stringstream
				//diff takes max of 2 arguments
				diff(valueOne, valueTwo);
			}

			else{
				diff(valueOne);
			}
		}

		else{
			//if no ints in stringstream: print differences between
			//current file state and last commit
			diff(head);
		}
	

	}

	else if("quit" == cmd){
		quit = true;
	}

    return quit;
}

//create a file in map with value of the integer passed in
void GitInt::create(const std::string& filename, int value){
	if(fileValMap.find(filename) == fileValMap.end()){
		fileValMap.insert( make_pair(filename, value) );
	}
	else{
		throw std::invalid_argument(INVALID_COMMAND);
	}
	
}

//modify the data value in the file
void GitInt::edit(const std::string& filename, int value){
	
	if(fileValMap.find(filename) == fileValMap.end()){
		throw std::invalid_argument(INVALID_COMMAND);
	}
	else{
		//changing data in file
		fileValMap.find(filename)->second = value;
	}


}


//displays the name of file and content of that one file
void GitInt::display(const std::string& filename) const{
	if(fileValMap.find(filename) == fileValMap.end()){
		throw std::invalid_argument(INVALID_COMMAND);
	}
	std::cout << filename << " : " << fileValMap.at(filename) << std::endl;

}

//check finish display_all and display_commit depending on if need 
//top loop through multiple commits
void GitInt::display_all() const{
	if( commitVec.empty() ){
		//no files
		return;
	}
	display_helper(fileValMap);

}

void GitInt::display_commit(CommitIdx commit) const
{
    if ( false == valid_commit(commit) ) {
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    display_helper(commitVec[commit].diffs_);
}



void GitInt::display_helper(const std::map<std::string, int>& dat) const
{
    for (std::map<std::string, int>::const_iterator cit = dat.begin();
            cit != dat.end();
            ++cit) {
        std::cout << cit->first << " : " << cit->second << std::endl;
    }
}


void GitInt::log_helper(CommitIdx commit_num, const std::string& log_message) const
{
    std::cout << LOG_COMMIT_STARTER << commit_num << std::endl;
    std::cout << log_message << std::endl << std::endl;

}


void GitInt::add(std::string filename){
	if(fileValMap.find(filename) == fileValMap.end()){
		//error if filename does not exist
		throw std::invalid_argument(INVALID_COMMAND);
	}
	addSet.insert( filename );

}


//get to the original values/current state values by going through the parent commit diff maps 
//and adding the diff values until the initial commit is found
std::map<std::string, int> GitInt::buildState(CommitIdx from, CommitIdx to) const{

	map<std::string, int> currMap;

	
	while(from != to){
		
			//iterate through diff map for a specific commit
			for(map<string, int>::const_iterator it = ( (commitVec[from].diffs_).begin() ); 
				it != ( (commitVec[from].diffs_).end() ); ++it){
					// for loop going through map of diff’s


				
				if(currMap.find(it->first) == currMap.end()){
					//if file isnt there, add with makepair
					currMap.insert(make_pair(it->first, it->second));

				}
				else{
					//if file is there, then update it
					currMap[it->first] += it->second;
				}
				
		}

		from = commitVec[from].parent_;
	}

	return currMap;
	
}


void GitInt::commit(std::string message){
//commits all staged files

	if(addSet.empty() ){
		throw std::runtime_error(INVALID_COMMAND);
	}

	std::map<std::string, int> buildMap = buildState(head);
	std::map<std::string, int> diffMap;


	for(std::set<string>::const_iterator it=addSet.begin(); it!=addSet.end(); ++it){
		if(buildMap.find(*it) != buildMap.end()){
			if( (fileValMap[*it]  - buildMap[*it]) != 0){
				diffMap[*it] =  fileValMap[*it]  - buildMap[*it];
			}

		}
		else{
			diffMap.insert( make_pair(*it, fileValMap[*it] ) );
		}
	
	}
	

		CommitObj commitObject(message, diffMap, head);
		commitVec.push_back(commitObject);
		head = commitVec.size() - 1;

		addSet.clear();
		
}

void GitInt::create_tag(const std::string& tagname, CommitIdx commit){
	if(tagMap.find(tagname) != tagMap.end()){
		throw std::invalid_argument(INVALID_OPTION);
	}

	
	tagMap.insert( make_pair(tagname, commit) );
	tagVec.push_back(tagname);
}

void GitInt::tags() const{
	
	for(int i= (tagVec.size() - 1); i >= 0; i--){
		std::cout << tagVec[i] << std::endl;
	}
}


bool GitInt::checkout(CommitIdx commitIndex){
	if(valid_commit(commitIndex) == false){
		throw std::invalid_argument(INVALID_COMMIT_NUMBER);
	}

	addSet.clear();
	head = commitIndex;
	map<std::string, int> checkoutMap = buildState(head);
	fileValMap.clear();
	for(std::map<string, int>::const_iterator it=checkoutMap.begin(); 
		it!= checkoutMap.end(); ++it){
		fileValMap.insert( make_pair( it->first, it->second ) );
	}
	return true;
}

    
bool GitInt::checkout(std::string tag){
	if(   ( (tagMap.find(tag)) == tagMap.end() ) || (valid_commit(tagMap.find(tag)->second) == false) ){
		throw std::invalid_argument(INVALID_COMMIT_NUMBER);
	}
	head = tagMap[tag];
	addSet.clear();
	map<std::string, int> checkoutMap = buildState(head);
	
	fileValMap.clear();
	for(std::map<string, int>::const_iterator it=checkoutMap.begin(); 
		it!= checkoutMap.end(); ++it){
		fileValMap.insert( make_pair( it->first, it->second ) );
	}
	
	return true;
}


//display commit numbers and log messages using for loop
void GitInt::log() const{

	for(unsigned int i = head; i > 0; i = commitVec[i].parent_){
		log_helper(i, commitVec[i].msg_);
	}


}

//print the difference in values from current state to 
//a previous commit that is passed in
void GitInt::diff(CommitIdx to) const{
	if(   (valid_commit(to) == false)  ) {
		throw std::invalid_argument(INVALID_COMMIT_NUMBER );
	}
	
	std::map<std::string, int> buildMap = buildState(to);

	for(std::map<string, int>::const_iterator it=fileValMap.begin(); 
		it!=fileValMap.end(); ++it){
		if(to == 0){
			buildMap.insert(make_pair( (it->first), fileValMap.at(it->first) ));
		}	

		else if(buildMap.find(it->first) != buildMap.end() ){
			buildMap[it->first] = fileValMap.at(it->first) - buildMap[it->first];
			if(buildMap[it->first] == 0){
				buildMap.erase(it->first);
			}

		}

		else{
			buildMap.insert( make_pair( (it->first), fileValMap.at(it->first) ) );
		}

		if(buildMap.find(it->first) != buildMap.end()){
			std::cout << it->first << " : " << buildMap[it->first] << endl;
		}
	}
			

}


//print the difference in values from a specified previous commit to 
//another specified previous commit that is passed in
void GitInt::diff(CommitIdx from, CommitIdx to) const{
	if(from < to){
		throw std::invalid_argument(INVALID_OPTION);
	}

	if(  (valid_commit(from) == false) || (valid_commit(to) == false)  ) {
		throw std::invalid_argument(INVALID_COMMIT_NUMBER );
	}

	std::map<std::string, int> buildMapFrom = buildState(from);
	std::map<std::string, int> buildMapTo = buildState(to);
	for(std::map<string, int>::const_iterator it=buildMapFrom.begin(); 
		it!=buildMapFrom.end(); ++it){		
		if(to == 0){ 
			std::cout << it->first << " : " << buildMapFrom[it->first] << endl;
		}
		else if(buildMapTo.find(it->first) != buildMapTo.end()){
			buildMapFrom[it->first] = buildMapFrom[it->first]-buildMapTo[it->first];
			if(buildMapFrom[it->first] == 0){
				buildMapFrom.erase(it->first);
		
			}
			std::cout << it->first << " : " << buildMapFrom[it->first] << endl;  
		}
		
		else{
			std::cout << it->first << " : " << buildMapFrom[it->first] << endl;
		}	
		
	}
}
   
  

//checks if the commit number is valid
bool GitInt::valid_commit(CommitIdx commit) const{	
	if( ( commit < (int)commitVec.size() ) && (commit >= 0) ){
		return true;
	}
	return false;
}