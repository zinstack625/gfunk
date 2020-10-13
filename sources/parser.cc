#include "parser.h"
#include <string>

Expression::Expression(std::string input) {
  function = input;
  parse();
}

void Expression::parse() {
  /* remove spaces */
  while (function.find(' ', 0) != std::string::npos) {
    function.erase(function.find(' ', 0), 1);
  }

  /* braces */
  bool has_braces = 0;
  int brace_pos = 0;
  int brace_insertion = 0;
  std::string func_name;
  for (;;) {
    brace_pos = function.find('(', brace_pos) == std::string::npos ?
	        -1 : function.find('(', brace_pos);
    if (brace_pos == -1) {
	    break;
    }
    if (brace_pos > 3) {
      func_name = function.substr(brace_pos - 4, 4);
      /* check for function having braces not related to functions */
      if (func_name.substr(1, 3) != "sin" && func_name.substr(1, 3) != "cos" &&
          func_name.substr(1, 3) != "tan" && func_name.substr(1, 3) != "ctg" &&
          func_name.substr(0, 3) != "log" && func_name.substr(2, 2) != "ln") 
              has_braces = 1;
    } else if (brace_pos == 3) {
      func_name = function.substr(0, brace_pos - 1);
      if (func_name.substr(0, 3) != "sin" && func_name.substr(0, 3) != "cos" &&
          func_name.substr(0, 3) != "tan" && func_name.substr(0, 3) != "ctg" &&
          func_name.substr(1, 2) != "ln") {
              has_braces = 1;
      }
    } else if (brace_pos == 2) {
      func_name = function.substr(0, brace_pos - 1);
      if (func_name != "ln") {
	      has_braces = 1;
      }
    } else {
      has_braces = 1;
    }
    if (has_braces) {
      Expression substring (function.substr(brace_pos + 1,
                            function.find(')', brace_pos + 1) - brace_pos - 1));
      function.erase(brace_pos, function.find(')', brace_pos) - brace_pos + 1);
      function.insert(brace_pos, "$");
      expressions.push_back(substring);
    }
    brace_pos++;
    has_braces = 0;
  }
  /* braces end */
  
  /* functions */
  static const std::string functions[] =
    { "sin", "cos", "tan", "ctg", "log", "ln" };
  while (function.find("(", 0) != std::string::npos) {
    int func_begin = function.find('(', 0) + 1;
    int func_end = function.find(')', func_begin) - 1;
    Expression substring (function.substr(func_begin, func_end - func_begin));
    expressions.push_back(substring);
    int i = 0;
    for ( ; i < 5; i++) {
      if (function.substr(func_begin - 4, func_begin - 1) == functions[i]) {
        function.erase(func_begin - 4, func_end + 1);
        break;
      }
    }
    if (function.substr(func_begin - 3, func_begin - 1) == functions[5]) {
      function.erase(func_begin - 3, func_end + 1);
      i = 5;
    }

    function.insert(func_begin - 4, "&" + std::to_string(i));
    
  }
  /* operations */
/*  static const char keys[] = { '^', '*', '/', '+', '-' };
  int string_position = 0;
  /*enum operations {
	  OP_POWER,
	  OP_MULT,
	  OP_DIV,
	  OP_PLUS,
	  OP_MINUS
  };
  for (int i = 0; i < 5; i++) {
    if (function.find(keys[i], string_position) != std::string::npos) {
      int cur_oper = function.find(keys[i], string_position);
      int prev_oper = 0;
      for (int j = cur_oper; j > 0; j--) {
        if (function[j] == '^' || function[j] == '*' ||
            function[j] == '/' || function[j] == '+' ||
	    function[j] == '-') {
          prev_oper = j;
	  break;
	}
      }
      int next_oper = 0;
      for (int j = cur_oper; j < function.size(); j++) {
        if (function[j] == '^' || function[j] == '*' ||
            function[j] == '/' || function[j] == '+' ||
	    function[j] == '-') {
          next_oper = j;
	  break;
	}
      }
      Expression substring (function.substr(prev_oper + 1,
                            next_oper - prev_oper - 1));
      expressions.push_back(substring);
      function.erase(prev_oper, next_oper - prev_oper - 1);
      function.insert(prev_oper, "#" + std::to_string(i));
    } else {
      continue;
    }
  } */
}
double Expression::calculate(int x) {
  std::vector<double> subexp_values;
  std::string function_push = function;
  int subop_cnt = 0;
  while (function_push.find("$", 0))
	  ;
  return 0;
}
	  
