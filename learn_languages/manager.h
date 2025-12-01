#pragma once

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
	//void meaningWords() const;
	//void learnVerbs() const;

};