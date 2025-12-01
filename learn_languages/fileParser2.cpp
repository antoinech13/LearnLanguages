#include "fileParser2.h"


Parser::Parser(const std::string& path) : m_path(path) {

	parse();

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
			else if (line.rfind("#POS", 0) == 0) current = Section::Pos;
			else if (line.rfind("#FORMS", 0) == 0) current = Section::Forms;
			else if (line.rfind("#CONCEPTS", 0) == 0) current = Section::Concepts;
			else if (line.rfind("#CONCEPT_LABELS", 0) == 0) current = Section::ConceptLabels;
			else if (line.rfind("#LINKS", 0) == 0) current = Section::Links;
			else if (line.rfind("#DIRECT_SETS", 0) == 0) current = Section::Direct;
			else if (line.rfind("#VERBS", 0) == 0) current = Section::Verbs;
			else current = Section::None;

			continue;
		}

		switch (current) {

		case Section::Langue:
			parseLanguages(line);
			break;

		case Section::Pos:
			//parsePos(line);
			break;

		case Section::Forms:
			parseForms(line);
			break;

		case Section::Concepts:
			parseConcepts(line);
			break;

		case Section::Links:
			parseLinks(line);
			break;

		case Section::Direct:
			parseDirect(line);
			break;

		case Section::ConceptLabels:
			parseConceptLabel(line);
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

	uint id = static_cast<uint>(std::stoul(line.substr(0, idx)));
	std::string name = line.substr(idx + 1);

	m_languagesToIdx[name] = id;
	m_langues.emplace_back(Langue(id, name));

}

void Parser::parsePos(const std::string& line)
{
	std::vector<std::string> lineElements = Utils::split(line, ':');

	if( lineElements.size() != 2)
		throw std::runtime_error("Given line incorrect: " + line);

	uint idPos = static_cast<uint>(std::stoul(lineElements.at(0)));
	std::string posLabel = lineElements.at(1);

	m_pos[idPos] = posLabel;
}

void Parser::parseForms(const std::string& line) {

	std::vector<std::string> lineElements = Utils::split(line, ':');
	
	if (lineElements.size() != 4)
		throw std::runtime_error("Given line incorrect: " + line);

	uint idForm = static_cast<uint>(std::stoul(lineElements.at(0)));
	uint idLang = static_cast<uint>(std::stoul(lineElements.at(1)));
	uint idType = static_cast<uint>(std::stoul(lineElements.at(2)));
	std::string word = lineElements.at(3);

	m_langues[idLang - 1].wordsIdx.push_back(idForm);

	m_forms.try_emplace(idForm, idForm, idType, word, idLang);


}

void Parser::parseConcepts(const std::string& line)
{
	std::vector<std::string> lineElements = Utils::split(line, ':');

	if (lineElements.size() != 2)
		throw std::runtime_error("Given line incorrect: " + line);

	uint idConcept = static_cast<uint>(std::stoul(lineElements.at(0)));
	std::string conceptLabel = lineElements.at(1);

	m_concepts[idConcept] = conceptLabel;


}

void Parser::parseLinks(const std::string& line)
{
	std::vector<std::string> lineElements = Utils::split(line, ':');

	if (lineElements.size() != 3)
		throw std::runtime_error("Given line incorrect: " + line);

	uint idlink = static_cast<uint>(std::stoul(lineElements.at(0)));
	uint idConcept = static_cast<uint>(std::stoul(lineElements.at(1)));
	uint idWord = static_cast<uint>(std::stoul(lineElements.at(2)));

	m_links.try_emplace(idlink, idlink, idConcept, idWord);

}

void Parser::parseDirect(const std::string& line) {

	auto parts = Utils::split(line, ':');

	if (parts.size() != 2)
		throw std::runtime_error("Given line incorrect: " + line);

	uint idSet = static_cast<uint>(std::stoul(parts[0])) - 1;
	uint idWord = static_cast<uint>(std::stoul(parts[1]));

	m_direct[idSet].push_back(idWord);

}

void Parser::parseConceptLabel(const std::string& line)
{
	auto parts = Utils::split(line, ':');

	if (parts.size() != 3)
		throw std::runtime_error("Given line incorrect: " + line);

	uint idConceptLabel = static_cast<uint>(std::stoul(parts[0]));
	uint languageIdx = static_cast<uint>(std::stoul(parts[1]));
	std::string label = parts[2];

	m_conceptLabels[idConceptLabel] = label;

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


std::vector<std::string> Parser::getLanguages() const {

	std::vector<std::string> languages;

	for (const auto& [name, idx] : m_languagesToIdx) 
		languages.push_back(name);
	
	return languages;

}

const uint Parser::getLanguageIdx(const std::string& language) const {

	auto it = m_languagesToIdx.find(language);

	if (it != m_languagesToIdx.end()) 
		return it->second;
	
	throw std::runtime_error("Language not found: " + language);
}

const uint Parser::getWordsNumber(const uint languageIdx) const {

	return m_langues[languageIdx].nWords();

}

const Word& Parser::getWordByIdx(const uint wordIdx, const uint languageIdx) const
{

	if ( languageIdx >= m_langues.size()  )
		throw std::runtime_error("language index not found: " + std::to_string(languageIdx));

	if( wordIdx >= m_langues[languageIdx].nWords() )
		throw std::runtime_error("word index not found: " + std::to_string(wordIdx));

	uint idx = m_langues[languageIdx].wordsIdx[wordIdx];
	
	return m_forms.at(idx);
}

template<typename T>
void Parser::commonSetPart(const uint wordId, const uint targetLangueIdx, T&& f) const
{
	const Langue& targetLange = m_langues[targetLangueIdx];

	for (auto it = m_direct.begin(); it != m_direct.end(); ++it)
	{
		const set& wordSet = it->second;

		if (std::find(wordSet.begin(), wordSet.end(), wordId) == wordSet.end())
			continue;

		for (uint directWordId : wordSet)
			if (std::find(targetLange.wordsIdx.begin(), targetLange.wordsIdx.end(), directWordId) != targetLange.wordsIdx.end())
				if (f(directWordId))
					return;


	}


}

bool Parser::isWordInDirectWithLang(const uint wordId, const uint targetLangueIdx) const
{

	bool found = false;

	commonSetPart(wordId, targetLangueIdx, [&](uint){
		found = true;
		return true; 
		});

	return found;

}

std::vector<std::string> Parser::searchDirect(const Word& word, const uint langueIdx) const
{
	std::vector<std::string> correspondingWords;

	commonSetPart(word.id, langueIdx, [&](uint directWordId) {
		correspondingWords.push_back(m_forms.at(directWordId).word);
		return false;
		});

	return correspondingWords;

}


const std::vector<std::string> Parser::getCorrespondingWords(const Word& word, const uint fromLanguageIdx, const uint toLanguageIdx) const
{
		return searchDirect(word, toLanguageIdx);
}

bool Parser::isWordInConcepts(const uint wordId) const
{

		for(Link link : m_links | std::views::values )
		{
			if( link.idWord == wordId )
				return true;
		}

		return false;

}

std::vector<std::string> Parser::searchConcepts(const Word& word, const uint langueIdx) const
{

	std::vector<std::string> correspondingWords;
	std::vector<uint> conceptIds;

	for (Link link : m_links | std::views::values)
	{
		if (link.idWord == word.id)
			conceptIds.push_back(link.idConcept);
	}

	for (uint conceptId : conceptIds)
	{
		for (Link link : m_links | std::views::values)
		{
			if (link.idConcept == conceptId)
			{
				const Word& linkedWord = m_forms.at(link.idWord);
				
				if (linkedWord.langueIdx == langueIdx && linkedWord.id != word.id)
					correspondingWords.push_back(linkedWord.word);
			}
		}
	}
	return correspondingWords;

}



