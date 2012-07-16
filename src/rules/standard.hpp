#ifndef GAMES__STANDARD_HPP
#define GAMES__STANDARD_HPP

#include "dictionary.hpp"
#include <iostream>

namespace wordpath {
	/* This rule allows you to change any single character */
	struct standard {
		void apply(dictionary& d) const {
			/* We'll go through every word and find all the possible
			 * destination words */
			const char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
			dictionary::iterator it(d.begin());
			for (; it != d.end(); ++it) {
				/* This is the word we're looking at */
				std::string word(it->first);
				for (size_t i = 0; i < it->second->word.length(); ++i) {
					std::string copy(word);
					for (size_t j = 0; j < 26; ++j) {
						if (alphabet[j] == word[i]) {
							continue;
						}
						copy[i] = alphabet[j];
						d.connect(word, copy);
					}
				}
			}
		}
	};
}

#endif
