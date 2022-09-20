#include "Cache.h"
Cache::Cache(int sets, int blocks, int bytes, bool LRU)
{
	this->sets = sets;
	this->blocks = blocks;
	this->bytes = bytes;
	this->LRU = LRU;

	hitRatio = 0;
	hit = 0;
	total = 0;
	offsetBits = log2(bytes);
	setBits = log2(sets);
}
void Cache::CheckFull(string setNum)
{
	if (cacheFull.find(setNum) == cacheFull.end())
	{
		cacheFull[setNum] = false;
	}
	else
	{
		if (cache[setNum].size() == blocks)
		{
			cacheFull[setNum] = true;
		}
	}
}
void Cache::LookUpFA(string line)
{
	string tag = line.substr(0, 32 - offsetBits);
	string setNum = "0";
	int temp = blocks;
	blocks = sets * blocks;
	CheckFull(setNum);
	if(LRU)
	{
		if (cache[setNum].find(tag) == cache[setNum].end())
		{
			if (cacheFull[setNum])
			{
				cacheRemove(tails[setNum]->tag, setNum);
				cacheAdd(tag, setNum);
			}
			else
			{
				cacheAdd(tag, setNum);
			}	
		}
		else
		{
			cacheRemove(tag, setNum);
			cacheAdd(tag, setNum);
			hit++;
		}
	}
	else
	{
		if (cache[setNum].find(tag) == cache[setNum].end())
		{
			if (cacheFull[setNum])
			{
				cacheRemove(tails[setNum]->tag, setNum);
				cacheAdd(tag, setNum);
			}
			else
			{
				cacheAdd(tag, setNum);
			}
		}
		else
		{
			hit++;
		}
	}
	hitRatio = hit / ++total;
	blocks = temp;
}
void Cache::LookUpDM(string line)
{
	int temp2 = setBits;
	setBits += log2(blocks);
	int temp = blocks;
	blocks = 1;

	string tag = line.substr(0, 32 - offsetBits - setBits);
	string setNum = line.substr(32 - offsetBits - setBits, setBits);
	CheckFull(setNum);
	if (cache[setNum].find(tag) == cache[setNum].end())
	{
		if (cacheFull[setNum])
		{
			cacheRemove(tails[setNum]->tag, setNum);
			cacheAdd(tag, setNum);
		}
		else
		{
			cacheAdd(tag, setNum);
		}
	}
	else
	{
		hit++;
	}
	hitRatio = hit / ++total;
	blocks = temp;
	setBits = temp2;
}
void Cache::LookupSA(string line)
{
	string tag = line.substr(0, 32 - offsetBits - setBits);
	string setNum = line.substr(32 - offsetBits - setBits, setBits);;
	CheckFull(setNum);
	if (LRU)
	{
		if (cache[setNum].find(tag) == cache[setNum].end())
		{
			if (cacheFull[setNum])
			{
				cacheRemove(tails[setNum]->tag, setNum);
				cacheAdd(tag, setNum);
			}
			else
			{
				cacheAdd(tag, setNum);
			}
		}
		else
		{
			cacheRemove(tag, setNum);
			cacheAdd(tag, setNum);
			hit++;
		}
	}
	else
	{
		if (cache[setNum].find(tag) == cache[setNum].end())
		{
			if (cacheFull[setNum])
			{
				cacheRemove(tails[setNum]->tag, setNum);
				cacheAdd(tag, setNum);
			}
			else
			{
				cacheAdd(tag, setNum);
			}
		}
		else
		{
			hit++;
		}
	}
	hitRatio = hit / ++total;
}
void Cache::cacheRemove(string tag, string setNum)
{
	node* temp = cache[setNum][tag];
	if (temp == heads[setNum])
	{
		if (temp == tails[setNum])
		{
			tails.erase(setNum);
			heads.erase(setNum);
		}
		else
		{
			temp->next->prev = nullptr;
			heads.erase(setNum);
			heads[setNum] = temp->next;
		}
	}
	else
	{
		if (temp == tails[setNum])
		{
			temp->prev->next = nullptr;
			tails.erase(setNum);
			tails[setNum] = temp->prev;
		}
		else
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
		}
	}
	cache[setNum].erase(temp->tag);
	delete temp;
}
void Cache::cacheAdd(string tag, string setNum)
{
	node* newNode = new node;
	newNode->tag = tag;
	if (heads.find(setNum) == heads.end())
	{
		heads[setNum] = newNode;
		tails[setNum] = newNode;
	}
	else 
	{
		newNode->next = heads[setNum];
		heads[setNum]->prev = newNode;
		heads.erase(setNum);
		heads[setNum] = newNode;
	}
	cache[setNum][tag] = newNode;
}
double Cache::GetHitRatio()
{
	return hitRatio;
}