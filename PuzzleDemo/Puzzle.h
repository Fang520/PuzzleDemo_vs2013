#pragma once
#include <list>
#include <vector>

// CPuzzle

class CPuzzle : public CWnd
{
	DECLARE_DYNAMIC(CPuzzle)

public:
	CPuzzle();
	virtual ~CPuzzle();
	void SetLevel(int level);
	void AutoLayout();
	void Reset();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	DECLARE_MESSAGE_MAP()
private:
	char* m_NumberList;
	int m_Level;
	std::list<std::vector<char>> m_Steps;
	void CheckFinish();
	void DrawPuzzle(CDC* pDC);
	void MoveCell(CPoint point);
	void GenerateRandomSequence();
};


