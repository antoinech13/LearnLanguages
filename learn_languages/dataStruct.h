#pragma once

#include <vector>
#include <string>
#include <iostream>

using uint = unsigned int;

template <typename T>
using matrice = std::vector<std::vector<T>>;
using set = std::vector<uint>;



enum class Section {
	None,
	Langue,
	Pos,
	Direct,
	Forms,
	Concepts,
	ConceptLabels,
	Links,
	Verbs
};

struct VerbTense {
	unsigned int languageIdx;
	std::string name;
	std::vector<std::string> forms;

	void print() const {
		std::cout << "Language Index: " << languageIdx << " name: " << name << " forms: ";

		for (const std::string& form : forms)
			std::cout << form << " ";

		std::cout << std::endl;
	};

};

struct VerbEntry {
	std::vector<std::string> infinitives;
	std::vector<VerbTense> tenses;

	void print() const {
		std::cout << "Infinitives: ";
		for (const std::string& inf : infinitives)
			std::cout << inf << " ";
		std::cout << std::endl;
		for (const VerbTense& tense : tenses)
			tense.print();
	};
};

struct Word {
	uint id, idType, langueIdx;
	std::string word;
	
	Word() = default;
	Word(const uint id, const uint idType, const std::string& word, const uint langueIdx) : id(id), idType(idType), word(word), langueIdx(langueIdx) {}

	void print() const {
		std::cout << "Word ID: " << id << " Type ID: " << idType << " Language Index: " << langueIdx << " Word: " << word << std::endl;
	}
};

struct Langue {
	uint id;
	std::string name;
	

	std::vector<uint> wordsIdx;
	

	Langue(uint id, std::string name) : id(id), name(name) {}

	std::vector<uint>::size_type nWords() const {
		return wordsIdx.size();
	}

	void print() const {
		std::cout << "Langue ID: " << id << " Name: " << name << std::endl;
		for (uint idx : wordsIdx) {
			std::cout << "ID: " << idx << std::endl;
		}
	};

};

struct Link
{
	uint idLink;
	uint idConcept;
	uint idWord;

	Link(uint idLink, uint idConcept, uint idWord) : idLink(idLink), idConcept(idConcept), idWord(idWord) {}
};