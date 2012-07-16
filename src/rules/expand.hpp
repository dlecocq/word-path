#ifndef GAMES__EXPAND_HPP
#define GAMES__EXPAND_HPP

#include "dictionary.hpp"

namespace wordpath {
	/* This rule allows you to insert any single character into the word */
	struct expand {
		void apply(dictionary& d) const {
			const char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
			dictionary::iterator it(d.begin());
			for (; it != d.end(); ++it) {
				std::string word(it->first);
				for (size_t i = 0; i <= it->second->word.length(); ++i) {
					std::string copy(word.substr(0, i) + " " + word.substr(i));
					for (size_t j = 0; j < 26; ++j) {
						copy[i] = alphabet[j];
						d.connect(word, copy);
					}
				}
			}
		}
	};
}

#endif
