// AutoLayout.h: interface for the CAutoLayout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOLAYOUT_H__75033F01_3667_45B3_8FD6_02C6A32932ED__INCLUDED_)
#define AFX_AUTOLAYOUT_H__75033F01_3667_45B3_8FD6_02C6A32932ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAutoLayout  
{
public:
	int* LayoutStringToArray(const CString& layout);
	CString LayoutArrayToString(const int* layout, int len);
	CStringList* LayoutBFS();
	CAutoLayout(const int *layout, int len);
	virtual ~CAutoLayout();

private:
	bool CheckClosed(const CString& layout);
	bool CheckFinish(const CString& layout);
	CStringList* GetNextLayouts(const CString& layout);
	CMapStringToPtr m_CloseLayouts;
	CString m_OriginalLayout;
	CString m_TargetLayout;
	int m_LayoutLen;
	int m_Level;
};

#endif // !defined(AFX_AUTOLAYOUT_H__75033F01_3667_45B3_8FD6_02C6A32932ED__INCLUDED_)
