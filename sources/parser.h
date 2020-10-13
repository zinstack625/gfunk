#include <string>
#include <vector>

class Expression {
	public:
		std::string function;
		double calculate(int x);
		Expression(std::string input);
		std::vector<Expression> expressions;
		std::vector<char> operations;
	private:
		std::vector<int> order;
		void linearize();
		void parse();
};
