
// PuzzleDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PuzzleDemo.h"
#include "PuzzleDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPuzzleDemoDlg 对话框



CPuzzleDemoDlg::CPuzzleDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPuzzleDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPuzzleDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPuzzleDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AUTO, &CPuzzleDemoDlg::OnBnClickedAuto)
	ON_BN_CLICKED(IDC_RESET, &CPuzzleDemoDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_LEVEL1, &CPuzzleDemoDlg::OnBnClickedLevel1)
	ON_BN_CLICKED(IDC_LEVEL2, &CPuzzleDemoDlg::OnBnClickedLevel2)
	ON_BN_CLICKED(IDC_LEVEL3, &CPuzzleDemoDlg::OnBnClickedLevel3)
END_MESSAGE_MAP()


// CPuzzleDemoDlg 消息处理程序

BOOL CPuzzleDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CWnd *pWnd = GetDlgItem(IDC_PUZZLE);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	pWnd->DestroyWindow();
	this->ScreenToClient(&rect);
	m_Puzzle.CreateEx(WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 0, NULL);

	CButton radio;
	radio.Attach(GetDlgItem(IDC_LEVEL1)->m_hWnd);
	radio.SetCheck(TRUE);
	radio.Detach();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPuzzleDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPuzzleDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPuzzleDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPuzzleDemoDlg::OnBnClickedAuto()
{
	// TODO: Add your control notification handler code here
	this->m_Puzzle.AutoLayout();
}


void CPuzzleDemoDlg::OnBnClickedReset()
{
	// TODO: Add your control notification handler code here
	this->m_Puzzle.Reset();
}


void CPuzzleDemoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


void CPuzzleDemoDlg::OnBnClickedLevel1()
{
	// TODO: Add your control notification handler code here
	this->m_Puzzle.SetLevel(3);
	this->m_Puzzle.Reset();
}


void CPuzzleDemoDlg::OnBnClickedLevel2()
{
	// TODO: Add your control notification handler code here
	this->m_Puzzle.SetLevel(4);
	this->m_Puzzle.Reset();
}


void CPuzzleDemoDlg::OnBnClickedLevel3()
{
	// TODO: Add your control notification handler code here
	this->m_Puzzle.SetLevel(5);
	this->m_Puzzle.Reset();
}
