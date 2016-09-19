
// KpmRecordDlg.cpp : ʵ���ļ�
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


// CKpmRecordDlg �Ի���

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


// CKpmRecordDlg ��Ϣ�������

BOOL CKpmRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SetWindowPos(&wndTop, 0, 0, 0, 0, NULL);	
	InitRecord();
	LoadInfo();	
	InstallHook();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKpmRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CKpmRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKpmRecordDlg::InstallHook()
{
	//���ù���                                      ��������(������Ϣ)  Ӧ�ó�����¼����  
	myhook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // �������͡����̡�
		KeyboardProc,   // ������
		0,      // ��ǰʵ�����
		0               // �������ھ��(NULLΪȫ�ּ���)
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
	//���ڶԵ�ǰ�������е���һ�����ӽ��̴��ݹ�����Ϣ  
	return CallNextHookEx(myhook,//��ǰ���Ӿ��  
		nCode,//���͵����ӽ��̵Ĺ��Ӵ���  
		wParam,//���͵����ӽ��̵�ֵ��  
		lParam);//���͵����ӽ��̵�ֵ��  
}

void CKpmRecordDlg::EndHook(void)
{
	//�����ͷ�  
	UnhookWindowsHookEx(myhook);
	KillTimer(KEYTIMER);
}

void CKpmRecordDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������
	EndHook();
}


void CKpmRecordDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	fsFile.open("Kpm.txt", std::ios::out | std::ios::trunc);//��һ���ļ���Ȼ���������
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
	fsFile.open("Kpm.txt", std::ios::in);//�Զ�ȡ��ʽ��
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