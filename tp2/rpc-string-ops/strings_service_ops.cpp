#include "./strings_service.h"

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int* num_char_s_1_svc(char **str, struct svc_req* req) {
	string s = string(*str);
	static int length;
	length = s.length();

	return &length;
}

int* num_char_ws_1_svc(char **str, struct svc_req* req) {
	static int count = 0;

	for(char c : string(*str)) {
		if(!isspace(c))
			count++;
	}

	return &count;
}

int* num_words_1_svc(char **str, struct svc_req* req) {
	static int numWords;
	numWords = 0;
	
    string word;
	string s = string(*str);
    stringstream words(s);
 
    while(words >> word) {
		numWords++;
	}
	words.str(""); // limpiando el stream

	return &numWords;
}