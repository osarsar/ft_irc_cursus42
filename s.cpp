#include <iostream>
#include <unistd.h>
#include <stdlib.h>

const int MAX = 27;

char printRandomString() 
{ 
    char alphabet[MAX] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',  
                          'o', 'p', 'q', 'r', 's', 't', 'u', 
                          'v', 'w', 'x', 'y', 'z' , ' ' }; 
    return alphabet[rand() % MAX];
} 

int main() {
	char c;
	std::string fill;
	srand(time(0));
	std::string var = "oil up lil nigga im coming for you";
	for (int i = 0; i < var.length() && fill != var;) {
		while (true) {
				c = printRandomString();
			std::cout << fill << c << std::endl;
			if (c == var[i])
				break;
			usleep(20000);
		}
		fill += var[i++];
	}
}