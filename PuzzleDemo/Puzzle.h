#pragma once


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

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	int* m_NumberList;
	int m_Level;
	void CheckFinish();
	void DrawPuzzle(CDC* pDC);
	void MoveCell(CPoint point);
	void GenerateRandomSequence();
};


