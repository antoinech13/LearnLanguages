#include "fileParser.h"


Parser::Parser(const std::string& path) : m_path(path) {

	parse();

	for(const VerbEntry& verb: m_verbs) {
		verb.print();
	}

}

Parser::~Parser()
{

}


void Parser::parse() {


	std::ifstream fichier(m_path);

	if (!fichier.is_open()) {
		std::cerr << "Impossible d'ouvrir le fichier: " << m_path << "\n";
		return;
	}
	
	Section current = Section::None;
	std::string line;

	while (std::getline(fichier, line)) {
	
		if (line.empty()) continue;

		if (line[0] == '#') {
			
			if (line.rfind("#LANGUE", 0) == 0) current = Section::Langue;
			else if (line.rfind("#WORDS", 0) == 0) current = Section::Words;
			else if (line.rfind("#VERBS", 0) == 0) current = Section::Verbs;
			else current = Section::None;
			
			continue;
		}

		switch (current) {

		case Section::Langue:
			parseLanguages(line);
			break;

		case Section::Words:
			parseWords(line);
			break;

		case Section::Verbs:
			parseVerbs(line);
			break;

		default:
			break;

		}

	}

}


void Parser::parseLanguages(const std::string& line) {

	std::string::size_type idx = line.find('.');

	if (idx == std::string::npos)
		throw std::runtime_error("Language section error: missing '.'");

	if (idx != line.rfind('.'))
		throw std::runtime_error("Language section error: too many '.' in line");

	int id = std::stoi(line.substr(0, idx));
	std::string name = line.substr(idx + 1);

	m_languagesToIdx[name] = id;

}

void Parser::parseWords(const std::string& line) {

	std::vector<std::string> lineElements = Utils::split(line, '|');

	if(lineElements.size() != m_languagesToIdx.size())
		throw std::runtime_error("Words section error: number of words does not match number of languages");

	m_words.emplace_back( std::move(lineElements) );

}

void Parser::parseVerbs(const std::string& line) {
	

	if (line.find("TENSE") == std::string::npos) {
		
		VerbEntry entry;
		entry.infinitives = Utils::split(line, '|');
		m_verbs.emplace_back(std::move(entry));

		return;
	}

	VerbTense tense;


	std::vector<std::string> paramData = Utils::split(line, '=');
	std::vector<std::string> params = Utils::split(paramData.at(0), ':');
	std::vector<std::string> data = Utils::split(paramData.at(1), '|');

	tense.languageIdx = std::stoi(params[1]);
	tense.name = params[2];

	tense.forms = std::move(data);

	m_verbs.back().tenses.emplace_back(std::move(tense));



}

const std::vector<std::string> Parser::getLanguages() const
{

	if (m_languagesToIdx.empty())
		return {};

	auto keys = m_languagesToIdx | std::views::keys;

	return std::vector<std::string>(keys.begin(), keys.end());
}

const int Parser::getWordsNumber() const
{
	return m_words.size();
}

const int Parser::getLanguageIdx(const std::string& language) const
{

	if( m_languagesToIdx.find(language) == m_languagesToIdx.end() )
		throw std::runtime_error("Language not found: " + language);

    return m_languagesToIdx.find(language)->second;

}

const std::string& Parser::getWord(int wordIdx, int languageIdx) const
{
	if(wordIdx < 0 || wordIdx >= getWordsNumber())
		throw std::runtime_error("Word index out of range: " + std::to_string(wordIdx));

	if(languageIdx < 0 || languageIdx >= m_languagesToIdx.size())
		throw std::runtime_error("Language index out of range: " + std::to_string(languageIdx));

	return m_words[wordIdx][languageIdx];
}
