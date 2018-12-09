// LayoutLinkNode.cpp: implementation of the CLayoutLinkNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LayoutLinkNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayoutLinkNode::CLayoutLinkNode(const CString& layout, CLayoutLinkNode* parent)
{
	m_Layout = layout;
	m_Parent = parent;
}

CLayoutLinkNode::~CLayoutLinkNode()
{
	
}

CString& CLayoutLinkNode::GetLayout()
{
	return m_Layout;
}

CStringList* CLayoutLinkNode::GetPath()
{
	CStringList* result = new CStringList;
	CLayoutLinkNode* node = this;
	while (node)
	{
		result->AddTail(node->m_Layout);
		node = node->m_Parent;
	}
	return result;
}
