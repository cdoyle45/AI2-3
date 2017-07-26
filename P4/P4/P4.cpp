#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "fileRead.h"
#include "VectorHash.h"
#include "utilsToStudents.h"
#include <math.h>

using namespace std;


typedef string T;

unordered_map<vector<T>, int> process(vector<T> tokens, unsigned int n) {

	unordered_map<vector<T>, int> database;

	if (tokens.size() < n)
		cout << "\nInput file is too small to create any nGrams of size " << n;
	else
	{
		for (unsigned int i = 0; i <= tokens.size() - n; i++)
		{
			vector<T> nGram(n);
			for (unsigned int j = 0; j < n; j++)
				nGram[j] = tokens[i + j];

			if (database.count(nGram) == 0)
				database[nGram] = 1;
			else
				database[nGram] = database[nGram] + 1;

		}
	}
	return database;
}

vector<vector<T>> sentenceGenerator(string sentences)
{
	vector<vector<T>> sentence_vector;
	vector<T> sentence_tokens;
	read_tokens(sentences, sentence_tokens, true);

	vector<T> tmp;
	for (std::vector<T>::iterator i = sentence_tokens.begin(); i != sentence_tokens.end(); ++i)
	{
		if (*i != "<END>")
		{
			tmp.push_back(*i);
		}
		else {
			sentence_vector.push_back(tmp);
			tmp.clear();
		}
	}
	return sentence_vector;
}
void addDelta(string textModel, string sentences, unsigned int n, unsigned int delta) {
	double vocabSize = 0;
	double B = 0;
	int N = 0;
	try
	{
		// Vocab size creating
		vector<T> tokens;
		read_tokens(textModel, tokens, false);
		N = tokens.size();

		unordered_map<vector<T>, int> vocab = process(tokens, 1);
		unordered_map<vector<T>, int> words = process(tokens, n);
		
		vocabSize = vocab.size() * 2;
		B = pow(vocabSize, n);

		// Sentences
		vector<vector<T>> sentence_vector = sentenceGenerator(sentences);

		// Vector of sentence nGrams per sentence
		vector<unordered_map<vector<T>, int>> sentence_maps;
		for (int i = 0; i < sentence_vector.size(); i++)
		{
			sentence_maps.push_back(process(sentence_vector[i], n));
		}
		
		/*for (int i = 0; i < sentence_maps.size(); i++)
		{
			for (auto j = sentence_maps[i].begin(); j != sentence_maps[i].end(); ++j)
			{
				vector<T> nGram = j->first;
				cout << "\n";

				for (unsigned int k = 0; k < nGram.size(); k++)
					cout << nGram[k] << " ";  // prints out next string in the current nGram

				cout << " " << j->second; // prints out the count of this nGram
			}
		}*/

		vector<unordered_map<vector<T>, int>> probabilities;
		unordered_map<vector<T>, int> empty;
		// Probabilities[1] is unigram probabilities, etc
		probabilities.push_back(empty);

		// Probability = cost(nGram) + delta / (N + delta*B)
	}
	catch (FileReadException e)
	{
		e.Report();
	}
}

void goodTuring(string textModel, string sentences, unsigned int n, unsigned int threshold) {

	try
	{

	}
	catch (FileReadException e)
	{
		e.Report();
	}
}

int main(int argc, char **argv) {

	string textModel = argv[2];
	string sentences = argv[3];
	unsigned int n = *argv[4] - '0';
	unsigned int param = *argv[5] - '0';
	unsigned int langModel = *argv[6] - '0';

	if (langModel == 0) {
		goodTuring(textModel, sentences, n, param);
	}
	else if (langModel == 1) {
		addDelta(textModel, sentences, n, param);
	}


	return 0;
}