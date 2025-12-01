#include "utils.h"

std::vector<std::string> Utils::split(const std::string& str, char delimiter)
{
	std::vector<std::string> elements;
	std::string::size_type start = 0, end;

	while ( (end = str.find(delimiter, start)) != std::string::npos  )
	{

		elements.emplace_back(str.substr(start, end - start));	
		start = end + 1;

	}

	elements.emplace_back(str.substr(start));

	return elements;

}

int Utils::inputToInt()
{
	std::string line;

	
	while(true)
	{

		if( !std::getline(std::cin, line))
			throw std::runtime_error("Input error");

		try {

			size_t pos;
			int val = std::stoi(line, &pos);
			

			while (pos < line.size() && std::isspace((unsigned char)line[pos]))
				pos++;

			if (pos != line.size()) {
				std::cout << "unexpected argument ! try again" << std::endl;
				continue;
			}

			return val;
		}
		catch (const std::invalid_argument&) {
			std::cout << "invalide argument" << std::endl;
			continue;
		}
		catch (const std::out_of_range&){
			std::cout << "number out of range" << std::endl;
			continue;
		}

	}

}

int Utils::inputToIntInRange(int min, int max)
{
	while (true) {

		int val = inputToInt();
		if (val < min || val > max) {
			std::cout << "number out of range [" << min << "," << max << "] try again" << std::endl;
			continue;
		}
		return val;

	}
}
