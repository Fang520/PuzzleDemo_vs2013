#pragma once

#include <list>
#include <vector>

class CAutoLayoutAStar
{
public:
	std::list<std::vector<char>> LayoutBFS();
	CAutoLayoutAStar(const char* layout, int len);
	virtual ~CAutoLayoutAStar();
private:
	int GetNextLayouts(const char* data, char* new_data_list[4]);
	void PrintLayout(const char* data);
	char* m_OriginalLayout;
	char* m_TargetLayout;
	int m_LayoutLen;
	int m_Level;
};

