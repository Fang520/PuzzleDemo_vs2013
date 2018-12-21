#pragma once

#include <list>
#include <vector>

using namespace std;

class OpenNode
{
public:
	static int level;
	OpenNode(char* layout, OpenNode* parent, int len);
	~OpenNode();
	void CalcWeight();
	list<vector<char>> GetPath();
	char* data;
	int weight;
	int len;
	OpenNode* parent;
};

class CloseNode
{
public:
	static int data_len;
	CloseNode(const char* data);
	bool operator==(const CloseNode& other) const;
	size_t hash() const;
private:
	const char* data;
};

class CAutoLayoutAStar
{
public:
	list<std::vector<char>> LayoutBFS();
	CAutoLayoutAStar(const char* layout, int len);
	virtual ~CAutoLayoutAStar();
private:
	int GetNextLayouts(const char* data);
	bool CanSolved();
	char* m_OriginalLayout;
	char* m_TargetLayout;
	int m_LayoutLen;
	int m_Level;
	char m_Probe[4][25];
};

