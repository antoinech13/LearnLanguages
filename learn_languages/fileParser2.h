#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <ranges>
#include <algorithm>
#include <ranges>

#include "./utils.h"
#include "./dataStruct.h"


class Parser
{

public:

	Parser(const std::string& path);
	~Parser();

	std::vector<std::string> getLanguages() const;
	const uint getLanguageIdx(const std::string& language) const;
	const uint getWordsNumber(const uint languageIdx) const;
	const Word& getWordByIdx(const uint wordIdx, const uint languageIdx) const;
	

	const std::vector < std::string> getCorrespondingWords(const Word& word, const uint fromLanguageIdx, const uint toLanguageIdx) const;


private:

	const std::string m_path;

	std::unordered_map<std::string, uint> m_languagesToIdx;
	std::vector<Langue> m_langues;
	std::unordered_map<uint, Word> m_forms;
	std::unordered_map<uint, std::string> m_concepts, m_conceptLabels;
	std::unordered_map<uint, Link> m_links;
	std::unordered_map<uint, set> m_direct;
	std::unordered_map<uint, std::string> m_pos;

	std::vector<VerbEntry> m_verbs;


	matrice<std::string> m_words;

	void parse();
	void parseLanguages(const std::string& line);
	void parsePos(const std::string& line);
	void parseForms(const std::string& line);
	void parseConcepts(const std::string& line);
	void parseLinks(const std::string& line);
	void parseDirect(const std::string& line);
	void parseVerbs(const std::string& line);
	void parseConceptLabel(const std::string& line);

	bool isWordInDirectWithLang(const uint wordId, const uint langueIdx) const;
	std::vector<std::string> searchDirect(const Word& word, const uint langueIdx) const;

	template<typename T>
	void commonSetPart(const uint wordId, const uint targetLangueIdx, T&& f) const;

	bool isWordInConcepts(const uint wordId) const;
	std::vector<std::string> searchConcepts(const Word& word, const uint langueIdx) const;

};


