#include <iostream>
#include <sstream>
#include <vector>
#include "privmsg.hpp"
#include <algorithm>



bool isStringInVector(const std::vector<std::string>& vec, const std::string& str) {
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        if (*it == str) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<std::string> operators;
    operators.push_back("add");
    operators.push_back("subtract");
    operators.push_back("multiply");
    operators.push_back("divide");

    std::string input;
    std::cout << "Enter an operator: ";
    std::cin >> input;

    if (isStringInVector(operators, input)) {
        std::cout << "Operator found in the vector." << std::endl;
    } else {
        std::cout << "Operator not found in the vector." << std::endl;
    }

    return 0;
}

// std::vector<std::string> split(const std::string& input, char delimiter) {
//     std::vector<std::string> tokens;
//     std::istringstream stream(input);
//     std::string token;

//     while (std::getline(stream, token, delimiter)) {
//         token = trim(token);
//         if (!token.empty()) {
//             tokens.push_back(token);
//         }
//     }

//     return tokens;
// }

// int main() {
//     std::string input = "Hello      Wo    rld  C++ Split";
//     char delimiter = ' ';

//     std::vector<std::string> result = split(input, delimiter);

//     // Afficher les résultats
//     for (size_t i = 0; i < result.size(); ++i) {
//         std::cout << "[" << result[i] << "]" << std::endl;
//     }

//     return 0;
// }

// #include "error.hpp"
// #include "channel.hpp"

// #define TEST(clt, cmd)      std::string(clt) + " " + std::string(cmd) + " :Not enough parameters"
// #define TEST2(clt, cmd)      std::string(clt) + " " + std::string(cmd) + " :2Not enough parameters"

// int main() {
//     try {
//         if (1) {
//             throw (std::runtime_error(TEST("1", "2")));
//         }
//     } catch (const std::exception &e) 
//     {
//         std::cerr << e.what() << std::endl;
//     }

//     std::cout << TEST("1", "2") << "\n";

//     return 0;
// }