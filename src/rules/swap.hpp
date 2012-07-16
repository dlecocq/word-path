#ifndef GAMES__SWAP_HPP
#define GAMES__SWAP_HPP

#include "dictionary.hpp"

namespace wordpath {
	/* This rule allows you to swap any two characters in the word */
	struct swap {
		void apply(dictionary& d) const {
			dictionary::iterator it(d.begin());
			for (; it != d.end(); ++it) {
				std::string word(it->first);
				/* Go through each position, and try every subsequent */
				for (size_t i = 0; i < word.length(); ++i) {
					std::string copy(word);
					for (size_t j = i+1; j < word.length(); ++j) {
						/* Swap forwards */
						copy[i] = word[j];
						copy[j] = word[i];
						d.connect(word, copy);
						/* Swap back */
						copy[i] = word[i];
						copy[j] = word[j];
					}
				}
			}
		}
	};
}

#endif
