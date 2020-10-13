#include <iostream>
#include "parser.h"

int main() {
	Expression test ("(x^2) + 7 * sin(x)");
	std::cout << test.function << std::endl;
	for (int i = -10; i < 10; i++) {
		std::cout << test.calculate(i) << std::endl;
	}
}

