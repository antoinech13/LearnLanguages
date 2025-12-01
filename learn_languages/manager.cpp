#include "manager.h"

Manager::Manager(const std::string& path) : m_parser(path) {}

Manager::Manager(Parser& parser) : m_parser(parser){}

Manager::Manager(Parser&& parser) : m_parser(std::move(parser)) {}

void Manager::start()
{
	std::srand(std::time(nullptr));

	std::vector<std::string> languages = m_parser.getLanguages();

	int selfLanguage = -1, choiceLanguage = -1, exerciceChoice = -1;

	std::string line;

	if (languages.empty()) {
		std::cout << "No languages available to learn." << std::endl;
		return;
	}

	if (languages.size() == 1) {
		std::cout << "Only one language available: " << languages[0] << std::endl;
		return;
	}


	std::cout << "Your learning start now !" << std::endl;
	std::cout << "Select your language ?" << std::endl;

	displayLanguages(languages);
	
	selfLanguage = Utils::inputToIntInRange(1, languages.size() );

	m_selfLanguage = languages[selfLanguage - 1];
	
	languages.erase(languages.begin() + selfLanguage - 1);
	
	std::cout << "Select the language you want to learn ?" << std::endl;
	
	displayLanguages(languages);
	
	choiceLanguage = Utils::inputToIntInRange(1, languages.size());
	m_choosedLanguage = languages[choiceLanguage - 1];

	while (true) {

		std::cout << "What exercice do you want to practise?" << std::endl;
		std::cout << "1. Translate words" << std::endl;
		std::cout << "2. What words mean" << std::endl;
		std::cout << "3. Verbs" << std::endl;

		exerciceChoice = Utils::inputToIntInRange(1, 3);

		if (exerciceChoice == 1)
			translateWords();
		else if (exerciceChoice == 2)
			continue;
			//	meaningWords();
		else if (exerciceChoice == 3)
			continue;
		//	learnVerbs();
		else
			std::cout << "Invalid choice, please try again." << std::endl;

	}

	

}

void Manager::displayLanguages(const std::vector<std::string>& languages) const
{

	for( std::size_t i = 0; i < languages.size(); i++ )
		std::cout << i + 1 << ". " << languages[i] << std::endl;

}

void Manager::translateWords() const
{

	int wordStart, wordEnd, wordCount = 0;
	int idxOfKnowLanguage, idxOfLearningLanguage;
	std::string answer;

	idxOfKnowLanguage = m_parser.getLanguageIdx(m_selfLanguage) - 1;
	idxOfLearningLanguage = m_parser.getLanguageIdx(m_choosedLanguage) - 1;

	std::cout << "you selected " << std::to_string(idxOfKnowLanguage) << " for known language and " << std::to_string(idxOfLearningLanguage) << " for learning language" << std::endl;

	std::cout << "start ?" << std::endl;
	wordStart = Utils::inputToIntInRange(1, m_parser.getWordsNumber(idxOfLearningLanguage));
	std::cout << "end ? " << std::endl;
	wordEnd = Utils::inputToIntInRange(wordStart, m_parser.getWordsNumber(idxOfLearningLanguage));

	wordCount = wordEnd - wordStart;


	std::cout << "Write /stop when you want to stop ! " << std::endl;

	while(answer != "/stop" && wordCount > 0)
	{
		std::cout << "Translate this word: ";
		
		int randomIndex = std::rand() % wordCount + wordStart;
		std::vector<std::string> solutions;
		Word toTranslate;


		do {
			toTranslate = m_parser.getWordByIdx(randomIndex, idxOfKnowLanguage);
			solutions = m_parser.getCorrespondingWords(toTranslate, idxOfKnowLanguage, idxOfLearningLanguage);
		} while (!solutions.size());

		std::cout << toTranslate.word << std::endl;

		if( !std::getline(std::cin, answer))
			throw std::runtime_error("Input error");

		if(answer == "/stop")
			break;

		auto it = std::find(solutions.begin(), solutions.end(), answer);

		if(it != solutions.end())
			std::cout << "Correct!" << std::endl;
		else {
			std::cout << "Wrong! The correct answer(s) was (were): " << std::endl;
			for (const auto& solution : solutions)
				std::cout << solution << std::endl;
		}
			

		
	}


}
/*
void Manager::meaningWords() const
{
	int wordCount = 0;
	int idxOfKnowLanguage, idxOfLearningLanguage;
	std::string toTranslate, solution, answer;
	std::cout << "How many words do you want to practise ?" << std::endl;
	
	wordCount = Utils::inputToIntInRange(1, m_parser.getWordsNumber());
	idxOfKnowLanguage = m_parser.getLanguageIdx(m_selfLanguage) - 1;
	idxOfLearningLanguage = m_parser.getLanguageIdx(m_choosedLanguage) - 1;

	std::cout << "Write /stop when you want to stop ! " << std::endl;

	while(answer != "/stop" && wordCount > 0)
	{
		std::cout << "What does this word mean: ";
		
		int randomIndex = std::rand() % wordCount;
		
		toTranslate = m_parser.getWord(randomIndex, idxOfLearningLanguage);
		solution = m_parser.getWord(randomIndex, idxOfKnowLanguage);
		std::cout << toTranslate << std::endl;
		
		if( !std::getline(std::cin, answer))
			throw std::runtime_error("Input error");
		
		if(answer == "/stop")
			break;
		
		if(solution == answer)
			std::cout << "Correct!" << std::endl;
		else
			std::cout << "Wrong! The correct answer was: " << solution << std::endl;
		
	}

}

void Manager::learnVerbs() const
{
}*/