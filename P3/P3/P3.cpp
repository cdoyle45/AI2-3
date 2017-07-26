#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "fileRead.h"
#include "VectorHash.h"
#include "utilsToStudents.h"

using namespace std;


typedef string T;

unordered_map<string, int> process_vocab(vector<T> tokens, unsigned int n)
{
	unordered_map<string, int> database;

	if (tokens.size() < n)
		cout << "\nInput file is too small to create any nGrams of size " << n;
	else
	{
		for (unsigned int i = 0; i <= tokens.size() - n; i++)
		{
			string nGram = tokens[i];

			if (database.count(nGram) == 0)
				database[nGram] = 1;
			else
				database[nGram] = database[nGram] + 1;
		}
	}

	return database;
}

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

			if (database.count(nGram) == 0) {
				database[nGram] = 1;
			}
			else {
				database[nGram] = database[nGram] + 1;
			}

		}
	}
	return database;
}
void test(string filename, unsigned int n) {

	vector<T> sentence;
	vector<unordered_map<vector<T>, int>> file;
	unordered_map<string, int> vocab;

	try
	{		
		vector<T> tokens;
		read_tokens(filename, tokens, true);
		vocab = process_vocab(tokens, 1);
		for (unsigned int i = 1; i <= n; i++) {
			unordered_map<vector<T>, int> database;
			database = process(tokens, i);
			file.push_back(database);
		}
		vector<T> vocab_vector;
		vector<double> probs;

		for (auto i = vocab.begin(); i != vocab.end(); ++i)
		{
			probs.push_back(i->second / double(tokens.size()));
			vocab_vector.push_back(i->first);
		}

		int word1_index = drawIndex(probs);
		sentence.push_back(vocab_vector[word1_index]);
		unsigned int count = 2;
		if (n == 1)
		{
			int word_index = drawIndex(probs);
			while (vocab_vector[word_index] != "<END>")
			{
				sentence.push_back(vocab_vector[word_index]);
				word_index = drawIndex(probs);
			}
		}
		else {
		
			string word = "";
			while (word != "<END>") {
				vector<double> new_probs;

				for (auto i = vocab.begin(); i != vocab.end(); ++i)
				{
					vector<string> last;
					for (unsigned int j = sentence.size() - (count - 1); j < sentence.size(); j++) {
						last.push_back(sentence[j]);
					}
					last.push_back(i->first);

					if (file[last.size() - 1].count(last) != 0)
					{
						vector<string> interim = last;
						interim.pop_back();

						double probability = (file[last.size() - 1].find(last)->second / double(file[interim.size() - 1].find(interim)->second));
						new_probs.push_back(probability);
					}
					else {
						new_probs.push_back(0);
					}
				}
				int word_index = drawIndex(new_probs);
				word = vocab_vector[word_index];
				sentence.push_back(word);

				if (count < n)
				{
					count++;
				}
			}
		}
		for (std::vector<string>::iterator i = sentence.begin(); i != sentence.end(); ++i)
			std::cout << *i << ' ';
	}
	catch (FileReadException e)
	{
		e.Report();
	}
}

int main(int argc, char **argv) {
	// Input: Name of a text file, size n of the nGram model
	// First your program should construct a ML language model from file

	string filename = argv[2];
	unsigned int n = *argv[3] - '0';


	test(filename, n);



	return 0;
}