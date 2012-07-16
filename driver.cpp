#include "dictionary.hpp"
#include "rules/standard.hpp"
#include "rules/expand.hpp"
#include "rules/swap.hpp"

#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Provide a path to a dictionary file" << std::endl;
		exit(1);
	}

	/* Let's make a few nodes */
	wordpath::dictionary d(argv[1]);
	std::cout << "Applying standard rule..." << std::endl;
	d.apply(wordpath::standard());
	std::cout << "Applying expand rule..." << std::endl;
	d.apply(wordpath::expand());
	std::cout << "Applying swap rule..." << std::endl;
	d.apply(wordpath::swap());

	std::string start, end;
	while (std::cin.good()) {
		std::cout << "Start: ";
		std::getline(std::cin, start);
		std::cout << "End: ";
		std::getline(std::cin, end);

		d.print_path(start, end);
	}

	return 0;
}