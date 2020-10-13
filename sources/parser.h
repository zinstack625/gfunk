#include <string>
#include <vector>

class Expression {
	public:
		std::string function;
		double calculate(int x);
		Expression(std::string input);
		std::vector<Expression> expressions;
		std::vector<char> operations;
		bool is_calculated();
		double get_value();
	private:
		void parse();
};
