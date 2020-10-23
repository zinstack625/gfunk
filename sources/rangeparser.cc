#include "rangeparser.h"

std::pair<double, double> rangeparser(std::string range) {
	// Delete unneded spaces
	while (range.find(' ') != std::string::npos) {
		range.erase(range.find(' '), 1);
	}
	std::pair<double, double> result = { 0, 0 };
	size_t divider = 0;
	// Implement checks for various user errors
	if (isdigit(range[0]) &&
	    range.find(';') < range.size() - 1 &&
	    isdigit(range[range.find(';') + 1])) {
		try {
			result.first = std::stod(range, &divider);
			result.second = std::stod(range.substr(divider + 1));
		}
		catch (const std::invalid_argument &ia) {
			return {0, 0};
		}
	}
	return result;
}
