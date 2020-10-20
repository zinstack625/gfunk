#include "parser.h"
#include <string>
#include <cctype>
#include <cmath>

Expression::Expression(const std::string& input) {
  function = input;
  parse();
}

void Expression::parse() {
  /* remove spaces */
  while (function.find(' ', 0) != std::string::npos) {
    function.erase(function.find(' ', 0), 1);
  }

  /* braces */
  int subexp_cnt = 0;
  while (function.find('(', 0) != std::string::npos) {
    int brace_pos = function.find('(', 0);  
    Expression substring (function.substr(brace_pos + 1,
                          function.find(')', brace_pos + 1) - brace_pos - 1));
    function.replace(brace_pos, function.find(')', brace_pos) - brace_pos + 1,
		    "$" + std::to_string(subexp_cnt));
    expressions.push_back(substring);
    subexp_cnt++;
  }
  
  /* functions */
  const std::string functions[] =
    { "sin", "cos", "tan", "ctg", "log" };
  for (int i = 0; i < 5; i++) {
    while (function.find(functions[i], 0) != std::string::npos) {
      int func_begin = function.find(functions[i], 0);
      function.replace(func_begin, 3, "&" + std::to_string(i) + "|");
    }
  }
}

bool Expression::is_calculated() {
  if (function[0] == '-') {
    for (char i : function.substr(1)) {
      if (!isdigit(i)) {
        return false;
      }
    }
  } else {
    for (char i : function) {
      if (!isdigit(i)) {
        return false;
      }
    }
  }
  return true;
}

double Expression::get_value() {
  return std::stod(function);
}

double Expression::calculate(int x) {
  std::string function_push = function;
  /* insert x */
  while (function_push.find('x', 0) != std::string::npos) {
    function_push.replace(function_push.find('x', 0), 1, std::to_string(x));
  }
  const char opers[] = { '^', '*', '/', '+', '-' };
  for (char i : opers) {
    while (function_push.find(i, 1) != std::string::npos) {
      /* find the edges of subexpression */
      size_t prev_oper = -1;
      size_t cur_oper = function_push.find(i, 1);
      size_t next_oper = function_push.size();
      for (int j = cur_oper - 2; j > 0; j--) {
	  char am = function_push[j - 1];
	  switch (function_push[j]) {
            case '-':
	      if (am == '*' || am == '/' || am == '+' || am == '-') {
                prev_oper = j - 1;
		break;
	      }
            case '^':
	    case '*':
	    case '/':
	    case '+':
              prev_oper = j;
              break;
          }
      }
      for (int j = cur_oper + 2; j < function_push.size(); j++) {
        if (function_push[j] == '^' || function_push[j] == '*' ||
            function_push[j] == '/' || function_push[j] == '+' ||
            function_push[j] == '-') {
          next_oper = j;
          break;
        }
      }
      std::string subexp = function_push.substr(
          prev_oper + 1, next_oper - prev_oper - 1);
      while (subexp.find('$', 0) != std::string::npos) {
        size_t subexp_end = subexp.size() - 1;
        size_t subexp_begin = subexp.find('$', 0);
        int subexp_num = std::stoi(subexp.substr(subexp_begin + 1,
          		      std::string::npos), &subexp_end);
	double subexp_value = 0;
	if (!expressions[subexp_num].is_calculated()) {
          subexp_value = expressions[subexp_num].calculate(x);
	} else {
          subexp_value = expressions[subexp_num].get_value();
	}
        subexp.replace(subexp_begin,
                 subexp_end + 1, std::to_string(subexp_value));
      }
      while (subexp.find('&', 0) != std::string::npos) {
        size_t func_end = 0;
        size_t func_begin = subexp.find('&', 0);
        double func_value = 0;
	size_t arg_end = 0;
        int func_num = std::stoi(subexp.substr(func_begin + 1,
          		      std::string::npos), &func_end);
        double subexp_value = std::stod(subexp.substr(func_begin + func_end + 2,
          		      std::string::npos), &arg_end);
        switch (func_num) {
          case 0:
            func_value = sin(subexp_value);
            break;
          case 1:
            func_value = cos(subexp_value);
            break;
          case 2:
            func_value = tan(subexp_value);
            break;
          case 3:
            func_value = tan(1 / subexp_value);
            break;
          case 4:
            func_value = log(subexp_value);
            break;
        }
        subexp.replace(func_begin, arg_end - func_begin + 1,
			std::to_string(func_value));
      }
      size_t subexp_oper = 0;
      double first = std::stod(subexp, &subexp_oper);
      double second = std::stod(subexp.substr(subexp_oper + 1,
			                      std::string::npos), nullptr);
      double subexp_value = 0;
      switch (i) {
        case '^':
	  subexp_value = pow(first, second);
          break;
	case '*':
	  subexp_value = first * second;
	  break;
	case '/':
	  subexp_value = first / second;
	  break;
	case '+':
          subexp_value = first + second;
	  break;
	case '-':
	  subexp_value = first - second;
	  break;
      }
      subexp.replace(0, std::string::npos, std::to_string(subexp_value));
      function_push.replace(prev_oper + 1, next_oper - 1, subexp);
    }
  }
  return std::stod(function_push);
}

void Expression::set_function(const std::string& input) {
	function = input;
	parse();
}
