
#include <iostream>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// Given an array of characters which form a sentence of words, write an efficient algorithm to
// reverse the order of the words (not characters) in it.
void reverseWords() {
	printf("Start: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	std::cout
			<< "Test: Given an array of characters which form a sentence of words, write an efficient algorithm to"
			<< "reverse the order of the words (not characters) in it."
			<< std::endl;
	const char* phrase = "this is an absolute mess; why are you here today!!";
	printf("initial string=%s\n", phrase);

	int length = strlen(phrase) + 100;
	char c_phrase[length];
	char temp[length];
	memset(c_phrase, 0, length);
	memset(temp, 0, length);
	char result[length];
	memset(result, 0, length);
	strncpy(c_phrase, phrase, length);
	printf("c_phrase = %s\n ", c_phrase);

	char* token = strtok(c_phrase, " ");
	while (token != NULL) {
		printf("token = %s\n", token);
		strncpy(temp, result, length);
		sprintf(result, "%s %s", token, temp);
		printf("%s\n\n", result);
		token = strtok(NULL, " ");
	}
	printf("final result=%s\n", result);
	printf("initial string=%s\n", phrase);

	std::cout
			<< "Done: Test: Given an array of characters which form a sentence of words, write an efficient algorithm to"
			<< "reverse the order of the words (not characters) in it."
			<< std::endl;
	printf("END: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}
