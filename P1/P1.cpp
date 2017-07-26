#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include "fileRead.h"
#include "VectorHash.h"
#include "utilsToStudents.h"

using namespace std;

typedef string T;   // Currently tests string language model.

void test(string filename1, string filename2, unsigned int n, unsigned int printing) {

	cout << "\n\nTesting language model with n = " << n << endl;

	int maxCount = 0; // will store the count of most frequent nGram
	int N1 = 0;        // will store the number of tokens read from file 
	int N2 = 0;

	try
	{
		// First read all tokens from file into a vector 		
		vector<T> tokens1;     // stores tokens read from file
		vector<T> tokens2;

		read_tokens(filename1, tokens1, false); // reads tokens from file without EOS 
		read_tokens(filename2, tokens2, false);

		// Initialize hash table storing n-grams. Each nGram is a vector<T>, used as hashtable key.
		// Associated with the key is the count of that n-gram
		unordered_map<vector<T>, int> database1;
		unordered_map<vector<T>, int> commonDb;

		cout << "\nNow processing " + filename1 << endl;
		// Now create all n-grams from vector of tokens and insert them into the database 
		if (tokens1.size() < n)
			cout << "\nInput file is too small to create any nGrams of size " << n;
		else
		{
			for (unsigned int i = 0; i <= tokens1.size() - n; i++)
			{
				vector<T> nGram(n);   // for temporarily storing tokens to go into next n-gram

									  // Take next n tokens read from the input file 
				for (unsigned int j = 0; j < n; j++) // put next n tokens into vector temp
					nGram[j] = tokens1[i + j];

				if (database1.count(nGram) == 0) // nGram is not in the database yet, insert it with count 1
					database1[nGram] = 1;
				else // nGram is already in the database, increase its count by 1
					database1[nGram] = database1[nGram] + 1;
			}
		}

		double count = 0;
		double totalNGrams2 = 0;
		cout << "\nNow processing " + filename2 << endl;
		// Now create all n-grams from vector of tokens and insert them into the database 
		if (tokens2.size() < n)
			cout << "\nInput file is too small to create any nGrams of size " << n;
		else
		{
			for (unsigned int i = 0; i <= tokens2.size() - n; i++)
			{
				vector<T> nGram(n);   // for temporarily storing tokens to go into next n-gram

				// Take next n tokens read from the input file 
				for (unsigned int j = 0; j < n; j++) // put next n tokens into vector temp
					nGram[j] = tokens2[i + j];

				if (database1.count(nGram) == 0) // nGram is not in the database yet, insert it with count 1
					count++;
				else // nGram is already in the database, increase its count by 1
					commonDb[nGram] = 1;
				totalNGrams2++;
			}
		}
		
		double avg = (count / totalNGrams2) * 100;
		cout << "\n%%%%%%\n " << avg << endl;
		if (printing == 1) {
			cout << "\nCommon nGrams";
			for (auto i = commonDb.begin(); i != commonDb.end(); ++i)
			{
				vector<T> nGram = i->first;
				cout << "\n";

				for (unsigned int j = 0; j < nGram.size(); j++)
					cout << nGram[j] << " ";  // prints out next string in the current nGram
			}
			cout << endl;
		}
		else {
			cout << "Done" << endl;
		}
		
	}
	catch (FileReadException e)
	{
		e.Report();
	}
}

int main(int argc, char **argv) {
	// Takes as command line arguments the names of two text files, the size n for the ngram,
	// and indicating whether to print out common nGrams or not

	string filename1 = argv[2];
	string filename2 = argv[3];
	unsigned int n = *argv[4] - '0';
	unsigned int printing = *argv[5] - '0';

	test(filename1, filename2, n, printing);

	/*for (int i = 1; i < 21; i++) {
		test("MarxEngelsManifest.txt", "SmithWealthNations.txt", i, 0);
	}*/
	
	return 0;
}