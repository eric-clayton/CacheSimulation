#pragma once
#include <string>
#include <unordered_map>
#include <math.h>
using namespace std;
 class Cache
{
	 struct node
	 {
		 node* prev = nullptr;
		 node* next = nullptr;
		 string tag;
	 }; 
	bool LRU;
	int sets;
	int blocks;
	int bytes;	
	double offsetBits;
	double setBits;
	double hitRatio;
	double hit;
	double total;

	unordered_map<string, unordered_map<string, node*>> cache;
	unordered_map<string, node*> heads;
	unordered_map<string, node*> tails;
	unordered_map<string, bool> cacheFull;

public:
	Cache(int sets, int blocks, int bytes, bool LRU);
	void CheckFull(string setNum);
	void LookUpFA(string line);
	void LookUpDM(string line);
	void LookupSA(string line);
	void cacheRemove(string tag, string setNum);
	void cacheAdd(string tag, string setNum);
	double GetHitRatio();
};




