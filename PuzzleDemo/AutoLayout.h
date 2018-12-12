// AutoLayout.h: interface for the CAutoLayout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOLAYOUT_H__75033F01_3667_45B3_8FD6_02C6A32932ED__INCLUDED_)
#define AFX_AUTOLAYOUT_H__75033F01_3667_45B3_8FD6_02C6A32932ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <vector>

class CAutoLayout
{
public:
	std::list<std::vector<char>> LayoutBFS();
	std::list<std::vector<char>> LayoutAStar();
	CAutoLayout(const char* layout, int len);
	virtual ~CAutoLayout();
private:
	int GetNextLayouts(const char* data, char* new_data_list[4]);
	void PrintLayout(const char* data);
	char* m_OriginalLayout;
	char* m_TargetLayout;
	int m_LayoutLen;
	int m_Level;
};

#endif // !defined(AFX_AUTOLAYOUT_H__75033F01_3667_45B3_8FD6_02C6A32932ED__INCLUDED_)
