#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

/* This is a dictionary, designed to be able to read in a text file, and 
 * have rules applied to it to generate a graph. With the graph in hand, it can
 * then be used to generate winning sequences subject to those rules. */

#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

namespace wordpath {
	class dictionary {
	public:
		/* This represents a node in our graph */
		typedef struct node_ {
			std::string         word;
			std::vector<node_*> edges;
			node_(const std::string& w): word(w) {};
		} node;

		/* Various typedefs. Edges are a vector of pointers to other nodes,
		 * and a path is a sequence of strings. The map we use internally is
		 * also defined here, and the iterator we use as well */
		typedef node                                            node_type;
		typedef std::vector<node_type*>                         edge_type;
		typedef std::vector<std::string>                        path_type;
		typedef std::tr1::unordered_map<std::string,node_type*> map_type;
		typedef map_type::iterator                              iterator;

		/* Constructor
		 * 
		 * Accepts a path, and expects that path to contain a dictionary with
		 * one word per line */
		dictionary(const std::string& path): nodes() {
			std::fstream ifs(path.c_str(), std::ifstream::in);
			std::string word;
			while (ifs.good()) {
				std::getline(ifs, word);
				/* Strip off the trailing newline */
				word = word.substr(0, word.length() - 1);
				nodes[word] = new node_type(word);
			}
			std::cout << "Read " << nodes.size() << " words" << std::endl;
		}

		/* Destructor
		 *
		 * Since we're keeping node_type*'s, we need to destroy them when we're
		 * done */
		~dictionary() {
			iterator it(nodes.begin());
			for(; it != nodes.end(); ++it) {
				delete it->second;
			}
		};

		/* Node access */
		
		/* Operator[] -- O(1)
		 *
		 * If the provided word does not exist, then it is created and returned
		 * to the user. */
		node_type* operator[](const std::string& word) {
			node_type* result = nodes.operator[](word);
			if (result == NULL) {
				nodes.operator[](word) = result = new node_type(word);
			}
			return result;
		}

		/* existence -- O(1)
		 *
		 * Return true if it's in the dictionary, else false */
		bool exists(const std::string& word) {
			return nodes.find(word) != nodes.end();
		}

		/* iteration
		 *
		 * For iteration across the entiry body of words. These are really 
		 * just acess to the underlying map type */
		iterator begin() { return nodes.begin(); }
		iterator end()   { return nodes.end();   }

		/* connection
		 *
		 * Add a connection from a to b (one way), and optionally create the 
		 * nodes if they don't exist */
		void connect(const std::string a, const std::string b,
			bool create=false) {
			if (create) {
				operator[](a)->edges.push_back(operator[](b));
			} else {
				/* Try to find the two words */
				iterator a_it(nodes.find(a));
				iterator b_it(nodes.find(b));
				/* If they exist, connect them */
				if (a_it != nodes.end() && b_it != nodes.end()) {
					a_it->second->edges.push_back(b_it->second);
				}
			}
		}

		/* apply
		 *
		 * A rule is a functor that accepts a single paramter, this dictionary.
		 * It then may iterate through all the words in the dictionary, and 
		 * adds connections between two nodes as appropriate */
		template <class Rule>
		void apply(const Rule& rule) { rule.apply(*this); }

		void print_path(const std::string& start, const std::string& end) {
			std::vector<std::string> result = bfs(start, end);
			if (result.size() == 0) {
				std::cout << "No path " << start << " -> " << end << std::endl;
				return;
			}

			std::vector<std::string>::iterator it(result.begin());
			for (; it != result.end();) {
				std::cout << *it;
				if (++it != result.end()) {
					std::cout << " -> ";
				}
			}
			std::cout << std::endl;
		}
		
		/* This is an implementation of BFS
		 *
		 * Returns an ordered vector of the path taken to get from A to B. If 
		 * no path can be found, then the returned vector will be empty. */
		std::vector<std::string> bfs(const std::string& start, 
			const std::string& end) {
			/* First, find the nodes that correspond to start, end */
			iterator start_it(nodes.find(start));
			iterator end_it(  nodes.find(end  ));
			/* If we don't actually have both words, then there is no path */
			if (start_it == nodes.end() || end_it == nodes.end()) {
				return std::vector<std::string>();
			}

			/* Otherwise, declare the pointers */
			node_type* start_node = start_it->second;
			node_type* end_node   = end_it->second;

			/* This is the queue of items we need to examine */
			std::queue<node_type*> q;
			/* Set of nodes that we /seen/, so we shouldn't add them again */
			std::tr1::unordered_set<node_type*> seen;
			/* Map of the node to the node from which we discovered it */
			std::tr1::unordered_map<node_type*,node_type*> discovered;

			node_type* current(NULL);
			/* We didn't discover start from anywhere, and we certainly have 
			 * now seen it */
			discovered[start_node] = NULL;
			seen.insert(start_node);
			/* Push the start node onto the queue */
			q.push(start_node);
			while (!q.empty()) {
				current = q.front(); q.pop();
				if (current == end_node) { break; }

				/* Otherwise, add all the edges that haven't been seen should 
				 * be added */
				edge_type::iterator it(current->edges.begin());
				for (; it != current->edges.end(); ++it) {
					/* insert returns std::pair<iterator,bool>, and the second 
					 * argument is true if the insertion took place, and false 
					 * if not */
					if (seen.insert(*it).second) {
						q.push(*it);
						discovered[*it] = current;
					}
				}
			}

			std::vector<std::string> results;
			if (current == end_node) {
				/* Start from current, and find out how we discovered this 
				 * node */
				while (current != NULL) {
					results.push_back(current->word);
					current = discovered[current];
				}
			}
			std::reverse(results.begin(), results.end());

			return results;
		}
	private:
		/* Private, unimplemented to prevent use */
		dictionary();
		dictionary(const dictionary& other);
		const dictionary& operator=(const dictionary& other);

		/* All the nodes we know about */
		map_type nodes;
	};
}

#endif
