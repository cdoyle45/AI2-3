#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "fileRead.h"
#include "VectorHash.h"
#include "utilsToStudents.h"

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
			vector<T> nGram(n);   // for temporarily storing tokens to go into next n-gram

								  // Take next n tokens read from the input file 
			for (unsigned int j = 0; j < n; j++) // put next n tokens into vector temp
				nGram[j] = tokens[i + j];

			if (nGram[0] == "<END>") {
				continue;
			}
			else if (database.count(nGram) == 0) {
				database[nGram] = 1;
			}
			else {
				database[nGram] = database[nGram] + 1;
			}

		}
	}
	return database;
}

void test(string filename1, string filename2, unsigned int n) {

	double count = 0;
	int N1 = 0;
	int N2 = 0;
	vector<unordered_map<vector<T>, int>> file1;
	vector<unordered_map<vector<T>, int>> file2;
	unordered_map<vector<T>, int> uncommon;

	try
	{ 		
		vector<T> tokens1;
		vector<T> tokens2;

		N1 = tokens1.size();
		N2 = tokens2.size();


		read_tokens(filename1, tokens1, true);
		read_tokens(filename2, tokens2, true);

		unordered_map<vector<T>, int> empty;
		file1.push_back(empty);
		file2.push_back(empty);


		for (unsigned int i = 1; i <= n; i++) {
			unordered_map<vector<T>, int> database1;
			unordered_map<vector<T>, int> database2;

			database1 = process(tokens1, i);
			database2 = process(tokens2, i);

			file1.push_back(database1);
			file2.push_back(database2);
		}


		for (auto i = file2[n].begin(); i != file2[n].end(); ++i)
		{
			vector<T> nGram = i->first;

			if (file1[n].count(nGram) == 0)
			{
				count++;
			}
			else {
				uncommon[nGram] = 1;
			}
		}
		
		double avg = (count / double(file2[n].size())) * 100;
		cout << "\n" << avg << endl;
		
		/*for (auto i = uncommon.begin(); i != uncommon.end(); ++i)
		{
			vector<T> nGram = i->first;
			cout << "\n";

			for (unsigned int j = 0; j < nGram.size(); j++)
				cout << nGram[j] << " ";  // prints out next string in the current nGram

			cout << " " << i->second; // prints out the count of this nGram
		}*/
	}
	catch (FileReadException e)
	{
		e.Report();
	}
}

int main(int argc, char **argv) {

	string filename1 = argv[2];
	string filename2 = argv[3];
	unsigned int n = *argv[4] - '0';

	test(filename1, filename2, n);


	return 0;
}