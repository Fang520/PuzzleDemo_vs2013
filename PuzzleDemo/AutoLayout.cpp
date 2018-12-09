// AutoLayout.cpp: implementation of the CAutoLayout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "AutoLayout.h"
#include "LayoutLinkNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoLayout::CAutoLayout(const int *layout, int len)
{
	m_OriginalLayout = LayoutArrayToString(layout, len);
	CString tmp = _T("");
	for (int i=1; i<=len; i++)
	{
		tmp.Format(_T("%d "), i);
		m_TargetLayout += tmp;
	}
	m_LayoutLen = len;
	m_Level = (int)sqrt((float)len);
	m_CloseLayouts.InitHashTable(218357);
}

CAutoLayout::~CAutoLayout()
{

}

CStringList* CAutoLayout::LayoutBFS()
{
	CStringList* result = NULL;
	CPtrList free_layouts;
	CPtrList open_layouts;
	CLayoutLinkNode* node = new CLayoutLinkNode(m_OriginalLayout, NULL); // vc6 don't raise exception if new fail
	open_layouts.AddHead(node);
	while (open_layouts.GetCount() > 0)
	{
		CLayoutLinkNode* node = (CLayoutLinkNode*)open_layouts.RemoveHead();
		free_layouts.AddTail(node);//为了释放, 这里必须保存node
		CString layout = node->GetLayout();

		if (CheckClosed(layout))
		{
			continue;
		}

		if (CheckFinish(layout))
		{
			result = node->GetPath();
			break;
		}

		CStringList* nexts = GetNextLayouts(layout);
		POSITION pos;
		pos = nexts->GetHeadPosition();
		while (pos != NULL)
		{
			CString new_layout = nexts->GetNext(pos);
			CLayoutLinkNode* new_node = new CLayoutLinkNode(new_layout, node);
			open_layouts.AddTail(new_node);
		}
		delete nexts;

		m_CloseLayouts.SetAt(layout, NULL);
	}

	if (!free_layouts.IsEmpty())
	{
		POSITION pos = free_layouts.GetHeadPosition();
		while (pos != NULL)
		{
			CLayoutLinkNode* node = (CLayoutLinkNode*)free_layouts.GetNext(pos);
			delete node;
		}
	}

	if (!open_layouts.IsEmpty())
	{
		POSITION pos = open_layouts.GetHeadPosition();
		while (pos != NULL)
		{
			CLayoutLinkNode* node = (CLayoutLinkNode*)open_layouts.GetNext(pos);
			delete node;
		}
	}

	return result;
}

int* CAutoLayout::LayoutStringToArray(const CString& layout)
{
	static int result[256];
	int start = 0;
	int end = 0;
	int i = 0;
	while (end != -1)
	{
		end = layout.Find(' ', start);
		if (end != -1)
		{
			CString s = layout.Mid(start, end - start);
			start = end + 1;
			result[i] = _wtoi(s);
			i++;
		}
	}
	return result;
}

CString CAutoLayout::LayoutArrayToString(const int* layout, int len)
{
	CString result = _T("");
	CString tmp = _T("");
	for (int i=0; i<len; i++)
	{
		tmp.Format(_T("%d "), layout[i]);
		result += tmp;
	}
	return result;
}

CStringList* CAutoLayout::GetNextLayouts(const CString& layout)
{
	int i, j;
	int* data = LayoutStringToArray(layout);
	int index = -1;
    for (i=0 ; i<m_LayoutLen; i++)
    {
        if (m_LayoutLen == data[i])
        {
            index = i;
            break;
        }
    }

	int near_index[4];
	near_index[0] = index - m_Level;
	near_index[1] = index + m_Level;
	near_index[2] = index - 1;
	near_index[3] = index + 1;
	if (near_index[2] / m_Level != index / m_Level)
		near_index[2] = -1;
	if (near_index[3] / m_Level != index / m_Level)
		near_index[3] = -1;
    
	int new_data[256];
	CStringList* result = new CStringList;
    for (i=0; i<4; i++)
    {
		int probe_index = near_index[i];
		if (probe_index >=0 && probe_index < m_LayoutLen)
		{
            for (j=0; j<m_LayoutLen; j++)
            {
                new_data[j] = data[j];
            }
            new_data[probe_index] = data[index];
            new_data[index] = data[probe_index];
			CString new_layout = LayoutArrayToString(new_data, m_LayoutLen);
			result->AddTail(new_layout);
		}
    }
	return result;
}

bool CAutoLayout::CheckFinish(const CString &layout)
{
	if (m_TargetLayout == layout)
	{
		return true;
	}
	return false;
}

bool CAutoLayout::CheckClosed(const CString &layout)
{
	void* value;
	if (m_CloseLayouts.Lookup(layout, value))
	{
		return true;
	}
	return false;
}