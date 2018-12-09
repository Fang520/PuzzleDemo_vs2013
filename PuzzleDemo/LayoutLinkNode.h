// LayoutLinkNode.h: interface for the CLayoutLinkNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYOUTLINKNODE_H__EB3133AD_F8D0_47D7_916C_E5CFD9E8F0EA__INCLUDED_)
#define AFX_LAYOUTLINKNODE_H__EB3133AD_F8D0_47D7_916C_E5CFD9E8F0EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLayoutLinkNode  
{
public:
	CLayoutLinkNode(const CString& layout, CLayoutLinkNode* parent);
	virtual ~CLayoutLinkNode();
	CString& GetLayout();
	CStringList* GetPath();
private:
	CString m_Layout;
	CLayoutLinkNode* m_Parent;
};

#endif // !defined(AFX_LAYOUTLINKNODE_H__EB3133AD_F8D0_47D7_916C_E5CFD9E8F0EA__INCLUDED_)
