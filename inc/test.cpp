#include <iostream>
#include <sstream>
#include <vector>
#include "privmsg.hpp"
#include <algorithm>



#include <iostream>
#include <cstdlib>
#include <cctype>

#include <iostream>


int main() {
    std::string str = "Hello";

    str.erase(str.size() - 1);

    
    std::cout << "Chaîne résultante : " << str << std::endl;
    
    return 0;
}

// int main() {
//     std::string str = "012d456d8l";

//     // Trouver la première occurrence des caractères 'o', 'l' ou 'd'
//     size_t first_occurrence = str.find_first_of("old");
//     if (first_occurrence != std::string::npos) {
//         std::cout << "Première occurrence à la position : " << first_occurrence << std::endl;
//     } else {
//         std::cout << "Aucune occurrence trouvée." << std::endl;
//     }

//     // Trouver la dernière occurrence des caractères 'o', 'l' ou 'd'
//     size_t last_occurrence = str.find_last_of("old");
//     if (last_occurrence != std::string::npos) {
//         std::cout << "Dernière occurrence à la position : " << last_occurrence << std::endl;
//     } else {
//         std::cout << "Aucune occurrence trouvée." << std::endl;
//     }

//     return 0;
// }

// bool isNumeric(const std::string& str) {
//     for (size_t i = 0; i < str.length(); ++i) {
//         if (!std::isdigit(str[i])) {
//             return false;
//         }
//     }
//     return true;
// }

// int main() {
//     std::string input;
//     std::cout << "Entrez une \rchaîne : .";
//     std::cin >> input;

//     if (isNumeric(input)) {
//         std::cout << "La chaîne est un nombre." << std::endl;
//     } else {
//         std::cout << "La chaîne n'est pas un nombre." << std::endl;
//     }

//     return 0;
// }

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