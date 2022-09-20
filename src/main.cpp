#include "Cache.h"
#include <iostream>
#include <fstream>

string transform(string line)
{
	unordered_map<char, string> HexToBin =
	{
		{'0', "0000"}, {'1', "0001"},
		{'2', "0010"}, {'3', "0011"},
		{'4', "0100"}, {'5', "0101"},
		{'6', "0110"}, {'7', "0111"},
		{'8', "1000"}, {'9', "1001"},
		{'a', "1010"}, {'b', "1011"},
		{'c', "1100"}, {'d', "1101"},
		{'e', "1110"}, {'f', "1111"}
	};

	string hex = line.substr(4, 8);
	string result = "";
	for (char c : hex)
	{
		result += HexToBin[c];
	}
	return result;
}
int main()
{
	ifstream file;
	ofstream output;
	string filename;
	string line;
	string replacement;
	const int MAIN_MEMORY_SIZE = 32;
	int sets;
	int blocks;
	int bytes;
	int cacheSize;
	bool LRU;
	char response;

	double hitRatioFA = 0;
	double hitRatioDM = 0;
	double hitRatioSA = 0;
	while (true)
	{
		do
		{
			cout << "Enter number of sets: ";
			cin >> sets;
			cout << "Enter number of blocks per set: ";
			cin >> blocks;
			cout << "Enter number of bytes per block: ";
			cin >> bytes;
			cacheSize = log2(sets) + log2(blocks) + log2(bytes);
			if (cacheSize > MAIN_MEMORY_SIZE)
			{
				cout << "Cache bigger than main memory!!" << endl;
			}
		} while (cacheSize > MAIN_MEMORY_SIZE);

		cout << "Enter replacement policy FIFO or LRU: ";
		cin >> replacement;
		cout << endl;

		if (replacement == "LRU")
		{
			LRU = true;
		}
		else
		{
			LRU = false;
		}

		Cache FA(sets, blocks, bytes, LRU);
		Cache DM(sets, blocks, bytes, LRU);
		Cache SA(sets, blocks, bytes, LRU);


		file.open("cache.trace");
		if (!file.is_open())
		{
			cout << "Invalid filename" << endl;
		}
		while (getline(file, line))
		{
			line = transform(line);
			FA.LookUpFA(line);
			DM.LookUpDM(line);
			SA.LookupSA(line);
		}
		hitRatioFA = FA.GetHitRatio();
		hitRatioDM = DM.GetHitRatio();
		hitRatioSA = SA.GetHitRatio();
		cout << "Cache Size: " << pow(2, cacheSize) << " bytes." << endl;
		cout << "Fully associative hit ratio: " << hitRatioFA << endl;
		cout << "Direct mapped hit ratio: " << hitRatioDM << endl;
		cout << "Set-associative hit ratio: " << hitRatioSA << endl;
		file.close();
		cout << "Continue? (y/n)" << endl;
		cin >> response;
		if (response == 'n')
		{
			break;
		}
		cout << endl;
	}
	return 0;

}