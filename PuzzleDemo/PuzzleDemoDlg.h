
// PuzzleDemoDlg.h : 头文件
//

#pragma once
#include "Puzzle.h"


// CPuzzleDemoDlg 对话框
class CPuzzleDemoDlg : public CDialogEx
{
// 构造
public:
	CPuzzleDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PUZZLEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CPuzzle m_Puzzle;
public:
	afx_msg void OnBnClickedAuto();
	afx_msg void OnBnClickedReset();
	virtual void OnOK();
	afx_msg void OnBnClickedLevel1();
	afx_msg void OnBnClickedLevel2();
	afx_msg void OnBnClickedLevel3();
};
