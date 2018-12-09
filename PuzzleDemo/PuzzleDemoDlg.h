
// PuzzleDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "Puzzle.h"


// CPuzzleDemoDlg �Ի���
class CPuzzleDemoDlg : public CDialogEx
{
// ����
public:
	CPuzzleDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PUZZLEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
