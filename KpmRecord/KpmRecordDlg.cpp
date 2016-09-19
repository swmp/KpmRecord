
// KpmRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KpmRecord.h"
#include "KpmRecordDlg.h"
#include "afxdialogex.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKpmRecordDlg 对话框

HHOOK myhook;
int m_nKeyIndex;
int m_aKeyPerSecond[60];
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

CKpmRecordDlg::CKpmRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKpmRecordDlg::IDD, pParent)
	, m_nKeyCount(0)
	, m_nKeyCountOfDay(0)
	, m_nToday(0)
	, m_nMinute(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKpmRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKpmRecordDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CKpmRecordDlg 消息处理程序

BOOL CKpmRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SetWindowPos(&wndTop, 0, 0, 0, 0, NULL);	
	InitRecord();
	LoadInfo();	
	InstallHook();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKpmRecordDlg::OnPaint()
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
HCURSOR CKpmRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKpmRecordDlg::InstallHook()
{
	//设置钩子                                      钩子类型(键盘消息)  应用程序的事件句柄  
	myhook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // 监听类型【键盘】
		KeyboardProc,   // 处理函数
		0,      // 当前实例句柄
		0               // 监听窗口句柄(NULL为全局监听)
		);
	SetTimer(KEYTIMER, 1000,NULL);

	
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (WM_KEYUP == wParam)
		{
			++m_aKeyPerSecond[m_nKeyIndex];
		}
	}
	//用于对当前钩子链中的下一个钩子进程传递钩子信息  
	return CallNextHookEx(myhook,//当前钩子句柄  
		nCode,//传送到钩子进程的钩子代码  
		wParam,//传送到钩子进程的值。  
		lParam);//传送到钩子进程的值。  
}

void CKpmRecordDlg::EndHook(void)
{
	//钩子释放  
	UnhookWindowsHookEx(myhook);
	KillTimer(KEYTIMER);
}

void CKpmRecordDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	EndHook();
}


void CKpmRecordDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == KEYTIMER)
	{
		UpdateKeyTime();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CKpmRecordDlg::InitRecord()
{
	memset(m_aKeyPerSecond, 0, sizeof(m_aKeyPerSecond));
	m_nKeyIndex = 0;
	m_nTodayKpm = 0;
	m_nKeyCount = 0;
	m_nMinute = 0;
	m_nKeyCountOfDay = 0;
}

void CKpmRecordDlg::UpdateKeyTime()
{
	m_nKeyCountOfDay += m_aKeyPerSecond[m_nKeyIndex];
	if (m_nTopKeyCount < m_nKeyCountOfDay)
	{
		m_nTopKeyCount = m_nKeyCountOfDay;
	}
	m_nKeyCount = m_nKeyCount - m_nOldKeyCount + m_aKeyPerSecond[m_nKeyIndex];
	if (m_nTodayKpm < m_nKeyCount)
	{
		m_nTodayKpm = m_nKeyCount;
		if (m_nTopKpm < m_nKeyCount)
		{
			m_nTopKpm = m_nKeyCount;
		}
	}
	SaveInfo();
	DrawKpmInfo();
	time_t tmNow = time(NULL);
	tm* ptNow = new tm();
	localtime_s(ptNow, &tmNow);	
	if (ptNow->tm_yday != m_nToday)
	{
		if (m_nToday)
		{
			InitRecord();
		}
		m_nToday = ptNow->tm_yday;		
	}
	m_nKeyIndex = ptNow->tm_sec;		
	m_nOldKeyCount = m_aKeyPerSecond[m_nKeyIndex];
	m_aKeyPerSecond[m_nKeyIndex] = 0;
}
WCHAR wszKpmInfo[256] = { 0 };
void CKpmRecordDlg::DrawKpmInfo()
{	
	memset(wszKpmInfo, 0, sizeof(wszKpmInfo));
	wsprintf(wszKpmInfo, L"%04d - %04d - %04d\r\n%04d - %04d - %04d", m_nKeyCount, m_nTodayKpm, m_nTopKpm, m_nKeyCountOfDay, m_nAvageKeyCount, m_nTopKeyCount);
	int  nFullWidth = GetSystemMetrics(SM_CXSCREEN);
	//int  nFullHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC hdc = ::GetDC(0);
	CDC *pDc = CDC::FromHandle(hdc);
	pDc->SetTextColor(RGB(255, 0, 255));
	//pDc->SetBkMode(TRANSPARENT);
	CRect rc(nFullWidth/2, 30, nFullWidth, 80);
	pDc->DrawText(wszKpmInfo, rc, DT_CENTER);
	ReleaseDC(pDc);
}
char szRecordLine[64] = { 0 };
void CKpmRecordDlg::SaveInfo()
{
	std::fstream fsFile;
	fsFile.open("Kpm.txt", std::ios::out | std::ios::trunc);//打开一个文件，然后清空内容
	memset(szRecordLine,0, sizeof(szRecordLine));
	sprintf_s(szRecordLine, "%d-%d\r\n", m_nTopKpm, m_nTopKeyCount);
	fsFile << szRecordLine;
	memset(szRecordLine, 0, sizeof(szRecordLine));
	sprintf_s(szRecordLine, "%d-%d-%d\r\n", m_nToday, m_nTodayKpm, m_nKeyCountOfDay);
	fsFile << szRecordLine;
	std::list<KpmRecord>::iterator it = m_lRecords.begin();
	bool bFirst = true;
	while (m_lRecords.end() != it)
	{
		if (bFirst)
		{
			bFirst = false;
			if (it->nDay == m_nToday)
			{
				++it;
				continue;;
			}			
		}
		memset(szRecordLine, 0, sizeof(szRecordLine));
		sprintf_s(szRecordLine, "%d-%d-%d\r\n", it->nDay, it->nKpm, it->nKeyCount);
		fsFile << szRecordLine;
		++it;
	}	
	fsFile.close();
}
char szSub[16] = { 0 };
void CKpmRecordDlg::LoadInfo()
{
	time_t tmNow = time(NULL);
	tm* ptNow = new tm();
	localtime_s(ptNow, &tmNow);
	m_nToday = ptNow->tm_yday;

	std::fstream fsFile;
	fsFile.open("Kpm.txt", std::ios::in);//以读取方式打开
	if (!fsFile)
	{
		return;
	}
	if (!fsFile.eof())
	{
		memset(szRecordLine, 0, sizeof(szRecordLine));
		fsFile.getline(szRecordLine, sizeof(szRecordLine));
		char* pGap = strchr(szRecordLine, '-');
		if (pGap)
		{
			memset(szSub, 0, sizeof(szSub));
			strncpy_s(szSub, szRecordLine, pGap - szRecordLine);
			m_nTopKpm = atoi(szSub);
			++pGap;
			m_nTopKeyCount = atoi(pGap);
		}		
	}
	int nCount = 0;
	while (!fsFile.eof())
	{		
		memset(szRecordLine, 0, sizeof(szRecordLine));
		fsFile.getline(szRecordLine, sizeof(szRecordLine));
		char* pGap = strchr(szRecordLine, '-');
		if (pGap)
		{
			KpmRecord rd;
			memset(szSub, 0, sizeof(szSub));
			strncpy_s(szSub, szRecordLine, pGap - szRecordLine);
			rd.nDay = atoi(szSub);
			++pGap;
			char* pGap2 = strchr(pGap, '-');
			if (pGap2)
			{
				memset(szSub, 0, sizeof(szSub));
				strncpy_s(szSub, pGap, pGap2 - pGap);
				rd.nKpm = atoi(pGap);
				++pGap2;
				rd.nKeyCount = atoi(pGap2);
				if (0 == nCount)
				{
					if (rd.nDay == m_nToday)
					{
						m_nTodayKpm = rd.nKpm;
						m_nKeyCountOfDay = rd.nKeyCount;
					}					
				}
				m_lRecords.push_back(rd);				
				++nCount;
			}			
			
		}
		
	}
	fsFile.close();
}