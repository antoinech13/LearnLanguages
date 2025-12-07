#pragma once

#include <algorithm>	// std::transform 
#include <cstdlib>    // std::srand
#include <ctime> 	   // std::time

#include "fileParser2.h"
#include "./dataStruct.h"



class Manager {

public:

	Manager(const std::string& path);
	Manager(Parser& parser);
	Manager(Parser&& parser);
	
	void start();

private:
	Parser m_parser;
	std::string m_selfLanguage, m_choosedLanguage;


	void displayLanguages(const std::vector<std::string>& languages) const;
	void translateWords() const;
	void meaningWords() const;

	void compareWords(const int idxSelfLanguage, const int idxChoosedLanguage) const;
	bool equalsIgnoreCase(const std::string& str1, const std::string& str2) const;
	
	//void learnVerbs() const;



};