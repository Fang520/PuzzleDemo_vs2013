// Puzzle.cpp : implementation file
//

#include "stdafx.h"
#include "PuzzleDemo.h"
#include "Puzzle.h"
#include "AutoLayoutAStar.h"

// CPuzzle

IMPLEMENT_DYNAMIC(CPuzzle, CWnd)

CPuzzle::CPuzzle()
{
	m_NumberList = NULL;
	srand((unsigned int)time(NULL));
	SetLevel(3);
	GenerateRandomSequence();
	/*
	m_NumberList[0] = 2;
	m_NumberList[1] = 1;
	m_NumberList[2] = 3;
	m_NumberList[3] = 4;
	m_NumberList[4] = 6;
	m_NumberList[5] = 5;
	m_NumberList[6] = 7;
	m_NumberList[7] = 9;
	m_NumberList[8] = 8;
	*/
}

CPuzzle::~CPuzzle()
{
	if (m_NumberList)
	{
		free(m_NumberList);
		m_NumberList = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPuzzle, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CPuzzle message handlers




void CPuzzle::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	DrawPuzzle(&dc);
}


void CPuzzle::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MoveCell(point);
	Invalidate();
	//CWnd::OnLButtonDown(nFlags, point);
}

void CPuzzle::DrawPuzzle(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int cw = rect.Width() / m_Level; //cell width
	int ch = rect.Height() / m_Level; //cell height

	CBrush bk_brush(RGB(190, 190, 190));
	pDC->FillRect(rect, &bk_brush);

	for (int i = 0; i<m_Level*m_Level; i++)
	{
		int col = i % m_Level;
		int row = i / m_Level;
		CRect rc;
		rc.left = cw * col + 1;
		rc.top = ch * row + 1;
		rc.right = rc.left + cw;
		rc.bottom = rc.top + ch;
		//rc.DeflateRect(1, 1);
		if (m_NumberList[i] != m_Level*m_Level)
		{
			CString title;
			title.Format(_T("%d"), m_NumberList[i]);
			pDC->DrawFrameControl(&rc, DFC_BUTTON, DFCS_BUTTONPUSH);
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(title, rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}
}

void CPuzzle::GenerateRandomSequence()
{
	if (m_Level == 6)
	{
		for (int i = 0; i < 25; i++)
		{
			m_NumberList[i] = i + 1;
		}
		m_NumberList[5] = 25;
		m_NumberList[24] = 6;
		return;
	}

	if (m_Level == 6)
	{
		for (int i = 0; i < 16; i++)
		{
			m_NumberList[i] = i + 1;
		}
		m_NumberList[4] = 6;
		m_NumberList[5] = 5;
		m_NumberList[10] = 16;
		m_NumberList[15] = 11;
		return;
	}

	
	int i, n, t;
	int len = m_Level*m_Level;

	// init list
	for (i = 0; i<len; i++)
	{
		m_NumberList[i] = i + 1;
	}

	// random change the order
	for (i = len - 1; i>0; i--)
	{
		n = rand() % i;
		t = m_NumberList[i];
		m_NumberList[i] = m_NumberList[n];
		m_NumberList[n] = t;
	}
}

void CPuzzle::Reset()
{
	GenerateRandomSequence();
	Invalidate();
}

void CPuzzle::AutoLayout()
{
	CAutoLayoutAStar al(m_NumberList, m_Level * m_Level);
	BeginWaitCursor();
	m_Steps = al.LayoutBFS();
	EndWaitCursor();
	if (m_Steps.size() != 0)
	{
		printf("set timer\n");
		SetTimer(0, 100, NULL);
	}
	else
	{
		MessageBox(_T("There is no solution !"));
	}
}

void CPuzzle::MoveCell(CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	int cw = rect.Width() / m_Level; //cell width
	int ch = rect.Height() / m_Level; //cell height
	int col = point.x / cw;
	int row = point.y / ch;
	int index = row * m_Level + col;

	int near_index[4];
	near_index[0] = index - m_Level;
	near_index[1] = index + m_Level;
	near_index[2] = index - 1;
	if (near_index[2] / m_Level != index / m_Level) //fix bug, if not in the same row must be ignored 
		near_index[2] = -1;
	near_index[3] = index + 1;
	if (near_index[3] / m_Level != index / m_Level)
		near_index[3] = -1;

	for (int i = 0; i<4; i++)
	{
		int probe_index = near_index[i];
		if (probe_index >= 0 && probe_index < m_Level*m_Level)
		{
			if (m_NumberList[probe_index] == m_Level*m_Level)
			{
				int n = m_NumberList[index];
				m_NumberList[index] = m_NumberList[probe_index];
				m_NumberList[probe_index] = n;
				CheckFinish();
			}
		}
	}
}

void CPuzzle::CheckFinish()
{
	bool result = true;
	for (int i = 0; i<m_Level*m_Level; i++)
	{
		if (m_NumberList[i] != i + 1)
		{
			result = false;
			break;
		}
	}
	if (result)
	{
		MessageBox(_T("Finished"));
	}
}

void CPuzzle::SetLevel(int level)
{
	if (level < 3 || level > 9)
	{
		return;
	}
	m_Level = level;
	if (m_NumberList)
	{
		free(m_NumberList);
	}
	m_NumberList = (char*)malloc(level * level * sizeof(char));
}


void CPuzzle::OnTimer(UINT_PTR nIDEvent)
{
	printf("step=%d\n", m_Steps.size());
	std::vector<char> item = m_Steps.front();
	for (unsigned int i = 0; i < item.size(); i++)
	{
		m_NumberList[i] = item[i];
	}
	m_Steps.pop_front();
	if (m_Steps.empty())
	{
		printf("kill timer\n");
		KillTimer(nIDEvent);
	}
	Invalidate();
	CWnd::OnTimer(nIDEvent);
}