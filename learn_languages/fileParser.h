#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <ranges>

#include "./utils.h"

template <typename T>
using matrice = std::vector<std::vector<T>>;

enum class Section { 
	None, 
	Langue, 
	Words, 
	Verbs 
};

struct VerbTense {
	int languageIdx;
	std::string name;
	std::vector<std::string> forms;

	void print() const{
		std::cout << "Language Index: " << languageIdx << " name: " << name << " forms: ";

		for( const std::string& form: forms) 
			std::cout << form << " ";
		
		std::cout << std::endl;
	};

};

struct VerbEntry {
	std::vector<std::string> infinitives;
	std::vector<VerbTense> tenses;

	void print() const {
		std::cout << "Infinitives: ";
		for (const std::string& inf: infinitives)
			std::cout << inf << " ";
		std::cout << std::endl;
		for (const VerbTense& tense: tenses)
			tense.print();
	};
};

class Parser
{

public:

	Parser(const std::string& path);
	~Parser();

	const std::vector<std::string> getLanguages() const;
	const int getWordsNumber() const;
	const int getLanguageIdx(const std::string& language) const;

	const std::string& getWord(int wordIdx, int languageIdx) const;


private:

	const std::string m_path;

	std::unordered_map<std::string, int> m_languagesToIdx;
	std::vector<VerbEntry> m_verbs;


	matrice<std::string> m_words;

	void parse();
	void parseLanguages(const std::string& line);
	void parseWords(const std::string& line);
	void parseVerbs(const std::string& line);


};