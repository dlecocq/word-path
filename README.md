Wort Path
=========
A coworker of mine likes to ask this as an interview question. Imagine a game
where you have to transform one word into another, changing only a single
letter at a time, but each intermediary letter must also be a valid word. So,
for example, one such puzzle might look like this:

	H E A D
	H E A L
	T E A L
	T E L L
	T A L L
	T A I L

The challenege is then to design a program designed to find such a path between
two words if they exist.

Design
======
If this sounds like a graph travseral problem to you, you would do well in this
interview. Consider each word as a node in a graph, where edges join words that
differ by a single letter. Then, if a path exists between two query words, then
each word on the path must therefore be a valid word, and at each step 
transformed by only a single letter at a time.

In order to guarantee that the shortest path is found, then a breadth-first 
search would be appropriate.

Augmentation
============
I wanted to not only solve this puzzle, but also wanted to support additional 
types of transformations. Off hand, I've considered switching any two letters,
and inserting additional letters. An example of the first would be:

	T A R
	R A T
	A R T

An example of the second would be:

	D A D
	B A D
	B A H
	B A T H
	B A T H E
	B A T H E R
	F A T H E R

Data
====
The necessary input for this is a dictionary file, with one word per line. For
example, in the past I've selected from
[this list](http://wordlist.sourceforge.net/).

Usage
=====
This comes with a driver program that loads a dictionary from a path provided
on the command line, and then prompts the user for a starting word and an end
word, and then provides a path between the two if one exists. Alternatively,

	#include "dictionary.hpp"
	#include "rules/standard.hpp"

	// Create a dictionary object
	wordpath::dictionary d("some/path.txt");
	// Apply the standard one-character-change rule
	d.apply(wordpath::standard());

	// Print the path from "head" to "tail"
	d.print_path("head", "tail");

	// Get the path from "head" to "tail" in a std::vector<std::string>
	wordpath::dictionary::path_type results = d.bfs("head", "tail");
	