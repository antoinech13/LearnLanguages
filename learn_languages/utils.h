#pragma once

#include <string>
#include <vector>
#include <iostream>


class Utils {

public:
	static std::vector<std::string> split(const std::string& str, char delimiter);
	static int inputToInt();
	static int inputToIntInRange(int min, int max);
};