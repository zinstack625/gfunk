#include <iostream>
#include "parser.h"

int main() {
	Expression test ("(x^2) + 7 * sin(y)");
	std::cout << test.function << std::endl;
	for (Expression i : test.expressions) {
		std::cout << i.function << ' ';
		for (char j : i.operations) {
			std::cout << j << std::endl;
		}
	}
}

