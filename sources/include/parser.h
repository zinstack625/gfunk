#include <string>
#include <vector>

class Expression {
	public:
		double calculate(double x, double y);
		void set_function(const std::string& input);
		Expression(const std::string& input);
		Expression();
	private:
		void parse();
		bool is_calculated();
		std::string function;
		std::vector<Expression> expressions;
		double get_value();
};
