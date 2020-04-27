#include <iostream>
#include <stdlib.h>
#include <time.h>

int main() {
	int rand_number;
	int guess_number = -1;
	char cont;
	srand(time(NULL));
	while (true) {
		rand_number = rand() % 100;
		while (guess_number != rand_number) {
			std::cout << "Enter your guess: " << std::endl;
			std::cin >> guess_number;
			if (guess_number < rand_number) {
				std::cout << "Your guess is smaller than ours" << std::endl;
			} 
			else if (guess_number > rand_number) {
				std::cout << "Your guess is larger than ours" << std::endl;
			}
		}
		std::cout << "Your guess is correct! Continue? (y/n)" << std::endl;
		std::cin >> cont;
		if (cont != 'y') {
			std::cout << "Bye bye!"	 << std::endl;
			break;
		}
		guess_number = -1;
	}
}