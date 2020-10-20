#include <string>
#include <vector>

class Expression {
	public:
		double calculate(int x);
		void set_function(const std::string& input);
		Expression(const std::string& input);
	private:
		void parse();
		bool is_calculated();
		std::string function;
		std::vector<Expression> expressions;
		double get_value();
};
