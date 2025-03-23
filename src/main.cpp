#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>

std::map<std::string, double> variables;

int precedence(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  return 0;
}

double applyOp(double a, double b, char op) {
  switch (op) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
    return a * b;
  case '/':
    if (b == 0)
      throw std::runtime_error("Division by zero");
    return a / b;
  default:
    throw std::runtime_error("Unknown operator");
  }
}

std::string removeSpaces(const std::string &expr) {
  std::string result = expr;
  result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
  return result;
}

double evaluate(std::string expr) {
  expr = removeSpaces(expr);

  std::stack<double> values;
  std::stack<char> ops;
  std::string token;

  for (size_t i = 0; i < expr.size(); ++i) {
    if (std::isdigit(expr[i]) ||
        (expr[i] == '-' && (i == 0 || expr[i - 1] == '('))) {
      size_t j = i;
      while (j < expr.size() && (std::isdigit(expr[j]) || expr[j] == '.'))
        ++j;
      values.push(std::stod(expr.substr(i, j - i)));
      i = j - 1;
    } else if (std::isalpha(expr[i])) {
      size_t j = i;
      while (j < expr.size() && std::isalpha(expr[j]))
        ++j;
      std::string var = expr.substr(i, j - i);
      if (variables.count(var))
        values.push(variables[var]);
      else
        throw std::runtime_error("Unknown variable: " + var);
      i = j - 1;
    } else if (expr[i] == '(') {
      ops.push('(');
    } else if (expr[i] == ')') {
      while (!ops.empty() && ops.top() != '(') {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(a, b, op));
      }
      ops.pop();
    } else if (std::string("+-*/").find(expr[i]) != std::string::npos) {
      while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(a, b, op));
      }
      ops.push(expr[i]);
    }
  }

  while (!ops.empty()) {
    double b = values.top();
    values.pop();
    double a = values.top();
    values.pop();
    char op = ops.top();
    ops.pop();
    values.push(applyOp(a, b, op));
  }

  return values.top();
}

void interactive_mode() {
  std::string input;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, input);
    if (input == "--exit")
      break;
    if (input.find('=') != std::string::npos) {
      size_t eq_pos = input.find('=');
      std::string var = input.substr(0, eq_pos);
      std::string value = input.substr(eq_pos + 1);
      variables[var] = evaluate(value);
    } else {
      try {
        std::cout << evaluate(input) << std::endl;
      } catch (const std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    interactive_mode();
  } else {
    std::string expr;
    for (int i = 1; i < argc; ++i) {
      expr += argv[i];
    }
    try {
      std::cout << evaluate(expr) << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  }
  return 0;
}
